#include <Adafruit_BMP085.h>
#include <BLEMIDI_Transport.h>
#include <Wire.h>
#include <hardware/BLEMIDI_ESP32_NimBLE.h>

BLEMIDI_CREATE_INSTANCE("E-TinWhistle", MIDI)

Adafruit_BMP085 bmp;
bool isConnected = false;

int cled = 19;

//電源供給
int bmpac = 23;
int tcac = 12;

//センサ入力 上から
int tc[] = {14, 27, 26, 25, 33, 32};

//ブレスセンサの感度(hPa差)
double pPress = 1000;
double diffPress = 4;
double onPress = 1;

// NoteOn記録
int send = 0;
int prenote = 0;

// Notes
// D5-C#7 2オクターブ
int midiCh = 1;
int velo = 100;

typedef struct {
    const char *name;
    int num;
} notesDec;

const notesDec Notes[]{
    {"D5", 74},
    {"E5", 76},
    {"F#5", 78},
    {"G5", 79},
    {"A5", 81},
    {"B5", 83},
    {"C5", 84},
    {"C#5", 85},
    {"D6", 86},
    {"E6", 88},
    {"F#6", 90},
    {"G6", 91},
    {"A6", 93},
    {"B6", 95},
    {"C7", 96},
    {"C#7", 97},
};

//つまりNotes[n]
// 0 1 2  3 4 5 6 7  8 9 10 11 12 13 14 15
// D E F# G A B C C# D E F# G  A  B  C  C#

/*
  それぞれタッチセンサに対応
  bitで管理する
  (1 << 0); // 0000 0001
  (1 << 1); // 0000 0010
  (1 << 2); // 0000 0100
  (1 << 3); // 0000 1000
  (1 << 4); // 0001 0000
  (1 << 5); // 0010 0000
  (1 << 6); // 0100 0000
*/
const unsigned int BIT_FLAG[] = {
    (1 << 0), (1 << 1), (1 << 2), (1 << 3), (1 << 4), (1 << 5)};

void setup() {

    Serial.begin(9600);
    // BMP180
    // 23 3.3V供給
    // 22 I2C_SCL
    // 21 I2C_SDA
    bmp.begin();
    pinMode(bmpac, OUTPUT);

    // BLE接続確認
    pinMode(cled, OUTPUT);
    BLEMIDI.setHandleConnected(OnConnected);
    BLEMIDI.setHandleDisconnected(OnDisconnected);

    //タッチセンサ
    // 27 3.3V供給
    // 26 センサ1
    pinMode(tcac, OUTPUT);
    for (int i = 0; i < sizeof(tc) / sizeof(tc[0]); i++) {
        pinMode(tc[i], INPUT);
    }
    digitalWrite(bmpac, HIGH); // BMP180の電力供給
    digitalWrite(tcac, HIGH);  //タッチセンサの電力供給

    //基準大気圧測定
    double p = 0;
    for (int i = 0; i < 5; i++) {
        p += double(bmp.readPressure()) / 100;
    }
    pPress = p / 5;

    MIDI.begin();
}

void loop() {

    double press = double(bmp.readPressure()) / 100; //気圧(hPa)

    /*
        //接続と気圧(hpa)の表示
        String bar = "";
        for (int i = 0; i < ((press - 1000) * 4); i++) {
            bar += "-";
        }
        Serial.println(String(press) + ":" + isConnected + bar);
    */

    double dp = press - pPress;
    int note = getnote(press);

    if (dp < onPress) {
        // onに達しなければ音を止めてreturn
        MIDI.sendNoteOff(Notes[prenote].num, 0, midiCh);
        send = 0;
        prenote = -1;
        // Serial.println("C");
        return;
    }
    if (note == prenote) {
        //音が変わらなければreturn
        // Serial.println("B");
        return;
    } else if (note == -1) {
        MIDI.sendNoteOff(Notes[prenote].num, 0, midiCh);
        send = 0;
        prenote = -1;
        return;
    }
    MIDI.sendNoteOff(Notes[prenote].num, 0, midiCh);
    MIDI.sendNoteOn(Notes[note].num, velo, midiCh);
    prenote = note; // noteを記録
    // Serial.println("A");
}

void OnConnected() {
    isConnected = true;
}

void OnDisconnected() {
    isConnected = false;
}

int getnote(double press) {
    int fingnote = getfinger();
    int note = fingnote;
    double dp = press - pPress;
    if (dp > diffPress) {
        note += 8; //オクターブ上げる
    }
    return note;
}

int getfinger() {
    //運指を判定
    unsigned int bit = 0;
    for (int i = 0; i < 6; i++) {
        if (digitalRead(tc[i]) == HIGH) {
            bit |= BIT_FLAG[i];
        }
    }

    int noteNum;

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
        noteNum = -1;
        break;
    }
    return noteNum;
}