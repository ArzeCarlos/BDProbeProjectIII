#include <TinyWireS.h>
int UVOUT = A2; //Output from the sensor
int REF_3V3 = A3; //3.3V power on the Arduino board
int averageAnalogRead(int pinToRead);
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);
void enviarFloat(float valor);
float outputVoltage,uvIntensity,uvLevel,refLevel;
void setup() {
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);
  TinyWireS.begin(8);  // Dirección del esclavo
  TinyWireS.onRequest(requestEvent);
}
void loop() {
  // Read data GYML8511
  uvLevel = averageAnalogRead(UVOUT);
  refLevel = averageAnalogRead(REF_3V3);
  //Use the 3.3V power pin as a reference to get a very accurate output value from sensor
  outputVoltage = 3.3 / refLevel * uvLevel;
  uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0); //Convert the voltage to a UV intensity level
  delay(1000);
}
void requestEvent() {
  enviarFloat(uvLevel);
  enviarFloat(refLevel);
  enviarFloat(outputVoltage);
  enviarFloat(uvIntensity);
}
//Takes an average of readings on a given pin
//Returns the average
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return(runningValue);  
}

//The Arduino Map function but for floats
//From: http://forum.arduino.cc/index.php?topic=3922.0
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void enviarFloat(float valor) {
  union {
    float valorFloat;
    byte bytes[4];
  } u;
  u.valorFloat = valor;
  for (int i = 0; i < 4; i++) {
    TinyWireS.write(u.bytes[i]);
  }
}
