/*【Bluetooth SPP関連】*/
BluetoothSerial SerialBT;
int humansBattery;
void setupSPP() {
  SerialBT.begin("TeaWatch");
  sendSppMsg("TeaWatch SPP Connected.\n");
}
/* SPPでメッセージ送信
   エラーハンドリングしていないので，多分簡単に死ぬ
*/
void sendSppMsg(char * str) {
  SerialBT.write((uint8_t *)str, strlen(str));
  Serial.print("BT sent:");
  Serial.println(str);
}
/* SPPﾒｯｾｰｼﾞ受信処理
   エラーハンドリングしていないので，多分簡単に死ぬ
*/
void recvSppMsg(char * str) {
  while (SerialBT.available()) {
    *str = SerialBT.read();
    str++;
  }
  *str = '\0';
  Serial.print("BT recv:");
  Serial.println(str);
}
/* SPPで現在の心拍送信 */
void sendBeatAvg() {
  int beat;
  char str[10];
  beat = getBeatAvg();
  sprintf(str, "%d" , beat);
  sendSppMsg(str);
}
/* 心拍蓄積データを一気に送信 */
void sendBarstBeatAvg() {
  //一括送信 要実装！！！
  // (送信ｲﾒｰｼﾞ) @B82,65,23,74,26,85,34,85,97........
  char str[100];
  sprintf(str, "B ");
  int i = 0, b = 0;
  while (!isnan(display_queue(b))) {
    while (str[i] != '\0') {
      i++;
    }
    sprintf(&str[i], "%d,", (int)display_queue(b));
    b++;
  }
  sendSppMsg(str);
}
/*ダミーデータの送信*/
void send_dummy_beat() {
  char dummy_point[1000];
  /*ダミーデータ初期化*/
  //1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67  68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 00  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20  21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51  52  53  54  55  56 57 58 59 60 61 62 63 64 65 66 67 68 69 70  71  72 73  74  75  76  77 78 79  80 81  82 83  84  85  86  87  88  89  90  91 92 93  94  95  96  97  98  99  00  1   2  3  4   5   6   7  8  9   10 11 12 13 14 15 16 17 18 19 20 21 22 23 24  25 26  27  28 29 30  31 32 33 34 35  36  37  38 39 40  41  42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88
  int dummy_beat_data[] = {59, 56, 69, 77, 63, 60, 56, 62, 65, 59, 71, 78, 75, 54, 62, 61, 56, 65, 57, 57, 52, 55, 72, 57, 55, 56, 59, 61, 71, 56,
                           56, 57, 65, 64, 59, 53, 53, 52, 50, 50, 49, 49, 49, 54, 53, 53, 48, 74, 56, 57, 54, 78, 68, 65, 86, 55, 75, 62, 64, 65,
                           71, 83, 79, 81, 92, 87, 102, 80, 84, 72, 72, 78, 59, 72, 61, 74, 69, 56, 59, 72, 96, 72, 68, 62, 64, 76, 90, 63, 78, 76,
                           80, 72, 62, 77, 67, 91, 74, 68, 69, 91, 78, 75, 68, 77, 77, 70, 80, 71, 64, 64, 63, 73, 60, 75, 69, 85, 80, 99, 90, 73,
                           103, 65, 70, 66, 69, 87, 79, 78, 96, 93, 89, 89, 81, 96, 97, 99, 89, 84, 90, 82, 77, 83, 92, 89, 74, 69, 89, 86, 80, 94,
                           94, 113, 111, 129, 126, 106, 98, 84, 88, 96, 87, 83, 84, 83, 73, 80, 71, 72, 95, 95, 117, 126, 98, 103, 105, 114, 106,
                           92, 99, 104, 97, 113, 129, 121, 118, 118, 118, 104, 110, 89, 127, 88, 90, 124, 121, 105, 122, 114, 115, 92, 104, 87, 72,
                           75, 122, 139, 122, 102, 97, 105, 82, 92, 74, 75, 90, 87, 94, 93, 91, 91, 83, 93, 97, 89, 101, 96, 100, 104, 90, 89, 103,
                           82, 85, 82, 91, 103, 103, 101, 98, 88, 109, 112, 93, 99, 87, 89, 78, 89, 65, 82, 81, 84, 92, 72, 68, 80, 86, 78, 79, 80,
                           84, 84, 84, 80, 84, 77, 83, 84, 82, 81, 78, 80, 81, 72, 74, 81, 66, 75, 68, 75, 85, 84, 76, 81, 78, 76, 100, 63
                          };
  sprintf(dummy_point, "B ");
  int i = 0, j = 0;
  for (j = 0; j < 288; j++) {
    while (dummy_point[i] != '\0') {
      i++;
    }
    sprintf(&dummy_point[i], "%d,", dummy_beat_data[j]);
  }
  sendSppMsg(dummy_point);
}
void sendProfile() {
  sendSppMsg("TeaWatch by SouzouSekkei2018 Group1");
}
/* Bluetoothできたコマンドをパースし，実行 */
void execCmd() {
  char str[100];
  int i = 0;
  recvSppMsg(str);
  while ((str[i] != '\0') && (str[i] != '\n')) {
    if (str[i] == '@') {
      switch (str[i + 1]) {
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
          parseHbData(&str[i + 2]);
          break;
        case 'D':
          send_dummy_beat();
          break;
      }
    }
    i++;
  }
}
/* 親機に人間バッテリー値送信要求 */
void callHumansBattery() {
  sendSppMsg("@h");
}
/* 人間バッテリー値を親機から受け取り */
void parseHbData(char * str) {
  int i = 0;
  int hbattery = 0;
  while ((str[i] != '\0') && (str[i] != '\n')) {
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
int getHumansBattery(void) {
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
int Atoi(char a) {
  return (a - '0');
}
