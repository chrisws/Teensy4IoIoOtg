#include "app_layer_v1/adc.h"
#include "app_layer_v1/connection.h"
#include "app_layer_v1/digital.h"
#include "app_layer_v1/features.h"
#include "app_layer_v1/field_accessors.h"
#include "app_layer_v1/i2c.h"
#include "app_layer_v1/icsp.h"
#include "app_layer_v1/incap.h"
#include "app_layer_v1/logging.h"
#include "app_layer_v1/pins.h"
#include "app_layer_v1/platform.h"
#include "app_layer_v1/pp_util.h"
#include "app_layer_v1/protocol_defs.h"
#include "app_layer_v1/protocol.h"
#include "app_layer_v1/pwm.h"
#include "app_layer_v1/queue.h"
#include "app_layer_v1/sequence.h"
#include "app_layer_v1/sequencer.h"
#include "app_layer_v1/sequencer_protocol.h"
#include "app_layer_v1/spi.h"
#include "app_layer_v1/sync.h"
#include "app_layer_v1/timers.h"
#include "app_layer_v1/uart.h"

void hex_dump(const uint8_t *data, int size) {
  for (int i = 0; i < size; i++) {
    // Print the offset
    if (i % 16 == 0) {
      if (i != 0) {
        // Print a new line after every 16 bytes
        printf("\n");
      }
      printf("%08x  ", i); // Print the offset
    }
    // Print the byte in hexadecimal format
    printf("%02x ", data[i]);
  }
  // New line at the end if there's leftover data
  printf("\n");
}

void ADCSetCapSense(int, int){}
void ADCSetScan(int, int){}
void CheckInterface(unsigned char*){}
bool ConnectionCanSend(int ch) {
  fprintf(stderr, "ConnectionCanSend %d\n", ch);
  return true;
}
void ConnectionCloseChannel(int){}
int ConnectionGetMaxPacket(int) {
  return 50;
}
void ConnectionSend(CHANNEL_HANDLE ch, const uint8_t *data, int size) {
  fprintf(stderr, "ConnectionSend %d size: %d\n", ch, size);
  hex_dump(data, size);
}
void HardReset(){}
void I2CConfigMaster(int, int, int){}
void I2CTasks(){}
void I2CWriteRead(int, unsigned int, void const*, int, int){}
void ICSPConfigure(int){}
void ICSPEnter(){}
void ICSPExit(){}
void ICSPRegout(){}
void ICSPSix(unsigned int){}
void ICSPTasks(){}
void InCapConfig(int, int, int, int){}
void SPIConfigMaster(int, int, int, int, int, int){}
void SPITasks(){}
void SPITransmit(int, int, void const*, int, int, int){}
bool SequencerClose(){return true;}
bool SequencerCommand(SEQ_CMD, unsigned char const*){return true;}
size_t SequencerExpectedCueSize(){return 0;}
bool SequencerOpen(unsigned char const*, unsigned long){return true;}
bool SequencerPush(unsigned char const*, unsigned short){return true;}
void SequencerTasks(){}
void SetChangeNotify(int, int){}
void SetDigitalOutLevel(int, int){}
void SetPinAnalogIn(int){}
void SetPinCapSense(int){}
void SetPinDigitalIn(int, int){}
void SetPinDigitalOut(int, int, int){}
void SetPinInCap(int, int, int){}
void SetPinPwm(int, int, int){}
void SetPinSpi(int, int, int, int){}
void SetPinUart(int, int, int, int){}
void SetPwmDutyCycle(int, int, int){}
void SetPwmPeriod(int, int, int){}
void SoftReset(){}
void UARTConfig(int, int, int, int, int){}
void UARTTasks(){}
void UARTTransmit(int, void const*, int){}
