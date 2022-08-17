#include <Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

void setup() {
  bmp.begin();
  Serial.begin(9600);
    pinMode(23,OUTPUT);

}

void loop() {
  digitalWrite(23,HIGH);
  Serial.print("温度＝");
  Serial.print(bmp.readTemperature()); //気温の表示
  Serial.println(" *C");

  Serial.print("気圧＝");
  Serial.print(double(bmp.readPressure()) / 100); //気圧(hpa)の表示
  Serial.println("hPa");
  
  Serial.println();
  delay(2000);
}
