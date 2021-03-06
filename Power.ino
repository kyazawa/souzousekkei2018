/* 【電源関連処理】 */

/* ■ ピン定義 */
#define POWER_PIN 14
#define LED_PIN 2
#define PUSHSW_PIN 12
#define VBAT_PIN 34
#define BATTOFST (+320) /* バッテリ電圧ｵﾌｾｯﾄ値（微調整） */

/* ■ グローバル変数定義 */
unsigned int BattADValue = 0;

/* 電源をオン（保持）する関数 */
void powerON(){
  digitalWrite(POWER_PIN, HIGH);
}

/* 電源をオフする関数 */
void powerOFF(){
  digitalWrite(POWER_PIN, LOW);
}

/* LEDの点灯，消灯 */
void controlLED(boolean value){
  digitalWrite(LED_PIN, value);
}

/* ピンの初期化 */
void initPin(){
  pinMode(POWER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUSHSW_PIN, INPUT_PULLUP);
  pinMode(VBAT_PIN, ANALOG);
}

/* ボタンの読み取り(チャタリング除去後の値) */
boolean readPushSW(){
  static boolean swstate = 0; /* 0:押されている 1:押されていない */
  static boolean swstate_old = swstate; /* swstateの前回の値 */
  static boolean sw_toggled = 1; /* 0:トグルされた，1:トグルされていない */
  swstate_old = swstate;

  /* チャタリングの除去 */
  if( (swvalue & 0xF) == 0xF ){ /* swvalueはPINREAD_TSKで生成されている */
    swstate = 1; /* 4回連続1なら1 */
  }else if((swvalue & 0xF) == 0x0){
    swstate = 0; /* 4回連続0なら0 */
  } /* それ以外の場合，そのままの値を保持する */

  /* トグル検出 */
  if(swstate != swstate_old){ /* 前回からスイッチがトグルされていたら0 */
    sw_toggled = 0;
  }else{
    sw_toggled = 1;
  }

  /* 立ち上がりのみ検出 */
  if((swstate == 0) && (sw_toggled == 0)){
    return 0; /* ボタンが押された */
  }else{
    return 1; /* ボタンが押されていない */
  }
}

/* ボタンピン値の取得 */
boolean getPushSWRawValue(){
  boolean result;
  result = digitalRead(PUSHSW_PIN);
  return result;
}

/* 電池電圧を検出する関数 [mv]で返す！ */
unsigned int readBatteryVoltage(){
  unsigned int adValue;
  unsigned int bVoltage;
  adValue = analogRead(VBAT_PIN); /* 12bitAD値取得 */
  bVoltage = (1611 * adValue) / 1000; /* AD値→電池電圧 換算 */
  bVoltage += BATTOFST;
  return bVoltage;
}

/* バッテリ電圧平均化処理  */
void averagingBattery(){
  static uint8_t cnt=0;
  
  if(BattADValue == 0){
    BattADValue = readBatteryVoltage();
  }else{
    BattADValue += readBatteryVoltage();
    BattADValue /= 2;
  }
  
}

/* 平均化済みバッテリ電圧を返す */
unsigned int getAvgBattVoltage(){
  return BattADValue;
}

