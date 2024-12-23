/*
 * Teensy4 IOIO-OTG Project
 *
 * Copyright 2011 Ytai Ben-Tsvi. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 *    1. Redistributions of source code must retain the above copyright notice, this list of
 *       conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above copyright notice, this list
 *       of conditions and the following disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ARSHAN POURSOHI OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those of the
 * authors and should not be interpreted as representing official policies, either expressed
 * or implied.
 *
 */

#include <assert.h>
#include <string.h>

#include "protocol.h"
#include "byte_queue.h"
#include "features.h"
#include "pwm.h"
#include "adc.h"
#include "digital.h"
#include "logging.h"
#include "platform.h"
#include "uart.h"
#include "spi.h"
#include "i2c.h"
#include "sync.h"
#include "icsp.h"
#include "incap.h"
#include "sequencer_protocol.h"

#define CHECK(cond) do { if (!(cond)) { log("Check failed: %s", #cond); return false; }} while(0)

const uint8_t incoming_arg_size[MESSAGE_TYPE_LIMIT] = {
  sizeof(HARD_RESET_ARGS),
  sizeof(SOFT_RESET_ARGS),
  sizeof(CHECK_INTERFACE_ARGS),
  sizeof(SET_PIN_DIGITAL_OUT_ARGS),
  sizeof(SET_DIGITAL_OUT_LEVEL_ARGS),
  sizeof(SET_PIN_DIGITAL_IN_ARGS),
  sizeof(SET_CHANGE_NOTIFY_ARGS),
  sizeof(REGISTER_PERIODIC_DIGITAL_SAMPLING_ARGS),
  sizeof(SET_PIN_PWM_ARGS),
  sizeof(SET_PWM_DUTY_CYCLE_ARGS),
  sizeof(SET_PWM_PERIOD_ARGS),
  sizeof(SET_PIN_ANALOG_IN_ARGS),
  sizeof(SET_ANALOG_IN_SAMPLING_ARGS),
  sizeof(UART_CONFIG_ARGS),
  sizeof(UART_DATA_ARGS),
  sizeof(SET_PIN_UART_ARGS),
  sizeof(SPI_CONFIGURE_MASTER_ARGS),
  sizeof(SPI_MASTER_REQUEST_ARGS),
  sizeof(SET_PIN_SPI_ARGS),
  sizeof(I2C_CONFIGURE_MASTER_ARGS),
  sizeof(I2C_WRITE_READ_ARGS),
  sizeof(RESERVED_ARGS),
  sizeof(ICSP_SIX_ARGS),
  sizeof(ICSP_REGOUT_ARGS),
  sizeof(ICSP_PROG_ENTER_ARGS),
  sizeof(ICSP_PROG_EXIT_ARGS),
  sizeof(ICSP_CONFIG_ARGS),
  sizeof(INCAP_CONFIG_ARGS),
  sizeof(SET_PIN_INCAP_ARGS),
  sizeof(SOFT_CLOSE_ARGS),
  sizeof(SET_PIN_CAPSENSE_ARGS),
  sizeof(SET_CAPSENSE_SAMPLING_ARGS),
  sizeof(SEQUENCER_CONFIGURE_ARGS),
  sizeof(SEQUENCER_PUSH_ARGS),
  sizeof(SEQUENCER_CONTROL_ARGS),
  sizeof(RESERVED_ARGS)

  // BOOKMARK(add_feature): Add sizeof (argument for incoming message).
  // Array is indexed by message type enum.
};

const uint8_t outgoing_arg_size[MESSAGE_TYPE_LIMIT] = {
  sizeof(ESTABLISH_CONNECTION_ARGS),
  sizeof(SOFT_RESET_ARGS),
  sizeof(CHECK_INTERFACE_RESPONSE_ARGS),
  sizeof(RESERVED_ARGS),
  sizeof(REPORT_DIGITAL_IN_STATUS_ARGS),
  sizeof(RESERVED_ARGS),
  sizeof(SET_CHANGE_NOTIFY_ARGS),
  sizeof(REGISTER_PERIODIC_DIGITAL_SAMPLING_ARGS),
  sizeof(RESERVED_ARGS),
  sizeof(RESERVED_ARGS),
  sizeof(RESERVED_ARGS),
  sizeof(REPORT_ANALOG_IN_STATUS_ARGS),
  sizeof(REPORT_ANALOG_IN_FORMAT_ARGS),
  sizeof(UART_STATUS_ARGS),
  sizeof(UART_DATA_ARGS),
  sizeof(UART_REPORT_TX_STATUS_ARGS),
  sizeof(SPI_STATUS_ARGS),
  sizeof(SPI_DATA_ARGS),
  sizeof(SPI_REPORT_TX_STATUS_ARGS),
  sizeof(I2C_STATUS_ARGS),
  sizeof(I2C_RESULT_ARGS),
  sizeof(I2C_REPORT_TX_STATUS_ARGS),
  sizeof(ICSP_REPORT_RX_STATUS_ARGS),
  sizeof(ICSP_RESULT_ARGS),
  sizeof(RESERVED_ARGS),
  sizeof(RESERVED_ARGS),
  sizeof(ICSP_CONFIG_ARGS),
  sizeof(INCAP_STATUS_ARGS),
  sizeof(INCAP_REPORT_ARGS),
  sizeof(SOFT_CLOSE_ARGS),
  sizeof(CAPSENSE_REPORT_ARGS),
  sizeof(SET_CAPSENSE_SAMPLING_ARGS),
  sizeof(SEQUENCER_EVENT_ARGS),
  sizeof(RESERVED_ARGS),
  sizeof(RESERVED_ARGS),
  sizeof(RESERVED_ARGS)

  // BOOKMARK(add_feature): Add sizeof (argument for outgoing message).
  // Array is indexed by message type enum.
};

typedef enum {
  STATE_OPEN,
  STATE_CLOSING,
  STATE_CLOSED
} STATE;

typedef enum {
  WAIT_TYPE,
  WAIT_ARGS,
  WAIT_VAR_ARGS
} RX_MESSAGE_STATE;

// Not enough RAM in the 24K RAM (old prototypes) platforms, since the
// introduction of the motion control library.
#define QUEUE_SIZE 8192

DEFINE_STATIC_BYTE_QUEUE(tx_queue, QUEUE_SIZE);
static STATE state;
static INCOMING_MESSAGE rx_msg;
static uint32_t rx_buffer_cursor;
static uint32_t rx_message_remaining;
static RX_MESSAGE_STATE rx_message_state;

static inline uint8_t OutgoingMessageLength(const OUTGOING_MESSAGE *msg) {
  return 1 + outgoing_arg_size[msg->type];
}

static inline uint8_t IncomingVarArgSize(const INCOMING_MESSAGE *msg) {
  switch (msg->type) {
  case UART_DATA:
    return msg->args.uart_data.size + 1;

  case SPI_MASTER_REQUEST:
    if (msg->args.spi_master_request.data_size_neq_total) {
      return msg->args.spi_master_request.data_size + msg->args.spi_master_request.res_size_neq_total;
    } else {
      return msg->args.spi_master_request.total_size + msg->args.spi_master_request.res_size_neq_total;
    }

  case I2C_WRITE_READ:
    return msg->args.i2c_write_read.write_size;

  case SEQUENCER_CONFIGURE:
    return msg->args.sequencer_configure.size;

  case SEQUENCER_PUSH:
    return SequencerExpectedCueSize();

  case SEQUENCER_CONTROL:
    return msg->args.sequencer_control.cmd == SEQ_CMD_MANUAL_START
      ? SequencerExpectedCueSize()
      : 0;

    // BOOKMARK(add_feature): Add more cases here if incoming message has variable args.
  default:
    return 0;
  }
}

void AppProtocolInit(CHANNEL_HANDLE h) {
  rx_buffer_cursor = 0;
  rx_message_remaining = 1;
  rx_message_state = WAIT_TYPE;
  ByteQueueClear(&tx_queue);
  state = STATE_OPEN;

  OUTGOING_MESSAGE msg;
  msg.type = ESTABLISH_CONNECTION;
  msg.args.establish_connection.magic = IOIO_MAGIC;

  memcpy(msg.args.establish_connection.hw_impl_ver, HW_IMPL_VER, 8);
  memcpy(msg.args.establish_connection.bl_impl_ver, BL_IMPL_VER, 8);
  memcpy(msg.args.establish_connection.fw_impl_ver, FW_IMPL_VER, 8);

  AppProtocolSendMessage(&msg);
}

void AppProtocolSendLogging(const uint8_t *msg, int size) {
  if (state != STATE_OPEN) return;
  PRIORITY(1) {
    ByteQueuePush(&tx_queue, DEBUG_OUT);
    ByteQueuePushBuffer(&tx_queue, msg, size);
    ByteQueuePush(&tx_queue, '\n');
  }
}

void AppProtocolSendMessage(const OUTGOING_MESSAGE *msg) {
  if (state != STATE_OPEN) return;
  PRIORITY(1) {
    ByteQueuePushBuffer(&tx_queue, (const uint8_t *) msg, OutgoingMessageLength(msg));
  }
}

void AppProtocolSendMessageWithVarArg(const OUTGOING_MESSAGE *msg, const void *data, int size) {
  if (state != STATE_OPEN) return;
  PRIORITY(1) {
    ByteQueuePushBuffer(&tx_queue, (const uint8_t *) msg, OutgoingMessageLength(msg));
    ByteQueuePushBuffer(&tx_queue, data, size);
  }
}

void AppProtocolSendMessageWithVarArgSplit(const OUTGOING_MESSAGE *msg,
                                           const void *data1, int size1,
                                           const void *data2, int size2) {
  if (state != STATE_OPEN) return;
  PRIORITY(1) {
    ByteQueuePushBuffer(&tx_queue, (const uint8_t *) msg, OutgoingMessageLength(msg));
    ByteQueuePushBuffer(&tx_queue, data1, size1);
    ByteQueuePushBuffer(&tx_queue, data2, size2);
  }
}

void AppProtocolTasks(CHANNEL_HANDLE h) {
  if (state == STATE_CLOSED) return;
  if (state == STATE_CLOSING && ByteQueueSize(&tx_queue) == 0) {
    log("Finished flushing, closing the channel.");
    ConnectionCloseChannel(h);
    state = STATE_CLOSED;
    return;
  }

  UARTTasks();
  SPITasks();
  I2CTasks();
  ICSPTasks();
  SequencerTasks();
  
  int available = ConnectionCanSend(h);
  if (available) {
    const uint8_t *data;
    int size;
    ByteQueuePeek(&tx_queue, &data, &size);
    if (size > available) {
      size = available;
    }
    if (size > 0) {
      ByteQueueDiscard(&tx_queue, size);
      ConnectionSend(h, data, size);
    }
  }
}

static void Echo() {
  AppProtocolSendMessage((const OUTGOING_MESSAGE *) &rx_msg);
}

static bool MessageDone() {
  // TODO: check pin capabilities
  switch (rx_msg.type) {
  case HARD_RESET:
    CHECK(rx_msg.args.hard_reset.magic == IOIO_MAGIC);
    HardReset();
    break;

  case SOFT_RESET:
    SoftReset();
    Echo();
    break;

  case SET_PIN_DIGITAL_OUT:
    CHECK(rx_msg.args.set_pin_digital_out.pin < NUM_PINS);
    SetPinDigitalOut(rx_msg.args.set_pin_digital_out.pin,
                     rx_msg.args.set_pin_digital_out.value,
                     rx_msg.args.set_pin_digital_out.open_drain);
    break;

  case SET_DIGITAL_OUT_LEVEL:
    CHECK(rx_msg.args.set_digital_out_level.pin < NUM_PINS);
    SetDigitalOutLevel(rx_msg.args.set_digital_out_level.pin,
                       rx_msg.args.set_digital_out_level.value);
    break;

  case SET_PIN_DIGITAL_IN:
    CHECK(rx_msg.args.set_pin_digital_in.pin < NUM_PINS);
    CHECK(rx_msg.args.set_pin_digital_in.pull < 3);
    SetPinDigitalIn(rx_msg.args.set_pin_digital_in.pin, rx_msg.args.set_pin_digital_in.pull);
    break;

  case SET_CHANGE_NOTIFY:
    CHECK(rx_msg.args.set_change_notify.pin < NUM_PINS);
    if (rx_msg.args.set_change_notify.cn) {
      Echo();
    }
    SetChangeNotify(rx_msg.args.set_change_notify.pin, rx_msg.args.set_change_notify.cn);
    if (!rx_msg.args.set_change_notify.cn) {
      Echo();
    }
    break;

  case SET_PIN_PWM:
    CHECK(rx_msg.args.set_pin_pwm.pin < NUM_PINS);
    CHECK(rx_msg.args.set_pin_pwm.pwm_num < NUM_PWM_MODULES);
    SetPinPwm(rx_msg.args.set_pin_pwm.pin, rx_msg.args.set_pin_pwm.pwm_num,
              rx_msg.args.set_pin_pwm.enable);
    break;

  case SET_PWM_DUTY_CYCLE:
    CHECK(rx_msg.args.set_pwm_duty_cycle.pwm_num < NUM_PWM_MODULES);
    SetPwmDutyCycle(rx_msg.args.set_pwm_duty_cycle.pwm_num,
                    rx_msg.args.set_pwm_duty_cycle.dc,
                    rx_msg.args.set_pwm_duty_cycle.fraction);
    break;

  case SET_PWM_PERIOD:
    CHECK(rx_msg.args.set_pwm_period.pwm_num < NUM_PWM_MODULES);
    SetPwmPeriod(rx_msg.args.set_pwm_period.pwm_num,
                 rx_msg.args.set_pwm_period.period,
                 rx_msg.args.set_pwm_period.scale_l
                 | (rx_msg.args.set_pwm_period.scale_h) << 1);
    break;

  case SET_PIN_ANALOG_IN:
    CHECK(rx_msg.args.set_pin_analog_in.pin < NUM_PINS);
    SetPinAnalogIn(rx_msg.args.set_pin_analog_in.pin);
    break;

  case UART_DATA:
    CHECK(rx_msg.args.uart_data.uart_num < NUM_UART_MODULES);
    UARTTransmit(rx_msg.args.uart_data.uart_num,
                 rx_msg.args.uart_data.data,
                 rx_msg.args.uart_data.size + 1);
    break;

  case UART_CONFIG:
    CHECK(rx_msg.args.uart_config.uart_num < NUM_UART_MODULES);
    CHECK(rx_msg.args.uart_config.parity < 3);
    UARTConfig(rx_msg.args.uart_config.uart_num,
               rx_msg.args.uart_config.rate,
               rx_msg.args.uart_config.speed4x,
               rx_msg.args.uart_config.two_stop_bits,
               rx_msg.args.uart_config.parity);
    break;

  case SET_PIN_UART:
    CHECK(rx_msg.args.set_pin_uart.pin < NUM_PINS);
    CHECK(rx_msg.args.set_pin_uart.uart_num < NUM_UART_MODULES);
    SetPinUart(rx_msg.args.set_pin_uart.pin,
               rx_msg.args.set_pin_uart.uart_num,
               rx_msg.args.set_pin_uart.dir,
               rx_msg.args.set_pin_uart.enable);
    break;

  case SPI_MASTER_REQUEST:
    CHECK(rx_msg.args.spi_master_request.spi_num < NUM_SPI_MODULES);
    CHECK(rx_msg.args.spi_master_request.ss_pin < NUM_PINS); {
      const uint8_t total_size = rx_msg.args.spi_master_request.total_size + 1;
      const uint8_t data_size = rx_msg.args.spi_master_request.data_size_neq_total
                                ? rx_msg.args.spi_master_request.data_size
                                : total_size;
      const uint8_t res_size = rx_msg.args.spi_master_request.res_size_neq_total
                               ? rx_msg.args.spi_master_request.vararg[rx_msg.args.spi_master_request.data_size_neq_total]
                               : total_size;
      const uint8_t *const data = &rx_msg.args.spi_master_request.vararg[rx_msg.args.spi_master_request.data_size_neq_total
                                                                         + rx_msg.args.spi_master_request.res_size_neq_total];

      SPITransmit(rx_msg.args.spi_master_request.spi_num,
                  rx_msg.args.spi_master_request.ss_pin,
                  data,
                  data_size,
                  total_size,
                  total_size - res_size);
    }
    break;

  case SPI_CONFIGURE_MASTER:
    CHECK(rx_msg.args.spi_configure_master.spi_num < NUM_SPI_MODULES);
    SPIConfigMaster(rx_msg.args.spi_configure_master.spi_num,
                    rx_msg.args.spi_configure_master.scale,
                    rx_msg.args.spi_configure_master.div,
                    rx_msg.args.spi_configure_master.smp_end,
                    rx_msg.args.spi_configure_master.clk_edge,
                    rx_msg.args.spi_configure_master.clk_pol);
    break;

  case SET_PIN_SPI:
    CHECK(rx_msg.args.set_pin_spi.mode < 3);
    CHECK((!rx_msg.args.set_pin_spi.enable
           && rx_msg.args.set_pin_spi.mode == 1)
          || rx_msg.args.set_pin_spi.pin < NUM_PINS);
    CHECK((!rx_msg.args.set_pin_spi.enable
           && rx_msg.args.set_pin_spi.mode != 1)
          || rx_msg.args.set_pin_spi.spi_num < NUM_SPI_MODULES);
    SetPinSpi(rx_msg.args.set_pin_spi.pin,
              rx_msg.args.set_pin_spi.spi_num,
              rx_msg.args.set_pin_spi.mode,
              rx_msg.args.set_pin_spi.enable);
    break;

  case I2C_CONFIGURE_MASTER:
    CHECK(rx_msg.args.i2c_configure_master.i2c_num < NUM_I2C_MODULES);
    I2CConfigMaster(rx_msg.args.i2c_configure_master.i2c_num,
                    rx_msg.args.i2c_configure_master.rate,
                    rx_msg.args.i2c_configure_master.smbus_levels);
    break;

  case I2C_WRITE_READ:
    CHECK(rx_msg.args.i2c_write_read.i2c_num < NUM_I2C_MODULES); {
      unsigned int addr;
      if (rx_msg.args.i2c_write_read.ten_bit_addr) {
        addr = rx_msg.args.i2c_write_read.addr_lsb;
        addr = addr << 8
               | ((rx_msg.args.i2c_write_read.addr_msb << 1)
                  | 0b11110000);
      } else {
        CHECK(rx_msg.args.i2c_write_read.addr_msb == 0
              && rx_msg.args.i2c_write_read.addr_lsb >> 7 == 0
              && rx_msg.args.i2c_write_read.addr_lsb >> 2 != 0b0011110);
        addr = rx_msg.args.i2c_write_read.addr_lsb << 1;
      }
      I2CWriteRead(rx_msg.args.i2c_write_read.i2c_num,
                   addr,
                   rx_msg.args.i2c_write_read.data,
                   rx_msg.args.i2c_write_read.write_size,
                   rx_msg.args.i2c_write_read.read_size);
    }
    break;

  case SET_ANALOG_IN_SAMPLING:
    CHECK(rx_msg.args.set_analog_pin_sampling.pin < NUM_PINS);
    ADCSetScan(rx_msg.args.set_analog_pin_sampling.pin,
               rx_msg.args.set_analog_pin_sampling.enable);
    break;

  case CHECK_INTERFACE:
    CheckInterface(rx_msg.args.check_interface.interface_id);
    break;

  case ICSP_SIX:
    ICSPSix(rx_msg.args.icsp_six.inst);
    break;

  case ICSP_REGOUT:
    ICSPRegout();
    break;

  case ICSP_PROG_ENTER:
    ICSPEnter();
    break;

  case ICSP_PROG_EXIT:
    ICSPExit();
    break;

  case ICSP_CONFIG:
    if (rx_msg.args.icsp_config.enable) {
      Echo();
    }
    ICSPConfigure(rx_msg.args.icsp_config.enable);
    if (!rx_msg.args.icsp_config.enable) {
      Echo();
    }
    break;

  case INCAP_CONFIG:
    CHECK(rx_msg.args.incap_config.incap_num < NUM_INCAP_MODULES);
    CHECK(!rx_msg.args.incap_config.double_prec
          || 0 == (rx_msg.args.incap_config.incap_num & 0x01));
    CHECK(rx_msg.args.incap_config.mode < 6);
    CHECK(rx_msg.args.incap_config.clock < 4);
    InCapConfig(rx_msg.args.incap_config.incap_num,
                rx_msg.args.incap_config.double_prec,
                rx_msg.args.incap_config.mode,
                rx_msg.args.incap_config.clock);
    break;

  case SET_PIN_INCAP:
    CHECK(rx_msg.args.set_pin_incap.incap_num < NUM_INCAP_MODULES);
    CHECK(!rx_msg.args.set_pin_incap.enable
          || rx_msg.args.set_pin_incap.pin < NUM_PINS);
    SetPinInCap(rx_msg.args.set_pin_incap.pin,
                rx_msg.args.set_pin_incap.incap_num,
                rx_msg.args.set_pin_incap.enable);
    break;

  case SOFT_CLOSE:
    log("Soft close requested");
    Echo();
    state = STATE_CLOSING;
    break;

  case SET_PIN_CAPSENSE:
    CHECK(rx_msg.args.set_pin_capsense.pin < NUM_PINS);
    SetPinCapSense(rx_msg.args.set_pin_capsense.pin);
    break;

  case SET_CAPSENSE_SAMPLING:
    CHECK(rx_msg.args.set_capsense_sampling.pin < NUM_PINS);
    ADCSetCapSense(rx_msg.args.set_capsense_sampling.pin,
                   rx_msg.args.set_capsense_sampling.enable);
    break;

  case SEQUENCER_CONFIGURE:
    if (rx_msg.args.sequencer_configure.size) {
      return SequencerOpen(rx_msg.args.sequencer_configure.config,
                           rx_msg.args.sequencer_configure.size);
    } else {
      return SequencerClose();
    }

  case SEQUENCER_PUSH:
    return SequencerPush(rx_msg.args.sequencer_push.cue,
                         rx_msg.args.sequencer_push.time);

  case SEQUENCER_CONTROL:
    return SequencerCommand((SEQ_CMD) rx_msg.args.sequencer_control.cmd,
                            rx_msg.args.sequencer_control.extra);

  case SYNC:
    Echo();
    break;

    // BOOKMARK(add_feature): Add incoming message handling to switch clause.
    // Call Echo() if the message is to be echoed back.

  default:
    return false;
  }
  return true;
}

bool AppProtocolHandleIncoming(const uint8_t *data, uint32_t data_len) {
  assert(data);
  if (state != STATE_OPEN) {
    log("Shouldn't get data after close!");
    return false;
  }

  while (data_len > 0) {
    // copy a chunk of data to rx_msg
    if (data_len >= rx_message_remaining) {
      memcpy(((uint8_t *) &rx_msg) + rx_buffer_cursor, data, rx_message_remaining);
      data += rx_message_remaining;
      data_len -= rx_message_remaining;
      rx_buffer_cursor += rx_message_remaining;
      rx_message_remaining = 0;
    } else {
      memcpy(((uint8_t *) &rx_msg) + rx_buffer_cursor, data, data_len);
      rx_buffer_cursor += data_len;
      rx_message_remaining -= data_len;
      data_len = 0;
    }

    // change state
    if (rx_message_remaining == 0) {
      switch (rx_message_state) {
      case WAIT_TYPE:
        rx_message_state = WAIT_ARGS;
        rx_message_remaining = incoming_arg_size[rx_msg.type];
        if (rx_message_remaining) break;
        // fall-through on purpose

      case WAIT_ARGS:
        rx_message_state = WAIT_VAR_ARGS;
        rx_message_remaining = IncomingVarArgSize(&rx_msg);
        if (rx_message_remaining) break;
        // fall-through on purpose

      case WAIT_VAR_ARGS:
        rx_message_state = WAIT_TYPE;
        rx_message_remaining = 1;
        rx_buffer_cursor = 0;
        if (!MessageDone()) return false;
        break;
      }
    }
  }
  return true;
}
