#include <Wire.h>
void recibirFloats(float *values);
float data[2];
void setup() {
  Wire.begin();  // Inicia la comunicación I2C como maestro
  Serial.begin(115200);
  delay(20000);
}
void loop() {
  recibirFloats(data);
  for(int i=0;i<2;i++){
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
  Wire.requestFrom(8, 8); // Solicita 12 bytes al esclavo en la dirección 8
  int i = 0;
  while (Wire.available()) {
    for (int j = 0; j < 4; j++) {
      u.bytes[j] = Wire.read();
    }
    values[i] = u.valorFloat;
    i++;
  }
}
