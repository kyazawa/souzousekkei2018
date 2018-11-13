/* バッテリー容量計算 2018.11.11 勝又 */

/* ★ 本プログラムとマージ 2018.11.13 矢澤 */

/*変数定義*/
unsigned int battery_RFC [][2] = {{0, 4159}, {5, 4101}, {10, 4080}, {15, 4057}, {20, 4038}, {25, 4026}, {30, 4015}, {35, 4005}, {40, 3981},
  {45, 3959}, {50, 3944}, {55, 3927}, {60, 3912}, {65, 3891}, {70, 3886}, {75, 3873}, {80, 3854}, {85, 3846}, {90, 3827}, {95, 3816},
  {100, 3811}, {105, 3799}, {110, 3795}, {115, 3792}, {120, 3785}, {125, 3775}, {130, 3768}, {135, 3761}, {140, 3753}, {145, 3746},
  {150, 3740}, {155, 3737}, {160, 3727}, {165, 3722}, {170, 3720}, {175, 3707}, {180, 3696}, {185, 3686}, {190, 3682}, {195, 3672},
  {200, 3656}, {205, 3649}, {210, 3645}, {215, 3643}, {220, 3604}, {225, 3593}, {230, 3550}, {235, 3485}, {240, 3324}, {245, 2965},
};
int percent_100;
unsigned int battery;
double new_ave, new_total = 0, pre_ave = 0;
//unsigned int battery_pre;

int Charge_battery() {
  int i;
  pre_ave = readBatteryVoltage();
  for (i = 0; i < 10; i++) {
    new_total += readBatteryVoltage();
  }
  new_ave = new_total / 10.0;
  if (new_ave - pre_ave > 80) {
    pre_ave = new_ave;
    return 1;
  } else {
    pre_ave = new_ave;
    return 0;
  }
}

int Convert_battery_to_percentage() {
  int i;
  int count;
  double tmp;
  battery = readBatteryVoltage();
  for (i = 0; i < 50; i++) {
    if (battery_RFC[i][1] > battery) count++;
    else break;
  }
  tmp = ((245 - battery_RFC[count][0]) / 245.0) * 100;
  percent_100 = tmp;
  count = 0;
  /* battery_pre = battery_new;
    if(battery_pre>battery_new){

    }*/
  return percent_100;
}

