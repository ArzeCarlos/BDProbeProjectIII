#include <Wire.h>
#include "Adafruit_CCS811.h"
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
}
void loop() {
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
  delay(3000);
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
