/*181018 関数追加　勝又将斗*/

/*変数定義*/
double Average_beat;
double beat;
unsigned int time_count_5min = 300000;
unsigned int time_count_5s = 5000;
/*キュー関数　作成*/
double queue[288];
int head_queue = 0;
int tail_queue = 0;
double *get_Average_beat = 0;

void calucAvg() {
  /*心拍を5秒ごとに計測し、それを代入*/
  if (millis() >= time_count_5s) {
    beat += getBeatAvg(); /*心拍関数から読み取る*/
    //if (beat < 40 || beat >250/*心拍が異常なら*/)count++; /*countの数が20以上になればその五分間のデータはむこうにしたりする予定*/
    Serial.println("TIME COUNT NOW!!");
    Serial.println(time_count_5s, DEC);
    Serial.println(millis(), DEC);

    /*心拍を五分に一回平均を計算*/
    if (millis() >= time_count_5min) {
      Serial.println("TIME COUNT NOW!!");
      Serial.println(time_count_5min, DEC);
      Serial.println(millis(), DEC);

      Average_beat = beat / 60; /*5min×5s=60*/
      enqueue(Average_beat);/*キュー（first in first out)に代入*/
      Average_beat = 0;
      beat = 0;
      time_count_5min += 1000 * 5 * 60;

    }
    time_count_5s += 5000; /*5s*/
  }
}

/*queueに入れる*/
void enqueue(double Average_beat) {
  //  if (tail_queue >= 288) return false; /*キュー容量の限界*/
  queue[tail_queue] = Average_beat;
  tail_queue++;
}

/*queueから取り出す*/
void dequeue() {
  //if (head_queue >= 288) return false; /*キュー容量の限界*/
  *get_Average_beat = queue[head_queue];
  head_queue ++;
}

double display_queue(int i) {
  if (tail_queue < i) {
    return NAN;
  } else {
    return queue[i];
  }
}

