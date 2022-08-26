#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ESP32_NimBLE.h>

BLEMIDI_CREATE_INSTANCE("E-TinWhistle", MIDI)

Adafruit_BMP085 bmp;

void setup() {
  bmp.begin();
  Serial.begin(9600);
   pinMode(23,OUTPUT);
   MIDI.begin();
}

void loop() {
  
  digitalWrite(23,HIGH);
  //Serial.print("温度＝");
  //Serial.print(bmp.readTemperature()); //気温の表示
  //Serial.println(" *C");

  double press = double(bmp.readPressure()) / 100;
  //Serial.print(press); //気圧(hpa)の表示
  String bar="";
  for (int i = 0; i<((press-1000)*4);i++){
    bar+="-";
  }
  Serial.println(bar);
  //delay(100);
}
