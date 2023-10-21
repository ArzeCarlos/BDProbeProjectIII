#include "TinyWireS.h"                         
#include "mics.hpp"
struct NodePacket {
    float  co, no2;
};
NodePacket p;
volatile bool calibrated = false;
void calibrateSensors();
void enviarFloat(float valor);
void setup()
{ 
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  if(!calibrated){
        calibrateSensors();
        calibrated = true;
  }
  digitalWrite(1, HIGH);
  delay(500);
  digitalWrite(1, LOW);
  delay(500);
 TinyWireS.begin(8);  // Dirección del esclavo
  TinyWireS.onRequest(requestEvent);
}

void loop()
{
  readMICS(&p);
  delay(1000);
}
void calibrateSensors(){
  // Circular buffer for the measurements

    uint16_t bufferRED[MICS_CALIBRATION_SECONDS];
    uint16_t bufferOX[MICS_CALIBRATION_SECONDS];

    // Pointers for the next element in the buffer
    uint8_t  pntrRED = 0, pntrOX = 0;

    // Current floating sum in the buffer
    uint16_t  fltSumRED = 0, fltSumOX = 0;

    // Current measurements;
    uint16_t  curRED, curOX;


    bool REDStable = false;
    bool OXStable = false;
    bool micsOK = false;
     // Initialize buffer
    for (int i = 0; i < MICS_CALIBRATION_SECONDS; ++i) {

        bufferRED[i] = 0;
        bufferOX[i] = 0;
    }
    do {
        // Wait a second
        delay(1000);
        // Read new resistance for NH3
        unsigned long rs = 0;
     


        // Read new resistance for CO
        rs = 0;
        delay(50);
        for (int i = 0; i < 3; i++) {
            delay(1);
            rs += analogRead(COPIN);
        }
        curRED = rs/3;

        // Read new resistance for NO2
        rs = 0;
        delay(50);
        for (int i = 0; i < 3; i++) {
            delay(1);
            rs += analogRead(OXPIN);
        }
        curOX = rs/3;

        // Update floating sum by subtracting value about to be overwritten and adding the new value.
      
        fltSumRED = fltSumRED + curRED - bufferRED[pntrRED];
        fltSumOX = fltSumOX + curOX - bufferOX[pntrOX];

        // Store new measurement in buffer
       
        bufferRED[pntrRED] = curRED;
        bufferOX[pntrOX] = curOX;

        // Determine new state of flags
       
        REDStable = abs(fltSumRED / MICS_CALIBRATION_SECONDS - curRED) < MICS_CALIBRATION_DELTA;
        OXStable = abs(fltSumOX / MICS_CALIBRATION_SECONDS - curOX) < MICS_CALIBRATION_DELTA;
        micsOK = REDStable && OXStable;
         // Advance buffer pointer
 
        pntrRED = (pntrRED + 1) % MICS_CALIBRATION_SECONDS;
        pntrOX = (pntrOX + 1) % MICS_CALIBRATION_SECONDS;
    }
    while (!micsOK);


    REDbaseR = fltSumRED / MICS_CALIBRATION_SECONDS;
    OXbaseR = fltSumOX / MICS_CALIBRATION_SECONDS;
}

void requestEvent()
{  
      enviarFloat(p.co);
      enviarFloat(p.no2);

}
void readMICS(NodePacket* p){
    float co = measureMICS(CO);
    float no2 = measureMICS(NO2);
    p->co = co;
    p->no2 = no2;
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
