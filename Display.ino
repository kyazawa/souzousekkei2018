/* 【グラフィック関連処理】 */

/* ※注意※
 *   ディスプレイ表示更新の際は，必ずI2Cセマフォで資源確保する！勝手に使わない！ 
 */

/* ②OLEDディスプレイ */
#define OLED_RESET 4 /* OLEDのリセットピンはD4 */
#define OLED_ADDRESS 0x3C /* OLED's I2C Address is "0x3C" */
Adafruit_SSD1306 display(OLED_RESET);

static const unsigned char PROGMEM logo16_glcd_bmp[] ={
  B00000011, B11000000,
  B00000010, B01000000,
  B00011110, B01111000,
  B00010000, B00001000,
  B00010111, B11101000,
  B00010111, B11101000,
  B00010111, B11101000,
  B00010111, B11101000,
  B00010111, B11101000,
  B00010111, B11101000,
  B00010111, B11101000,
  B00010111, B11101000,
  B00010111, B11101000,
  B00010111, B11101000,
  B00010000, B00001000,
  B00011111, B11111000,
};

static const unsigned char PROGMEM logo32_glcd_bmp[] ={
  B00000000, B11111111, B11111111, B00000000,
  B00000000, B10000000, B00000001, B00000000,
  B00000000, B10000000, B00000001, B00000000,
  B11111111, B10000000, B00000001, B11111111,
  B10000000, B00000000, B00000000, B00000001,
  B10111111, B11111111, B11111111, B11111101,
  B10111111, B11111111, B11111111, B11111101,
  B10000000, B00000000, B00000000, B00000001,
  B10111111, B11111111, B11111111, B11111101,
  B10111111, B11111111, B11111111, B11111101,
  B10000000, B00000000, B00000000, B00000001,
  B10111111, B11111111, B11111111, B11111101,
  B10111111, B11111111, B11111111, B11111101,
  B10000000, B00000000, B00000000, B00000001,
  B10111111, B11111111, B11111111, B11111101,
  B10111111, B11111111, B11111111, B11111101,
  B10000000, B00000000, B00000000, B00000001,
  B10111111, B11111111, B11111111, B11111101,
  B10111111, B11111111, B11111111, B11111101,
  B10000000, B00000000, B00000000, B00000001,
  B10111111, B11111111, B11111111, B11111101,
  B10111111, B11111111, B11111111, B11111101,
  B10000000, B00000000, B00000000, B00000001,
  B10111111, B11111111, B11111111, B11111101,
  B10111111, B11111111, B11111111, B11111101,
  B10000000, B00000000, B00000000, B00000001,
  B10111111, B11111111, B11111111, B11111101,
  B10111111, B11111111, B11111111, B11111101,
  B10111111, B11111111, B11111111, B11111101,
  B10111111, B11111111, B11111111, B11111101,
  B10000000, B00000000, B00000000, B00000001,
  B11111111, B11111111, B11111111, B11111111

};

/* 50% */
static const unsigned char PROGMEM BATTICON_LARGE_50P[] = {
  B00000000, B10000000, B00000001, B00000000,
  B00000000, B10000000, B00000001, B00000000,
  B00000000, B10000000, B00000001, B00000000,
  B11111111, B10000000, B00000001, B11111111,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10111111, B11100000, B00000000, B00000001,
  B10000000, B00011111, B11111000, B00000001,
  B10000000, B00000000, B00000111, B11111101,
  B10111111, B11100000, B00000000, B00000001,
  B10000000, B00011111, B11111000, B00000001,
  B10000000, B00000000, B00000111, B11111101,
  B10111111, B11100000, B00000000, B00000001,
  B10000000, B00011111, B11111000, B00000001,
  B10000000, B00000000, B00000111, B11111101,
  B10111111, B11100000, B00000000, B00000001,
  B10000000, B00011111, B11111000, B00000001,
  B10000000, B00000000, B00000111, B11111101,
  B10111111, B11100000, B00000000, B00000001,
  B10000000, B00011111, B11111000, B00000001,
  B10000000, B00000000, B00000111, B11111101,
  B10000000, B00000000, B00000000, B00000001,
  B11111111, B11111111, B11111111, B11111111,
};

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
  display.display();
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
  //display.drawBitmap(80,16 , logo16_glcd_bmp, 16, 16, 1);
  display.drawBitmap(90,0 , logo32_glcd_bmp, 32, 32, 1);

#if 0
  /* バッテリアイコン表示  */
  if(getHumansBattery()<10){
    display.drawBitmap(90,0 , BATTICON_LARGE_10P, 32, 32, 1);
  }
  else if(getHumansBattery()<20){
    display.drawBitmap(90,0 , BATTICON_LARGE_20P, 32, 32, 1);
  }
  else if(getHumansBattery()<30){
    display.drawBitmap(90,0 , BATTICON_LARGE_30P, 32, 32, 1);
  }
  else if(getHumansBattery()<40){
    display.drawBitmap(90,0 , BATTICON_LARGE_40P, 32, 32, 1);
  }
  else if(getHumansBattery()<50){
    display.drawBitmap(90,0 , BATTICON_LARGE_50P, 32, 32, 1);
  }
  else if(getHumansBattery()<60){
    display.drawBitmap(90,0 , BATTICON_LARGE_60P, 32, 32, 1);
  }
  else if(getHumansBattery()<70){
    display.drawBitmap(90,0 , BATTICON_LARGE_70P, 32, 32, 1);
  }
  else if(getHumansBattery()<80){
    display.drawBitmap(90,0 , BATTICON_LARGE_80P, 32, 32, 1);
  }
  else if(getHumansBattery()<90){
    display.drawBitmap(90,0 , BATTICON_LARGE_90P, 32, 32, 1);
  }
  else(getHumansBattery()<=100){
    display.drawBitmap(90,0 , BATTICON_LARGE_100P , 32, 32, 1);
  }
  #endif

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
  display.print(" HBT: ");
  display.println(getHumansBattery());
  display.display();
  i2cSemaphoreV(); /* i2cセマフォ解放 */
}

/* displayOff: ディスプレイ消す */
void displayOff(){
  while(i2cSemaphoreP() == 0); /* i2cセマフォ獲得できるまで待つ */
  display.clearDisplay();
  display.display();
  i2cSemaphoreV(); /* i2cセマフォ解放 */
}

