/* まとめて関数プロトタイプ宣言 */

/* BLE関連 */
void bleSetup();
void sendBleMsg(char * str);

/* I2C */
void i2cSemaphoreV();
boolean i2cSemaphoreP();

/* 電源関連 */
/* 電源をオン（保持）する関数 */
void powerON();
/* 電源をオフする関数 */
void powerOFF();
/* LEDの点灯，消灯 */
void controlLED(boolean value);
/* ピンの初期化 */
void initPin();
/* ボタンの読み取り(チャタリング除去後の値) */
boolean readPushSW();
/* ボタンピン値の取得 */
boolean getPushSWRawValue();
/* 人間バッテリー値を返す関数（仮） */
int getHumansBattery(void);
/* 電池電圧を検出する関数 [mv]で返す！ */
unsigned int readBatteryVoltage();

/* 心拍測定 */
/* displayHeartRate : 現在の心拍数を表示 */
void displayHeartRate();
/* displayHumansBattery : 現在の人間バッテリー値を表示 */
void displayHumansBattery();
/* displayOpening : オープニング画面を表示 */
void displayOpening();
/* displaySetup : ディスプレイの初期化設定 */
void displaySetup();

void sensorSetup();
long measureHeartRate();
int getBeatAvg();

