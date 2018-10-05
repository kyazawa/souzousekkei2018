/* 【グラフィック関連処理】 */
#include <Adafruit_GFX.h> /* ディスプレイ用グラフィックライブラリ */
#include <Adafruit_SSD1306.h> /* ディスプレイ用ライブラリ */

/* ※注意※
 *   ディスプレイ表示更新の際は，必ずI2Cセマフォで資源確保する！勝手に使わない！ 
 */

/* ②OLEDディスプレイ */
#define OLED_RESET 4 /* OLEDのリセットピンはD4 */
#define OLED_ADDRESS 0x3C /* OLED's I2C Address is "0x3C" */
Adafruit_SSD1306 display(OLED_RESET);

/* displayHeartRate : 現在の心拍数を表示 */
void displayHeartRate(){
  while(i2cSemaphoreP() == 0); /* i2cセマフォ獲得できるまで待つ */
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
  i2cSemaphoreV(); /* i2cセマフォ解放 */
}

/* displayHumansBattery : 現在の人間バッテリー値を表示 */
void displayHumansBattery(){
  while(i2cSemaphoreP() == 0); /* i2cセマフォ獲得できるまで待つ */
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Humans Battery");
  display.setTextSize(3);
  display.print((int)getHumansBattery());
  display.display();
  i2cSemaphoreV(); /* i2cセマフォ解放 */
}


/* displayOpening : オープニング画面を表示 */
void displayOpening(){
  while(i2cSemaphoreP() == 0); /* i2cセマフォ獲得できるまで待つ */
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("NIT S4 PL1");
  display.setTextSize(2);
  display.println("HeartRate");
  display.setTextSize(1);
  display.println("        Meter v0.0");
  display.display();
  i2cSemaphoreV(); /* i2cセマフォ解放 */
  delay(2000);

  while(i2cSemaphoreP() == 0); /* i2cセマフォ獲得できるまで待つ */
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Heart Rate Meter v0.0");
  display.setTextSize(1);
  display.println("Set your finger!");
  display.display(); //これをして初めて表示される
  i2cSemaphoreV(); /* i2cセマフォ解放 */
  delay(2000);
}

/* displaySetup : ディスプレイの初期化設定 */
void displaySetup(){
  while(i2cSemaphoreP() == 0); /* i2cセマフォ獲得できるまで待つ */
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
  display.display();   
  display.clearDisplay();
  delay(10); 
  i2cSemaphoreV(); /* i2cセマフォ解放 */
}

/* displayMonitor : ディスプレイに現在ｽﾃｰﾀｽを表示する */
void displayMonitor(){
  while(i2cSemaphoreP() == 0); /* i2cセマフォ獲得できるまで待つ */
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("-TeaWatch Monitor-");
  display.print("beat: ");
  display.print(getBeatAvg());
  display.print(" Vbat: ");
  display.println(readBatteryVoltage());
  display.print("Button: ");
  display.print(readPushSW());
  display.print(" HBT: ");
  display.println(getHumansBattery());
  display.display();
  i2cSemaphoreV(); /* i2cセマフォ解放 */
}

