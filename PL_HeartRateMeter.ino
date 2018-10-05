/* 心拍センサMAX30105 & OLEDディスプレイSSD1306 テストプログラム
 * 『Heart Rate Meter』 v0.4 【ESP-WROOM-32対応版】
 *  2018.10.05. (Fri)
 *  創造設計2018 1班エレキ担当 矢澤杏平🍵
 *  Thanks For SparkFun(Sensor) & AdaFruit(OLED) !!
 */

/* 【来歴】
 *  2018.06.12. v0.0 新規作成 矢澤
 *  2018.06.26  v0.1 【ESP-WROOM-32対応版】
 *                    表示の配置を変更。平均値を中心にナマの値・イマの値を表示するように更新
 *                    ＬＥＤの明るさを最大にした
 *  2018.07.04  v0.2  BLEなんとか対応版（無理やりなとこ多し・謎だらけ）
 *  2018.10.04  v0.3  ムダなところを削除！ファルを！分割
 *  2018.10.05  v0.4  マルチタスク化・セマフォを実装・プロトタイプ宣言を一元化(TeaWatch.h)
 */

/*　【スタック足りない問題について】7/3
 *  関数の呼び出し過ぎでスタックが足りないと起こられることがある。
 *  そんなときは hardware/espressif/esp32/cores/esp32/main.cpp の
 *  xTaskCreatePinnedToCore(loopTask, "loopTask", 8192, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
 *  8192のところがスタック容量。これをいじると増やせる。
 */

/* ■ ライブラリのインクルド */
#include <Wire.h> /* I2Cライブラリ */
#include "TeaWatch.h" /* まとめてプロトタイプ宣言 */

/* ■心拍測定用グローバル変数 */
const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
float beatsPerMinute;
int beatAvg;
long rowIrValue = 0;

/* ■ スイッチ用グローバル変数 */
byte swvalue;

/* ■ 心拍測定/スイッチ検出 タスク 
 *  測定後の心拍はグローバル変数『beatAvg』より取得可能
 */
void HEARTRATE_TSK(void *pvParameters){
  /* 心拍の測定 */
  rowIrValue = measureHeartRate(); 

  /* チャタリング除去(ピンの値を平均化する！) */
  swvalue << 1;
  swvalue |= getPushSWRawValue();
  
  delay(10);
}

/* ■ 初期化プログラム */
void setup() {
  initPin(); /* ピン入出力初期化 */
  powerON(); /* 電源オンする */

  /* 心拍測定タスク作成 */
  xTaskCreate(HEARTRATE_TSK,"HEARTRATE_TSK", 4096, NULL, 1, NULL);
  
  /* シリアルポートの設定(デバッグ用) */
  Serial.begin(115200);
  Serial.println("Heart Rate Meter v0.0");
  
  sensorSetup(); /* センサの初期化(ユーザ定義関数) */
  displaySetup(); /* ディスプレイ初期化(ユーザ定義関数) */
  bleSetup(); /* BLEの初期化(ユーザ定義関数) */
    
  displayOpening(); /* オープニング表示(ユーザ定義関数) */
  
  delay(100);
}

/* ■ メインループ */
void loop() {
  displayHeartRate(); /* 測定した心拍の表示(ユーザ定義関数) */

#if 1
  /***** デバッグ用 *****/
  /* 現在の心拍，バッテリ電圧，ボタン状態，人間バッテリー値をシリアルに出力 */
  Serial.print("beatAvg: ");
  Serial.println(getBeatAvg());
  Serial.print("Vbat: ");
  Serial.println(readBatteryVoltage());
  Serial.print("ButtonState: ");
  Serial.println(readPushSW());
  Serial.print("HumansBatteryValue: ");
  Serial.println(getHumansBattery());
#endif

  delay(1000);
}


