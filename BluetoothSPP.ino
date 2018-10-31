/*【Bluetooth SPP関連】*/
BluetoothSerial SerialBT;
int humansBattery; 

void setupSPP(){
  SerialBT.begin("TeaWatch"); 
  sendSppMsg("TeaWatch SPP Connected.\n");
}

/* SPPでメッセージ送信 
 * エラーハンドリングしていないので，多分簡単に死ぬ
 */
void sendSppMsg(char * str){
  SerialBT.write((uint8_t *)str, strlen(str));
}

/* SPPﾒｯｾｰｼﾞ受信処理 
 * エラーハンドリングしていないので，多分簡単に死ぬ
 */
void recvSppMsg(char * str){
  while(SerialBT.available()){
    *str = SerialBT.read();
    str++;
  }
  *str = '\0';
}

/* SPPで現在の心拍送信 */
void sendBeatAvg(){
  int beat;
  char str[10];
  beat = getBeatAvg();
  sprintf(str, "%d" ,beat);
  sendSppMsg(str);
}

/* 心拍蓄積データを一気に送信 */
void sendBarstBeatAvg(){
  char str[100];
  sprintf(str, "B ");
  for(int i=0; i<50; i++){

    //一括送信 要実装！！！
    // (送信ｲﾒｰｼﾞ) @B82,65,23,74,26,85,34,85,97........
    
    sprintf(str, "%d,", getBeatAvg());
  }
  sendSppMsg(str);
}

void sendProfile(){
  sendSppMsg("TeaWatch by SouzouSekkei2018 Group1");
}

/* Bluetoothできたコマンドをパースし，実行 */
void execCmd(){
  char str[100];
  int i=0;
  
  recvSppMsg(str);
    
  while((str[i]!='\0') && (str[i]!='\n')){
    if(str[i] == '@'){
      switch(str[i+1]){
        case 'B':
          sendBarstBeatAvg();
          break;
        case 'b':
          sendBeatAvg();
          break;
        case 'v':
          sendProfile();
          break;
        case 'h':
          parseHbData(&str[i+2]);
          break;
      }
    }
    i++;
  }
}

/* 親機に人間バッテリー値送信要求 */
void callHumansBattery(){
  sendSppMsg("@h");
}

/* 人間バッテリー値を親機から受け取り */
void parseHbData(char * str){
  int i=0;
  int hbattery=0;
  while((str[i]!='\0') && (str[i]!='\n')){
    hbattery *= 10;  /* 桁繰り上げ */
    hbattery += Atoi(str[i]); /* char⇒int変換 */
    Serial.print(Atoi(str[i]));
    Serial.print(" ");
    Serial.println(hbattery);
    i++;
  }
  humansBattery = hbattery; /* 現在人間バッテリー値をグローバル変数に代入 */
}

/* 人間バッテリー値を返す関数（仮） */
int getHumansBattery(void){
  return humansBattery;

  /* 旧コード！
  char str[50];
  int hBattery;
  recvSppMsg(str);
  sscanf(str, "%d", hBattery);
  return hBattery;
  */
}

/* 文字(ASCII)から数字(Int)に変換する関数 */
int Atoi(char a){
  return (a-'0');
}

