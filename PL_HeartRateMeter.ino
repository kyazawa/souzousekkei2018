/* 心拍センサMAX30105 & OLEDディスプレイSSD1306 テストプログラム
 * 『Heart Rate Meter』 v0.1 【ESP-WROOM-32対応版】
 *  2018.06.12. (Tue)
 *  創造設計2018 1班エレキ担当 矢澤杏平🍵
 *  Thanks For SparkFun(Sensor) & AdaFruit(OLED) !!
 */

/* 【来歴】
 *  2018.06.12. v0.0 新規作成 矢澤
 *  2018.06.26  v0.1 【ESP-WROOM-32対応版】
 *                    表示の配置を変更。平均値を中心にナマの値・イマの値を表示するように更新
 *                    ＬＥＤの明るさを最大にした
 */
 
/* 【メモリ使用率】 v0.0現在
 Flash: 15036/32256[Bytes] (46%)
 RAM:    1401/ 2048[Bytes] (68%)
*/

/* 【概要】
 *  センサに指をかざすと心拍数がOLEDディスプレイに表示されます。
 *  その他，わからないことがあったら作成者(矢澤)に聞いてください。
 *  プログラムからしてコメントの嵐で面倒な奴感でててすまんな🍵
 */

/* 【使用回路】
 *  [ESP32] --I2C-- [MAX30105] --I2C-- [OLED SSD1306]
 *                         (Addr:0xAE)        (Addr:0x3C)
 *  ※ 電源はいずれも3.3Vです。I2Cバスは4.7kΩでプルアップする。
 */



/* ■ ライブラリのインクルド */
/* 【通信系】 */
#include <SPI.h> /* SPIライブラリ */
#include <Wire.h> /* I2Cライブラリ */
/* 【グラフィック関連】 */
#include <Adafruit_GFX.h> /* ディスプレイ用グラフィックライブラリ */
#include <Adafruit_SSD1306.h> /* ディスプレイ用ライブラリ */
/* 【心拍センサ関連】 */
#include "MAX30105.h"  /* 心拍センサライブラリ */
#include "heartRate.h" /* 心拍測定ライブラリ */

/* ■ オブジェクトの定義 */
/* ①心拍センサ */
MAX30105 particleSensor;
//#define SENSORS_ADDRESS 0xAE /* このライブラリではアドレスは指定しなくていいらしい */
/* ②OLEDディスプレイ */
#define OLED_RESET 4 /* OLEDのリセットピンはD4 */
#define OLED_ADDRESS 0x3C /* OLED's I2C Address is "0x3C" */
Adafruit_SSD1306 display(OLED_RESET);

/* ■心拍測定用グローバル変数 */
const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
float beatsPerMinute;
int beatAvg;
/* 上記はサンプル『Exaple5_HeartRate』からのコピペ */
long rowIrValue = 0;


/* ■ 初期化プログラム */
void setup() {
  /* シリアルポートの設定(デバッグ用) */
  Serial.begin(115200);
  Serial.println("Heart Rate Meter v0.0");

  sensorSetup(); /* センサの初期化(ユーザ定義関数) */
  displaySetup(); /* ディスプレイ初期化(ユーザ定義関数) */
  displayOpening(); /* オープニング表示(ユーザ定義関数) */
}

/* ■ メインループ */
void loop() {
  rowIrValue = measureHeartRate(); /* 心拍の測定(ユーザ定義関数) */
  displayHeartRate(); /* 測定した心拍の表示(ユーザ定義関数) */
}




/* ■ ユーザ定義関数 */
/* 🍵ここから下はあんまり整理してないので注意🍵 */

/* displayHeartRate : 現在の心拍数を表示 */
void displayHeartRate(){  
  static byte kaisuu;
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Heart Rate Meter v0.0");
  display.setTextSize(3);
  display.print((int)beatAvg);

  display.setTextSize(1);
  display.setCursor(60,10);
  display.print("ROW:");
  display.print(rowIrValue);
  display.setCursor(60,20);
  display.print("NOW:");
  display.print(beatsPerMinute);

  if(kaisuu++ % 20 == 0){
    display.display();
  }
}

/* displayOpening : オープニング画面を表示 */
void displayOpening(){
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("NIT S4 PL1");
  display.setTextSize(2);
  display.println("HeartRate");
  display.setTextSize(1);
  display.println("        Meter v0.0");
  display.display();
  delay(2000);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Heart Rate Meter v0.0");
  display.setTextSize(1);
  display.println("Set your finger!");
  display.display(); //これをして初めて表示される
  delay(2000);
}

/* displaySetup : ディスプレイの初期化設定 */
void displaySetup(){
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
  display.display();   
  display.clearDisplay();
  delay(10); 
}

/* sensorSetup : センサの初期化設定 */
void sensorSetup(){
  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0xFF); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
}

/* measureHeartRate : 現在の心拍数を測定 */
long measureHeartRate(){
  long irValue = particleSensor.getIR();
  Serial.println(irValue);
  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 50)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }
  return irValue;
}

