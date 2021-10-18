#include <Adafruit_BMP085.h>
//#include <MIDI.h>
/*
  (1 << 0); // 0000 0000 0000 0001
  (1 << 1); // 0000 0000 0000 0010
  (1 << 2); // 0000 0000 0000 0100
  (1 << 3); // 0000 0000 0000 1000
  (1 << 4); // 0000 0000 0001 0000
  (1 << 5); // 0000 0000 0010 0000
  (1 << 6); // 0000 0000 0100 0000
*/
//穴の識別 上から昇順
const unsigned int BIT_FLAG[] = {
    (1 << 0), (1 << 1), (1 << 2), (1 << 3), (1 << 4), (1 << 5)};

// MIDI_CREATE_DEFAULT_INSTANCE();

Adafruit_BMP085 bmp;
// Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
// Connect GND to Ground
// Connect SCL to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats
// Analog 5 Connect SDA to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc
// thats Analog 4 EOC is not used, it signifies an end of conversion XCLR is a
// reset pin, also not used here

const int freq[] = {294, 330, 370, 392, 440, 494, 523, 554, 587, 659, 698, 740,
    784, 880, 988, 1047, 1109};
// const int notes[] = {
// 74, 76, 78, 79, 81, 83, 84, 85, 86, 88, 90, 91, 93, 95, 96, 97
//};
// 0 1 2  3 4 5 6 7  8 9 0  1 2 3 4 5
// D E F# G A B C C# d e f# g a b c c#

// 穴の識別 上から昇順
const int sIN[] = {2, 4, 6, 8, 10, 12};
const int sOUT[] = {3, 5, 7, 9, 11, 15};

const int LED = 13;
const int SP = 14;

const double threshold[] = {9, 9, 8, 8, 8, 8};

int32_t pressL, pressH;

int prevNote = 20;

void calibrate() {
    // int32_t pL, pH;
    // 1オクターブ目
    digitalWrite(LED, HIGH);
    tone(SP, 294, 500);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
    digitalWrite(LED, HIGH);
    tone(SP, 294, 500);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
    digitalWrite(LED, HIGH);
    tone(SP, 294, 1500);
    delay(1000);
    pressL = bmp.readPressure();
    // Serial.println(pressL);
    digitalWrite(LED, LOW);
    delay(1000);
    // 2オクターブ目
    digitalWrite(LED, HIGH);
    tone(SP, 587, 500);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
    digitalWrite(LED, HIGH);
    tone(SP, 587, 500);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
    digitalWrite(LED, HIGH);
    tone(SP, 587, 1500);
    delay(1000);
    pressH = bmp.readPressure();
    // Serial.println(pressH);
    digitalWrite(LED, LOW);
}

int checkTimeLag(int a, int b) {
    int counter = 0;
    digitalWrite(a, HIGH);
    while (digitalRead(b) != HIGH)
        counter++;
    digitalWrite(a, LOW);
    delay(1);
    return counter;
}

int readBtn() {
    unsigned int bit = 0;
    for (int i = 0; i < 6; i++) {
        int counter = checkTimeLag(sOUT[i], sIN[i]);
        if (counter > threshold[i]) {
            bit |= BIT_FLAG[i];
        }
    }
    return bit;
}

void sendnote(unsigned int bit) {
    int noteNum; // 20で無音
    // 0 1 2  3 4 5 6 7  8 9 0  1 2 3 4 5
    // D E F# G A B C C# d e f# g a b c c#

    switch (bit) {
    case 63:
        noteNum = 0;
        break;
    case 31:
        noteNum = 1;
        break;
    case 15:
        noteNum = 2;
        break;
    case 7:
        noteNum = 3;
        break;
    case 3:
        noteNum = 4;
        break;
    case 1:
        noteNum = 5;
        break;
    case 6:
        noteNum = 6;
        break;
    case 0:
        noteNum = 7;
        break;
    case 62:
        noteNum = 0;
        break;
    default:
        noteNum = 20;
        break;
    }

    int32_t pressure = bmp.readPressure();
    Serial.println(pressure);
    if (pressure > pressH) {
        noteNum += 8;
    } else if (pressure < pressL) {
        noteNum = 20;
    }

    if (prevNote <= 15) {
        // MIDI.sendNoteOff(notes[prevNote], 0, 1);
    }
    if (noteNum <= 15) {
        // MIDI.sendNoteOn(notes[noteNum], 127, 1);
        tone(SP, freq[noteNum]);
        digitalWrite(LED, HIGH);
    } else {
        noTone(SP);
    }
    Serial.println(noteNum);
    prevNote = noteNum;
}

void setup() {
    for (int i = 0; i < 6; i++) {
        pinMode(sIN[i], INPUT);
        pinMode(sOUT[i], OUTPUT);
    }
    for (int i = 0; i < 6; i++) {
        digitalWrite(sOUT[i], HIGH);
    }
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
    // MIDI
    // MIDI.begin(1);

    // BMP180
    Serial.begin(9600);
    if (!bmp.begin()) {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
        while (1) {
        }
    }

    calibrate();
}

void loop() {
    unsigned int checkbit = readBtn();
    sendnote(checkbit);
    delay(10);
}
