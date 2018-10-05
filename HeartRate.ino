/* 【心拍センサ関連処理】 */
#include "MAX30105.h"  /* 心拍センサライブラリ */
#include "heartRate.h" /* 心拍測定ライブラリ */

/* ①心拍センサ */
MAX30105 particleSensor;
#define SENSORS_ADDRESS 0xAE /* このライブラリではアドレスは指定しなくていいらしい */

/* sensorSetup : センサの初期化設定 */
void sensorSetup(){
  while(i2cSemaphoreP() == 0); /* i2cセマフォ獲得できるまで待つ */
  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    //while (1); つながってるのにnot foundになることがあって、その場合↑をコメントアウトすると動く
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0xFF); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0xFF); //Turn off Green LED
  i2cSemaphoreV(); /* i2cセマフォ解放 */
}

/* measureHeartRate : 現在の心拍数を測定 */
long measureHeartRate(){
  while(i2cSemaphoreP() == 0); /* i2cセマフォ獲得できるまで待つ */
  long irValue = particleSensor.getIR();
  Serial.println(irValue);
  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 50)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }
  i2cSemaphoreV(); /* i2cセマフォ解放 */
  return irValue;
}

int getBeatAvg(){
  return beatAvg;
}


