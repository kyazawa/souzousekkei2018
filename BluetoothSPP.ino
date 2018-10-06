BluetoothSerial SerialBT;

void setupSPP(){
  SerialBT.begin("TeaWatch"); 
  sendSppMsg("TeaWatch SPP Connected.\n");
}

/* BLEでメッセージ送信 
 * エラーハンドリングしていないので，多分簡単に死ぬ
 */
void sendSppMsg(char * str){
  while( *str  != '\0'){
    SerialBT.write(*str);
    str++;
  }
}

/* BLEﾒｯｾｰｼﾞ受信処理 
 * エラーハンドリングしていないので，多分簡単に死ぬ
 */
void recvSppMsg(char * str){
  while(SerialBT.available()){
    *str = SerialBT.read();
    str++;
  }
  *str = '\0';
}
