/* 【グラフィック関連処理】 */
#include <Adafruit_GFX.h> /* ディスプレイ用グラフィックライブラリ */
#include <Adafruit_SSD1306.h> /* ディスプレイ用ライブラリ */


/* ②OLEDディスプレイ */
#define OLED_RESET 4 /* OLEDのリセットピンはD4 */
#define OLED_ADDRESS 0x3C /* OLED's I2C Address is "0x3C" */
Adafruit_SSD1306 display(OLED_RESET);

/* displayHeartRate : 現在の心拍数を表示 */
void displayHeartRate(){  
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
}

/* displayHumansBattery : 現在の人間バッテリー値を表示 */
void displayHumansBattery(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Humans Battery");
  display.setTextSize(3);
  display.print((int)getHumansBattery());
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
