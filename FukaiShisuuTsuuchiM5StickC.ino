/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Stick-C sample source code
*                          配套  M5Stick-C 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe：ENVII_SHT30_BMP280.  环境传感器
* date：2021/8/11
*******************************************************************************
  Please connect to Port,Read temperature, humidity and atmospheric pressure and display them on the display screen
  请连接端口,读取温度、湿度和大气压强并在显示屏上显示
*/

#include <M5StickC.h>
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
#include "UNIT_ENV.h"

// BME280
SHT3X sht30;
Adafruit_BMP280 bme;

float tmp = 0.0;
float hum = 0.0;
int fukaiShisuu = 0;

// SPEAKER
const int servo_pin = 26;
int freq = 50;
int ledChannel = 0;
int resolution = 10;
extern const unsigned char m5stack_startup_music[];

void playMusic(const uint8_t* music_data, uint16_t sample_rate) {
  uint32_t length = strlen((char*)music_data);
  uint16_t delay_interval = ((uint32_t)1000000 / sample_rate);
    for(int i = 0; i < length; i++) {
      ledcWriteTone(ledChannel, music_data[i]*50);
      delayMicroseconds(delay_interval);
    } 
}

void setup() {
  // 共通初期化
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextSize(2);

  // BME280
  Wire.begin(32, 33);

  //SPEAKER
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(servo_pin, ledChannel);
  ledcWrite(ledChannel, 256);//0°
  ledcWriteTone(ledChannel, 0);
}

void loop() {
  while (!bme.begin(0x76)){ //初始化bme传感器.  Init the sensor of bme
    M5.Lcd.println("Could not find a valid BMP280 sensor, check wiring!");
  }
  sht30.get();  //Obtain the data of shT30.  获取sht30的数据
  tmp = sht30.cTemp;  //Store the temperature obtained from shT30.  将sht30获取到的温度存储
  hum = sht30.humidity; //Store the humidity obtained from the SHT30.  将sht30获取到的湿度存储
  fukaiShisuu = (0.81 * tmp) + (0.01 * hum * (0.99 * tmp - 14.3)) + 46.3;
  
  M5.lcd.setCursor(0,0);
  M5.Lcd.fillScreen(TFT_BLACK);
  if(fukaiShisuu >= 80)
    M5.Lcd.setTextColor(TFT_RED);
  else
    M5.Lcd.setTextColor(TFT_WHITE);
  
  M5.Lcd.printf("Temp: %2.1f  \r\nHumi: %2.0f%%  \r\nFukai: %d", tmp, hum,fukaiShisuu);

  if(fukaiShisuu >= 80){
    ledcWriteTone(ledChannel, 1250);
    delay(1000);
    ledcWriteTone(ledChannel, 0);
    delay(1000);
  }
  
  delay(2000);
}
