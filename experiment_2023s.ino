#include <LiquidCrystal.h>
#include <stdio.h>
#include <math.h>

// 使用状況に合わせて変更
constexpr int RS = 12, EN = 11, D4 = 10, D5 = 9, D6 = 8, D7 = 7;    // LCD に接続するピン番号
constexpr int SIG_IN = A0, SIG_REF = A1, HZ = A2;   // 方向性結合器を入力するピン
constexpr double RATIO_AMP = 100.;  // オペアンプの電圧増幅率
enum {  // 方向性結合器の番号
    FW58,
    FW61,
}
// 定数
constexpr double ANALOG_MAX = 1023.;    // アナログ入力のサンプリングの最大値
constexpr int NBUF = 17;    // LCD の1列文字数
// LCD 変数
LiquidCrystal lcd = LiquidCrystal(RS, EN, D4, D5, D6, D7);
char lpszDisp1[NBUF] = { '\0' };    // LCD 1行目表示文字列
char lpszDisp2[NBUF] = { '\0' };    // LCD 2行目表示文字列

// 信号電圧をワットに変換
// nDet: 検波器番号 (FW58 or FW61)
// vol: 信号電圧 [mV]
// hertz: 周波数 [MHz]
double vol_to_watt(int nDet, double vol, double hertz);

void setup() {
    Serial.begin(9600);
    lcd.begin(16, 2);
}

void loop() {
    double dVol_in = analogRead(SIG_IN) * SIG_MAX_VOL / ANALOG_MAX;
    double dVol_ref = analogRead(SIG_REF) * SIG_MAX_VOL / ANALOG_MAX;
    double dHertz = analogRead(HZ) * 20. / ANALOG_MAX + 10;
    double dWatt_in = vol_to_watt(FW61, dVol_in, dHertz);
    double dWatt_ref = vol_to_watt(FW58, dVol_ref, dHertz);
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
    double B, res;
    switch (nDet) {
    case FW58:
        B = (-0.000576326358200036 * pow(hertz, 3.) + 0.0580062059447736 * pow(hertz, 2.) - 2.52085896819404 * hertz + 92.4129697039502) / 10. - 3.; // 方結出力側
        res = ((2.0424388986619e-5 * pow(vol, 2.) + 0.00202430415694329 * vol + 7.02827150367452e-5) * pow(10., B));
        break;
    case FW61:
        B = (-0.00119220702668374 * pow(hertz, 3.) + 0.0966232853857063 * pow(hertz, 2.) - 3.28289959924571 * hertz + 96.7972152618555) / 10. - 3.; // 方結入力側
        res = ((1.72277251555492e-5 * pow(vol, 2.) + 0.00171336281683369 * vol + 7.12796451662058e-5) * pow(10., B));
        break;
    default:
        res = 0.;
        break;
    }
    return res;
}
