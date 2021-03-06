/* 関数リスト！ （関数プロトタイプ宣言） */

/* BLEの有効無効
 * BLEは意味不明なので ↓を0にするとBLE関連のプログラムはコメントアウトされる
 */
#define BLE_EN 0

#include "MAX30105.h"  /* 心拍センサライブラリ */
#include "heartRate.h" /* 心拍測定ライブラリ */

#if BLE_EN
#include <BLEDevice.h> /* BLE通信ライブラリ */  
#include <BLE2902.h>   /* BLE ESP32固有ライブラリ*/
#endif

#include <Adafruit_GFX.h> /* ディスプレイ用グラフィックライブラリ */
#include <Adafruit_SSD1306.h> /* ディスプレイ用ライブラリ */
#include "BluetoothSerial.h"

#if 0
/*MPU6050 使用ライブラリ*/
#include "MPU6050_6Axis_MotionApps20.h"
#endif

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


#include <WiFi.h>

/* バッテリ残量アイコン  */
#include "BattIcon.h"

/* ● BLE関連(BLE.ino) */
#if BLE_EN
/* BLE初期化・接続 */
void bleSetup();
/* BLEﾒｯｾｰｼﾞ(nortify)送信 */
void sendBleMsg(char * str);
/* BLEﾒｯｾｰｼﾞ受信処理 */
void recvBleMsg(char * str);
#endif

void setupSPP();
/* BLEでメッセージ送信 */
void sendSppMsg(char * str);
/* BLEﾒｯｾｰｼﾞ受信処理 */
void recvSppMsg(char * str);
/* Unknown */
void sendProfile();
/* Unknown */
void execCmd();
/* Unknown */
void parseHbData(char *str);
/* Unknown */
int Atoi(char a);
/* ダミーデータ単体で送る */
void sendSingleDummyData();


/* ● I2Cセマフォ(Semaphore.ino) */
void i2cSemaphoreV();       /* V命令：資源解法 */
boolean i2cSemaphoreP();    /* P命令：資源獲得 */

/* ● 電源関連(Power.ino) */
/* 電源をオン（保持）する関数(起動時に実行する) */
void powerON();
/* 電源をオフする関数 */
void powerOFF();
/* LEDの点灯，消灯  (点灯=1, 消灯=0)*/
void controlLED(boolean value);
/* ピンの初期化 */
void initPin();
/* ボタンの読み取り(チャタリング除去後の値) (押されている=0, 押されてない=1) */
boolean readPushSW();
/* ボタンピン生の値の取得 */
boolean getPushSWRawValue();
/* 人間バッテリー値を返す関数（仮） */
int getHumansBattery(void);
/* 電池電圧を検出する関数 (0～4200[mv] 整数値で返す) */
unsigned int readBatteryVoltage();
/* バッテリ電圧平均化処理  */
void averagingBattery();
/* 平均化済みバッテリ電圧を返す */
unsigned int getAvgBattVoltage();


/* ● ディスプレイ表示関連(Display.ino) */
/* 現在の心拍数を表示 */
void displayHeartRate();
/* 現在の人間バッテリー値を表示 */
void displayHumansBattery();
/* オープニング画面を表示 */
void displayOpening();
/* ディスプレイの初期化設定 */
void displaySetup();
/* ディスプレイに現在ｽﾃｰﾀｽを表示する */
void displayMonitor();
/* ディスプレイ消す */
void displayOff();
/* 現在の物理バッテリー値を表示 */
void displayLiionBattery();

/* ● 心拍測定関連(HeartRate.ino) */
/* 心拍センサ―初期化 */
void sensorSetup();
/* 心拍測定（１回分） */
long measureHeartRate();
/* 現在の心拍数平均を返す */
int getBeatAvg();

/*<PU6050関連*/
void Gryo_Start();
void GyroGet();
void GyroInit();

/*beat_queueユーザ関数定義*/
void dequeue();
void enqueue(double);
void calucAvg();
double display_queue(int);

/*ダミーデータ送信用*/
void send_dummy_beat();

/*バッテリーパーセント表示*/
int Convert_battery_to_percentage();
int Charge_battery() ;

