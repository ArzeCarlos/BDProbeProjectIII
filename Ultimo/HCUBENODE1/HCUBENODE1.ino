#include <Wire.h>
#include "Adafruit_CCS811.h"
#include "LoRaWan_APP.h"
#include "Arduino.h"
#ifndef LoraWan_RGB
#define LoraWan_RGB 0
#endif

#define RF_FREQUENCY                                915000000 // Hz

#define TX_OUTPUT_POWER                             14        // dBm

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false


#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 30 // Define the payload size here
// LoRa parameters
static RadioEvents_t RadioEvents;
#define NODEID 246
#define GATEWAYID 254
struct NodePacket {
    uint8_t dest;
    uint8_t emitter;
    float value1;
    float value2;
    float value3;
    float value4;
    float value5;
    float value6;
};
int txNumber;
bool lora_idle=true;
float data[4];
void recibirFloats(float *values);
Adafruit_CCS811 ccs;
void setup() {
  Wire.begin();  // Inicia la comunicación I2C como maestro
  Serial.begin(115200);
  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }
  while(!ccs.available());
  float temp = ccs.calculateTemperature();
  ccs.setTempOffset(temp - 25.0);
  txNumber=0;
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  Radio.Init( &RadioEvents );
  Radio.SetChannel( RF_FREQUENCY );
  Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000 ); 
}
void loop() {
   if(lora_idle == true)
  {
    
    delay(10000);
     NodePacket p;
    p.dest = GATEWAYID;
    p.emitter = NODEID;
    
  if(ccs.available()){
    float temp = ccs.calculateTemperature();
    if(!ccs.readData()){
      Serial.print("eCO2: ");
      float eCO2 = ccs.geteCO2();
      Serial.print(eCO2);
      Serial.print(" ppm, TVOC: ");      
      float TVOC = ccs.getTVOC();
      Serial.print(TVOC);
      Serial.print(" ppb   Temp:");
      Serial.println(temp);
      p.value1= eCO2;
      p.value2= TVOC;
    }
  }
  delay(500);
  recibirFloats(data);
  for(int i=0;i<4;i++){
    Serial.print("Valor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(data[i], 2);  // Imprime el valor recibido con dos decimales
  }
  p.value3= data[0];
  p.value4= data[1];
  p.value5= data[2];
  p.value6= data[3];
  txNumber += 1;
    byte mem[sizeof(p)];
    memcpy(mem, &p, sizeof(p));
    turnOnRGB(COLOR_SEND,0); //change rgb color
    Radio.Send( mem,sizeof(p)); //send the package out 
    lora_idle = false;
  }
  
}
void recibirFloats(float *values) {
  union {
    float valorFloat;
    byte bytes[4];
  } u;
  Wire.requestFrom(8, 16); // Solicita 12 bytes al esclavo en la dirección 8
  int i = 0;
  while (Wire.available()) {
    for (int j = 0; j < 4; j++) {
      u.bytes[j] = Wire.read();
    }
    values[i] = u.valorFloat;
    i++;
  }
}
void OnTxDone( void )
{
  turnOffRGB();
  Serial.println("TX done......");
  lora_idle = true;
}

void OnTxTimeout( void )
{
  turnOffRGB();
  Radio.Sleep( );
  Serial.println("TX Timeout......");
  lora_idle = true;
}
