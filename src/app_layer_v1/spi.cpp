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

#include "spi.h"
#include "byte_queue.h"
#include "field_accessors.h"
#include "platform.h"
#include "logging.h"
#include "pins.h"
#include "pp_util.h"
#include "protocol.h"
#include "sync.h"

#define RX_BUF_SIZE 256
#define TX_BUF_SIZE 256
#define INT_PRIORITY 3

typedef enum {
  PACKET_STATE_IDLE,
  PACKET_STATE_IN_PROGRESS,
  PACKET_STATE_DONE
} PACKET_STATE;

typedef struct {
  PACKET_STATE packet_state;
  volatile int num_tx_since_last_report;
  uint8_t cur_msg_dest;
  uint8_t cur_msg_total_tx;  // total number of bytes left to send
  uint8_t cur_msg_data_tx;   // number of *non-garbage* bytes left to send
  uint8_t cur_msg_trim_rx;   // number of *garbage* bytes left to read
  uint8_t cur_msg_total_rx;  // number of total bytes left to read
  uint8_t cur_msg_rx_size;   // number of bytes to send back
  uint8_t can_send;          // number of bytes available in the FIFO

  // message format:
  // BYTE dest
  // BYTE tx_size
  // BYTE tx_data[tx_size]
  ByteQueue rx_queue;

  volatile int num_messages_rx_queue;

  // message format:
  // BYTE dest
  // BYTE total_size
  // BYTE data_size
  // BYTE rx_trim
  // BYTE tx_data[tx_size]
  ByteQueue tx_queue;

  uint8_t rx_buffer[RX_BUF_SIZE];
  uint8_t tx_buffer[TX_BUF_SIZE];
} SPI_STATE;

//static SPI_STATE spis[NUM_SPI_MODULES];

typedef struct {
  unsigned int spixstat;
  unsigned int spixcon1;
  unsigned int spixcon2;
  unsigned int reserved;
  unsigned int spixbuf;
} SPIREG;

void SPIInit() {}
void SPIConfigMaster(int spi_num, int scale, int div, int smp_end, int clk_edge, int clk_pol) {}
void SPITasks() {}
void SPITransmit(int spi_num, int dest, const void* data, int data_size, int total_size, int trim_rx) {}

#if 0
#define _SPIREG_REF_COMMA(num, dummy) (volatile SPIREG*) &SPI##num##STAT,

volatile SPIREG* spi_reg[NUM_SPI_MODULES] = {
  REPEAT_1B(_SPIREG_REF_COMMA, NUM_SPI_MODULES, 0)
};

static void SPIConfigMasterInternal(int spi_num, int scale, int div,
                                    int smp_end, int clk_edge,
                                    int clk_pol, int external);

void SPIInit() {
  for (int i = 0; i < NUM_SPI_MODULES; ++i) {
    SPIConfigMasterInternal(i, 0, 0, 0, 0, 0, 0);
    AssignSPIxIP(i, INT_PRIORITY);  // int. priority INT_PRIORITY
  }
}

static inline void SPISendStatus(int spi_num, int enabled) {
  OUTGOING_MESSAGE msg;
  msg.type = SPI_STATUS;
  msg.args.spi_status.spi_num = spi_num;
  msg.args.spi_status.enabled = enabled;
  AppProtocolSendMessage(&msg);
}

static void SPIConfigMasterInternal(int spi_num, int scale, int div,
                                    int smp_end, int clk_edge,
                                    int clk_pol, int external) {
  volatile SPIREG* regs = spi_reg[spi_num];
  SPI_STATE* spi = &spis[spi_num];
  if (external) {
    log("SPIConfigMaster(%d, %d, %d, %d, %d, %d)", spi_num, scale, div, smp_end, clk_edge, clk_pol);
  }
  AssignSPIxIE(spi_num, 0);  // disable int.
  regs->spixstat = 0x0000;  // disable SPI
  // clear SW buffers
  ByteQueueInit(&spi->rx_queue, spi->rx_buffer, RX_BUF_SIZE);
  ByteQueueInit(&spi->tx_queue, spi->tx_buffer, TX_BUF_SIZE);
  spi->num_tx_since_last_report = 0;
  spi->num_messages_rx_queue = 0;
  spi->packet_state = PACKET_STATE_IDLE;
  if (scale || div) {
    if (external) {
      SPISendStatus(spi_num, 1);
    }
    spi->num_tx_since_last_report = TX_BUF_SIZE;
    regs->spixcon1 = (smp_end << 9)
                     | (clk_edge << 8)
                     | (clk_pol << 6)
                     | (1 << 5)  // master
                     | ((7 - div) << 2)
                     | ((3 - scale));
    regs->spixcon2 = 0x0001; // enhanced buffer mode
    regs->spixstat = (1 << 15) // enable
                     | (1 << 2);  // int. when RX FIFO is non-empty
    // Set int. flag, so int. will occur as soon as data is written
    AssignSPIxIF(spi_num, 1);
  } else {
    if (external) {
      SPISendStatus(spi_num, 0);
    }
  }
}

void SPIConfigMaster(int spi_num, int scale, int div, int smp_end, int clk_edge, int clk_pol) {
  SPIConfigMasterInternal(spi_num, scale, div, smp_end, clk_edge, clk_pol, 1);
}

static void SPIReportTxStatus(int spi_num) {
  int report;
  SPI_STATE *spi = &spis[spi_num];
  PRIORITY(INT_PRIORITY) {
    report = spi->num_tx_since_last_report;
    spi->num_tx_since_last_report = 0;
  }
  OUTGOING_MESSAGE msg;
  msg.type = SPI_REPORT_TX_STATUS;
  msg.args.spi_report_tx_status.spi_num = spi_num;
  msg.args.spi_report_tx_status.bytes_to_add = report;
  AppProtocolSendMessage(&msg);
}

void SPITasks() {
  for (int i = 0; i < NUM_SPI_MODULES; ++i) {
    int size1, size2, size;
    const uint8_t *data1, *data2;
    SPI_STATE *spi = &spis[i];
    ByteQueue *q = &spi->rx_queue;
    while (spi->num_messages_rx_queue) {
      OUTGOING_MESSAGE msg;
      msg.type = SPI_DATA;
      msg.args.spi_data.spi_num = i;
      msg.args.spi_data.ss_pin = ByteQueuePopByte(q);
      msg.args.spi_data.size = ByteQueuePopByte(q) - 1;
      ByteQueuePeekMax(q, msg.args.spi_data.size + 1, &data1, &size1, &data2, &size2);
      size = size1 + size2;
      assert(size == msg.args.spi_data.size + 1);
      log("SPI %d received %d bytes", i, size);
      AppProtocolSendMessageWithVarArgSplit(&msg, data1, size1, data2, size2);
      ByteQueuePop(q, size);
      atomic16_add(&spi->num_messages_rx_queue, -1);
    }
    if (spi->num_tx_since_last_report > TX_BUF_SIZE / 2) {
      SPIReportTxStatus(i);
    }
  }
}

static void SPIInterrupt(int spi_num) {
  volatile SPIREG* reg = spi_reg[spi_num];
  SPI_STATE* spi = &spis[spi_num];
  ByteQueue *tx_queue = &spi->tx_queue;
  ByteQueue *rx_queue = &spi->rx_queue;
  int bytes_to_write;

  // packet initialiation if needed
  if (spi->packet_state == PACKET_STATE_IDLE) {
    assert(ByteQueueSize(tx_queue) >= 4);
    // can't have incoming data on idle state. if we do - it's a bug
    assert(reg->spixstat & (1 << 5));

    // We haven't started writing yet. RX FIFO must be empty and will not fill
    // up until we write. Safe to clear.
    AssignSPIxIF(spi_num, 0);

    spi->cur_msg_dest = ByteQueuePopByte(tx_queue);
    spi->cur_msg_total_tx = ByteQueuePopByte(tx_queue);
    spi->cur_msg_total_rx = spi->cur_msg_total_tx;
    spi->cur_msg_data_tx = ByteQueuePopByte(tx_queue);
    spi->cur_msg_trim_rx = ByteQueuePopByte(tx_queue);
    spi->can_send = 8;
    atomic16_add(&spi->num_tx_since_last_report, 4);

    // write packet header to rx_queue, if non-empty
    spi->cur_msg_rx_size = spi->cur_msg_total_rx - spi->cur_msg_trim_rx;
    if (spi->cur_msg_rx_size > 0) {
      ByteQueuePush(rx_queue, spi->cur_msg_dest);
      ByteQueuePush(rx_queue, spi->cur_msg_rx_size);
    }

    PinSetLat(spi->cur_msg_dest, 0);  // activate SS
    spi->packet_state = PACKET_STATE_IN_PROGRESS;
  } else {
    // read as much incoming data as possible into rx_queue
    while (!(reg->spixstat & (1 << 5))) {
      uint8_t rx_byte = reg->spixbuf;
      if (spi->cur_msg_trim_rx) {
        --spi->cur_msg_trim_rx;
      } else {
        ByteQueuePush(rx_queue, rx_byte);
      }
      --spi->cur_msg_total_rx;
      ++spi->can_send;  // for every byte read we can write one

      // The interrupt triggers when there is data to be read in the RX FIFO.
      // Since we are just about to check again, we can safely clear now.
      AssignSPIxIF(spi_num, 0);
    }
    if (!spi->cur_msg_total_rx) {
      spi->packet_state = PACKET_STATE_DONE;
    }
  }

  // send as much data as possible
  if (spi->packet_state == PACKET_STATE_IN_PROGRESS) {
    bytes_to_write = spi->cur_msg_total_tx;
    if (bytes_to_write > spi->can_send) {
      bytes_to_write = spi->can_send;
    }
    while (bytes_to_write-- > 0) {
      uint8_t tx_byte = 0xFF;
      if (spi->cur_msg_data_tx) {
        tx_byte = ByteQueuePopByte(tx_queue);
        --spi->cur_msg_data_tx;
        atomic16_add(&spi->num_tx_since_last_report, 1);
      }
      reg->spixbuf = tx_byte;
      --spi->cur_msg_total_tx;
      --spi->can_send;
    }
  }

  // finalize packet if needed
  if (spi->packet_state == PACKET_STATE_DONE) {
    PinSetLat(spi->cur_msg_dest, 1);  // deactivate SS
    if (spi->cur_msg_rx_size) {
      atomic16_add(&spi->num_messages_rx_queue, 1);
    }
    spi->packet_state = PACKET_STATE_IDLE;
    AssignSPIxIE(spi_num, ByteQueueSize(tx_queue) > 0);
    AssignSPIxIF(spi_num, 1);
  }
}

void SPITransmit(int spi_num, int dest, const void* data, int data_size,
                 int total_size, int trim_rx) {
  ByteQueue *q = &spis[spi_num].tx_queue;
  PRIORITY(INT_PRIORITY) {
    ByteQueuePush(q, dest);
    ByteQueuePush(q, total_size);
    ByteQueuePush(q, data_size);
    ByteQueuePush(q, trim_rx);
    ByteQueuePushBuffer(q, data, data_size);
    AssignSPIxIE(spi_num, 1);  // enable int.
  }
}

// #define DEFINE_INTERRUPT_HANDLERS(spi_num)                              \
//   void __attribute__((__interrupt__, auto_psv)) _SPI##spi_num##Interrupt() { \
//     SPIInterrupt(spi_num - 1);                                          \
//   }

#if NUM_SPI_MODULES > 3
#error Currently only devices with 3 or less SPIs are supported. Please fix below.
#endif

#if NUM_SPI_MODULES >= 1
DEFINE_INTERRUPT_HANDLERS(1)
#endif

#if NUM_SPI_MODULES >= 2
  DEFINE_INTERRUPT_HANDLERS(2)
#endif

#if NUM_SPI_MODULES >= 3
  DEFINE_INTERRUPT_HANDLERS(3)
#endif

#endif
