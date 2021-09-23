#include <MIDI.h>
#include <Adafruit_BMP085.h>

MIDI_CREATE_DEFAULT_INSTANCE();
Adafruit_BMP085 bmp;

int notes[] = {74, 76,78,79,81,83,84,85,86,88,90,91,93,95,96,97}
// 0 1  2   3  4 5 6  7   8 9  0 1 2 3 4 5
// D E F# G A B C C# d e f# g a b c c#

void setup() {
 // MIDI
 MIDI.begin(1);

 // BMP180
 Serial.begin(9600);
  if (!bmp.begin()) {
 Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }
 
}

void loop() {
  for(int i = 0; i < 8; i++){
    //ノート出力
    MIDI.sendNoteOn(notes[i],127,1);  // ノートオン
    delay(1000);                // 1秒待機
    MIDI.sendNoteOff(notes[i],0,1);   // ノートオフ

    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
  }

  
}

void 
