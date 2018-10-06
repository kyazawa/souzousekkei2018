/* 心拍センサMAX30105 & OLEDディスプレイSSD1306 テストプログラム
 * 『Heart Rate Meter』 v0.5 【ESP-WROOM-32対応版】
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
 *  2018.10.05  v0.5  心拍タスクに記述ミスがあり修正，とりあえず動作確認済み🍵
 *  2018.10.06  v0.6  ピン読み込み用タスクを追加。ボタン読み取りアルゴリズムを改善。
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
boolean clickedSW = 1;

/* ■ 心拍測定 タスク 
 *  測定後の心拍はグローバル変数『beatAvg』より取得可能
 */
portTickType Delay1 = portTICK_RATE_MS; //freeRTOS 用の遅延時間定義
TaskHandle_t hTsk; /* タスクハンドルポインタ */
void HEARTRATE_TSK(void *pvParameters){
  while(1){
    /* 心拍の測定 */
    rowIrValue = measureHeartRate(); 
  }
}

/* ■ ピン読み込み タスク
 *  
 */
TaskHandle_t pTsk;
void PINREAD_TSK(void *pvParameters){
  while(1){
    /* ボタン入力値のサンプリング(10ms周期) */
    swvalue <<= 1;
    swvalue |= getPushSWRawValue();

 /*
    if((readPushSW()==0) && (clickedSW==1)){
      clickedSW = 0;
      Serial.println("clicked");
    }
    */
    delay(10);
  }
}


/* ■ 初期化プログラム */
void setup() {
  initPin(); /* ピン入出力初期化 */
  powerON(); /* 電源オンする */
  
  /* シリアルポートの設定(デバッグ用) */
  Serial.begin(115200);
  Serial.println("Heart Rate Meter v0.0");
  
  sensorSetup(); /* センサの初期化(ユーザ定義関数) */
  displaySetup(); /* ディスプレイ初期化(ユーザ定義関数) */
  //bleSetup(); /* BLEの初期化(ユーザ定義関数) */
    
  displayOpening(); /* オープニング表示(ユーザ定義関数) */
  
  delay(100);

  /* ピン読み込みタスク起動 */
  xTaskCreatePinnedToCore(
           PINREAD_TSK,   /* タスク名 */
           "PINREADE_TSK", /* ”タスク名” */
           1024,            /* スタックサイズ */
           NULL,            /* NULL(意味なし) */
           1,               /* タスク優先度 */
           &pTsk,           /* タスクハンドラ */
           0                /* 動作コア(0 or 1) */
  );

  /* 心拍測定タスク起動 */
  xTaskCreatePinnedToCore(
           HEARTRATE_TSK,   /* タスク名 */
           "HEARTRATE_TSK", /* ”タスク名” */
           4096,            /* スタックサイズ */
           NULL,            /* NULL(意味なし) */
           1,               /* タスク優先度 */
           &hTsk,           /* タスクハンドラ */
           0                /* 動作コア(0 or 1) */
  );
  
  delay(1000);
  
}

/* ■ メインループ */
void loop() {
  static boolean dmode=0;
  //displayHeartRate(); /* 測定した心拍の表示(ユーザ定義関数) */

#if 0
  /***** デバッグ用 *****/
  /* 現在の心拍，バッテリ電圧，ボタン状態，人間バッテリー値をシリアルに出力 */
  Serial.print("beatAvg: ");
  Serial.println(getBeatAvg());
  Serial.print("Vbat: ");
  Serial.println(readBatteryVoltage());
  Serial.print("HumansBatteryValue: ");
  Serial.println(getHumansBattery());
  Serial.print("dmode: ");
  Serial.println(dmode);
#endif
  if(readPushSW()==0){
    dmode = !dmode;
    delay(100);
    clickedSW = 1;
  }

  if(dmode == 1){
    displayMonitor();
  }else{
    displayHeartRate();
  }
  delay(50);
  
}


