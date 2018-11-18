/* 【グラフィック関連処理】 */

/* ※注意※
 *   ディスプレイ表示更新の際は，必ずI2Cセマフォで資源確保する！勝手に使わない！ 
 */

/* ②OLEDディスプレイ */
#define OLED_RESET 4 /* OLEDのリセットピンはD4 */
#define OLED_ADDRESS 0x3C /* OLED's I2C Address is "0x3C" */
Adafruit_SSD1306 display(OLED_RESET);

/* displayHeartRate : 現在の心拍数を表示 */
void displayHeartRate(){
  char str[10];
  sprintf(str, "%3d." ,(int)beatAvg);
  
  while(i2cSemaphoreP() == 0); /* i2cセマフォ獲得できるまで待つ */
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  //display.println("Heart Rate Meter v0.0");
  //display.setTextSize(3);
  //display.print((int)beatAvg);
  if( (millis()%1000)<800 ){
    display.drawBitmap(32,0 , HEARTICON, 32, 32, 1);
  }
  display.setCursor(80,12);
  display.print(str);

  /*
  display.setTextSize(1);
  display.setCursor(60,10);
  display.print("ROW:");
  display.print(rowIrValue);
  display.setCursor(60,20);
  display.print("NOW:");
  display.print(beatsPerMinute);
  
  */
  display.display();
  i2cSemaphoreV(); /* i2cセマフォ解放 */
}

void displayBatteryIcon(uint8_t batt, uint8_t x, uint8_t y){
  /* バッテリアイコン表示  */
  if(batt<10){
    display.drawBitmap(x,y , BATTICON_LARGE_10P, 32, 32, 1);
  }
  else if(batt<20){
    display.drawBitmap(x,y , BATTICON_LARGE_20P, 32, 32, 1);
  }
  else if(batt<30){
    display.drawBitmap(x,y , BATTICON_LARGE_30P, 32, 32, 1);
  }
  else if(batt<40){
    display.drawBitmap(x,y , BATTICON_LARGE_40P, 32, 32, 1);
  }
  else if(batt<50){
    display.drawBitmap(x,y , BATTICON_LARGE_50P, 32, 32, 1);
  }
  else if(batt<60){
    display.drawBitmap(x,y , BATTICON_LARGE_60P, 32, 32, 1);
  }
  else if(batt<70){
    display.drawBitmap(x,y , BATTICON_LARGE_70P, 32, 32, 1);
  }
  else if(batt<80){
    display.drawBitmap(x,y , BATTICON_LARGE_80P, 32, 32, 1);
  }
  else if(batt<90){
    display.drawBitmap(x,y , BATTICON_LARGE_90P, 32, 32, 1);
  }
  else if(batt<=100){
    display.drawBitmap(x,y , BATTICON_LARGE_100P , 32, 32, 1);
  }
}

/* displayHumansBattery : 現在の人間バッテリー値を表示 */
void displayHumansBattery(){
  char str[10];
  sprintf(str, "%3d%%" ,(int)getHumansBattery());
  
  while(i2cSemaphoreP() == 0); /* i2cセマフォ獲得できるまで待つ */
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(80,12);
  display.print(str);

  displayBatteryIcon(getHumansBattery(), 32, 0);
  
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
  else if(getHumansBattery()<=100){
    display.drawBitmap(90,0 , BATTICON_LARGE_100P , 32, 32, 1);
  }
  #endif

  display.display();
  i2cSemaphoreV(); /* i2cセマフォ解放 */
}


/* displayLiionBattery : 現在の物理バッテリー値を表示 */
void displayLiionBattery(){
  static uint8_t cnt=0;
  static uint8_t batt;
  static uint8_t vbat_h, vbat_l;
  char str[10];
  while(i2cSemaphoreP() == 0); /* i2cセマフォ獲得できるまで待つ */

  /* 256回に一度，値を更新する（揺れすぎるため反応を遅くして対処） */
  if(cnt == 0){
    batt = Convert_battery_to_percentage();
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  //display.println("Battery");
  //display.setTextSize(3);
  display.setCursor(80,12);

  vbat_h = readBatteryVoltage()/1000;
  vbat_l = (readBatteryVoltage()%1000)/100;

  sprintf(str, "%d%% %d.%dv", batt, vbat_h, vbat_l);
  display.print(str);

  displayBatteryIcon(batt, 32, 0);
  
#if 0
  /* バッテリアイコン表示  */
  if(batt<10){
    display.drawBitmap(90,0 , BATTICON_LARGE_10P, 32, 32, 1);
  }
  else if(batt<20){
    display.drawBitmap(90,0 , BATTICON_LARGE_20P, 32, 32, 1);
  }
  else if(batt<30){
    display.drawBitmap(90,0 , BATTICON_LARGE_30P, 32, 32, 1);
  }
  else if(batt<40){
    display.drawBitmap(90,0 , BATTICON_LARGE_40P, 32, 32, 1);
  }
  else if(batt<50){
    display.drawBitmap(90,0 , BATTICON_LARGE_50P, 32, 32, 1);
  }
  else if(batt<60){
    display.drawBitmap(90,0 , BATTICON_LARGE_60P, 32, 32, 1);
  }
  else if(batt<70){
    display.drawBitmap(90,0 , BATTICON_LARGE_70P, 32, 32, 1);
  }
  else if(batt<80){
    display.drawBitmap(90,0 , BATTICON_LARGE_80P, 32, 32, 1);
  }
  else if(batt<90){
    display.drawBitmap(90,0 , BATTICON_LARGE_90P, 32, 32, 1);
  }
  else if(batt<=100){
    display.drawBitmap(90,0 , BATTICON_LARGE_100P , 32, 32, 1);
  }
  #endif

  display.display();
  i2cSemaphoreV(); /* i2cセマフォ解放 */
  cnt++;
}

/* displayOpening : オープニング画面を表示 */
void displayOpening(){
  while(i2cSemaphoreP() == 0); /* i2cセマフォ獲得できるまで待つ */
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println();
  display.drawBitmap(32,0 , SHAKEICON_L, 32, 32, 1);
  display.drawBitmap(64,0 , SHAKEICON_R, 32, 32, 1);
  display.display();
  i2cSemaphoreV(); /* i2cセマフォ解放 */
  delay(2000);

  while(i2cSemaphoreP() == 0); /* i2cセマフォ獲得できるまで待つ */
  display.setCursor(0,24);
  display.setTextSize(1);
  display.println("     Please Wait.");
  display.display();
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

