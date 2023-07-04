#include <LiquidCrystal.h>

constexpr int RS = 12, EN = 11, D4 = 10, D5 = 9, D6 = 8, D7 = 7;
constexpr int SIG_IN = 1, SIG_REF = 2;
LiquidCrystal lcd = LiquidCrystal(RS, EN, D4, D5, D6, D7);
char lpszDisp1[17];
char lpszDisp2[17];

// 信号電圧をワットに変換
// vol: 信号電圧[mV]
// hertz: 周波数[MHz]
double vol_to_watt(double vol, double hertz);

void setup() {
    Serial.begin(9600);
    pinMode(SIGNAL, INPUT);
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("Hello World!");
}

void loop() {
    double dVol_in = analogRead(SIG_IN);
    double dVol_ref = analogRead(SIG_REF);
    double dWatt_in = vol_to_watt(dVol_in, 0.);
    double dWatt_ref = vol_to_watt(dVol_ref, 0.);
}

double vol_to_watt(double vol, double hertz) {
}
