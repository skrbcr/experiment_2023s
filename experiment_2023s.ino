#include <LiquidCrystal.h>
#include <stdio.h>
#include <math.h>

constexpr int RS = 12, EN = 11, D4 = 10, D5 = 9, D6 = 8, D7 = 7;
constexpr int SIG_IN = A0, SIG_REF = A1;
constexpr int HZ = A2;
constexpr int FW58 = 0, FW61 = 1;
constexpr double ANALOG_MAX = 1023.;
constexpr double SIG_MAX_VOL = 50.;
LiquidCrystal lcd = LiquidCrystal(RS, EN, D4, D5, D6, D7);
constexpr int NBUF = 17;
char lpszDisp1[NBUF];
char lpszDisp2[NBUF];

// 信号電圧をワットに変換
// nDet: 検波器番号 (FW58 or FW61)
// vol: 信号電圧 [mV]
// hertz: 周波数 [MHz]
double vol_to_watt(int nDet, double vol, double hertz);

void setup() {
    Serial.begin(9600);
    // pinMode(SIG_IN, INPUT);
    // pinMode(SIG_REF, INPUT);
    // pinMode(HZ, INPUT);
    lcd.begin(16, 2);
}

void loop() {
    double dVol_in = analogRead(SIG_IN) * SIG_MAX_VOL / ANALOG_MAX;
    double dVol_ref = analogRead(SIG_REF) * SIG_MAX_VOL / ANALOG_MAX;
    double dHertz = analogRead(HZ) * 20. / ANALOG_MAX + 10;
    // ↓↓↓テスト用
    dVol_in = 1.0;
    dVol_ref = 1.0;
    // ↑↑↑テスト用
    double dWatt_in = vol_to_watt(FW61, dVol_in, dHertz);
    double dWatt_ref = vol_to_watt(FW58, dVol_ref, dHertz);
    // ↓↓↓テスト用
    // dWatt_in = 21.3882;
    // dWatt_ref = 0.932;
    // dHertz = 13.56286;
    // ↑↑↑テスト用
    lcd.clear();
    lcd.setCursor(0, 0);
    char lpszTmp1[NBUF] = { '\0' };
    char lpszTmp2[NBUF] = { '\0' };
    dtostrf(dHertz, 4, 1, lpszTmp1);
    sprintf(lpszDisp1, "f:%sMHz", lpszTmp1);
    dtostrf(dWatt_in, 4, 1, lpszTmp1);
    dtostrf(dWatt_ref, 4, 1, lpszTmp2);
    sprintf(lpszDisp2, "In:%sW R:%sW", lpszTmp1, lpszTmp2);
    lcd.print(lpszDisp1);
    lcd.setCursor(0, 1);
    lcd.print(lpszDisp2);
    delay(1000);
}

// 方向性結合器の出入力比(入出力比の逆)
double vol_to_watt(int nDet, double vol, double hertz) {
  if (nDet == FW58) {
    double B = (-0.000576326358200036*(hertz*hertz*hertz) + 0.0580062059447736*(hertz*hertz) - 2.52085896819404*hertz + 92.4129697039502)/10 - 3; // 方結出力側
    return ((2.0424388986619e-05*(vol*vol) + 0.00202430415694329*vol + 7.02827150367452e-05)*pow(10,B));
  }
  else if (nDet == FW61) {
    double B = (-0.00119220702668374*(hertz*hertz*hertz) + 0.0966232853857063*(hertz*hertz) - 3.28289959924571*hertz + 96.7972152618555)/10 - 3; // 方結入力側
    return ((1.72277251555492e-05*(vol*vol) + 0.00171336281683369*vol + 7.12796451662058e-05)*pow(10,B));
  }
}
