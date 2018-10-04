/* 【電源関連処理】 */

/* ■ ピン定義 */
#define POWER_PIN 14
#define LED_PIN 2
#define PUSHSW_PIN 12

/* 電源をオン（保持）する関数 */
void powerON(){
  digitalWrite(14, HIGH);
}

/* 電源をオフする関数 */
void powerOFF(){
  digitalWrite(14, LOW);
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
}

/* ボタンの読み取り(チャタリング除去後の値) */
boolean readPushSW(){
  static boolean swstate = 0;
  if( (swvalue & 0xF) == 0xF ){
    swstate = 1; /* 4回連続1なら1 */
  }else if((swvalue & 0xF) == 0x0){
    swstate = 0; /* 4回連続0なら0 */
  } /* それ以外の場合，そのままの値を保持する */
  return swstate;
}

/* ボタンピン値の取得 */
boolean getPushSWRawValue(){
  boolean result;
  result = digitalRead(PUSHSW_PIN);
  return result;
}

/* 人間バッテリー値を返す関数 */
int getHumansBattery(void){
  return 50;
}

