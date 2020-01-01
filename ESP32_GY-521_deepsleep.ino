#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

#define XACC_OFFSET -3456
#define YACC_OFFSET 2838
#define ZACC_OFFSET 829
#define XGYRO_OFFSET 9
#define YGYRO_OFFSET 43
#define ZGYRO_OFFSET -24

#define INTERPRETER_PIN GPIO_NUM_13

MPU6050 accelgyro;
int16_t XAccel, YAccel, ZAccel, XGyro, YGyro, ZGyro;

void setoffset(MPU6050 mpu, int XAccel_offset, int YAccel_offset, int ZAccel_offset, int XGyro_offset, int YGyro_offset, int ZGyro_offset){
  mpu.setXAccelOffset(XAccel_offset);
  mpu.setYAccelOffset(YAccel_offset);
  mpu.setZAccelOffset(ZAccel_offset);
  mpu.setXGyroOffset(XGyro_offset);
  mpu.setYGyroOffset(YGyro_offset);
  mpu.setZGyroOffset(ZGyro_offset);
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(1000);
  
  accelgyro.initialize();
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  setoffset(accelgyro, XACC_OFFSET, YACC_OFFSET, ZACC_OFFSET, XGYRO_OFFSET, YGYRO_OFFSET, ZGYRO_OFFSET);

  // 動体検知のパラメータ指定
  accelgyro.setIntMotionEnabled(true);
  //accelgyro.setMotionDetectionThreshold(1); // 閾値以上の動作があったら動体検知とする関数
  accelgyro.setMotionDetectionDuration(100); // 閾値ms以上動いたら動体検知とする関数
  accelgyro.setInterruptMode(HIGH);

  esp_sleep_enable_ext0_wakeup(INTERPRETER_PIN, 0);

  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0){
    // ESP32通常起動時はこの処理が行われないが、ESP32deepsleep復帰時はここの処理が実行される。
  }

  esp_deep_sleep_start();
}
void loop() { }
