#include <Arduino.h>
#include <Wire.h>
#include <math.h>

#include "mpu6500.h"
#include <U8x8lib.h>

#define I2C_SDA 8
#define I2C_SCL 9

#define ledPin 10

#define buttonPin 4

#define motorPin 3
#define motorChannel 0
#define motorFrequency 1000
#define motorResolution 8



// Complementary filter coefficient (0-1), higher = trust gyro more (smooth, less accel noise), lower = trust accel more (less drift)

static constexpr float ALPHA = 0.90f;

// IMU samples at 100 times per second which is more than enough for bicep curl (Original 1000Hz overkill)
static constexpr uint8_t SAMPLE_RATE_DIV = 9;
static constexpr float DT = (1.0f + SAMPLE_RATE_DIV) / 1000.0f;  

bfs::Mpu6500 mpu6500(&Wire, bfs::Mpu6500::I2C_ADDR_PRIM);
U8X8_SSD1306_128X64_NONAME_HW_I2C oled(U8X8_PIN_NONE);

float roll = 0.0f;
float pitch = 0.0f;

bool filterInitialized = false;
bool collecting = false;

int reps = 0;
int sets = 0;

int isPressed = 1;



void taskSampleIMU(void *pvParameters) {
  Serial.println("IMU task started, calling Begin()...");
  if (!mpu6500.Begin()) {
    Serial.println("Error starting IMU");
    vTaskDelete(NULL);
    return;
  }
  Serial.println("IMU initialized OK");

  mpu6500.ConfigAccelRange(bfs::Mpu6500::ACCEL_RANGE_4G);
  mpu6500.ConfigGyroRange(bfs::Mpu6500::GYRO_RANGE_500DPS);
  mpu6500.ConfigDlpfBandwidth(bfs::Mpu6500::DLPF_BANDWIDTH_20HZ);
  mpu6500.ConfigSrd(SAMPLE_RATE_DIV);

  Serial.println("timestamp_ms,roll,label");

  for (;;) {
    if (mpu6500.Read()) {
      float ax = mpu6500.accel_x_mps2();
      float ay = mpu6500.accel_y_mps2();
      float az = mpu6500.accel_z_mps2();

      float gx = mpu6500.gyro_x_radps();
      float gy = mpu6500.gyro_y_radps();
      float gz = mpu6500.gyro_z_radps();

      // Complementary filter; no yaw since not needed for bicep curls and no magnetometer to prevent drift

      float accelRoll = atan2f(ay, az) * (180.0f / M_PI);
      float accelPitch = atan2f(ax, sqrtf(sq(ay) + sq(az))) * (180.0f / M_PI);

      if (!filterInitialized) {
        roll = accelRoll;
        pitch = accelPitch;
        filterInitialized = true;
      } else {
        roll = ALPHA * (roll + gx * (180.0f / M_PI) * DT)
             + (1.0f - ALPHA) * accelRoll;
        pitch = ALPHA * (pitch + gy * (180.0f / M_PI) * DT)
              + (1.0f - ALPHA) * accelPitch;
      }

      // collect.py reads this to write to data.csv
      if (collecting) {
        int label = (isPressed == LOW) ? 1 : 0;
        Serial.print(millis());
        Serial.print(",");
        Serial.print(roll, 2);
        Serial.print(",");
        Serial.print(pitch, 2);
        Serial.print(",");
        Serial.println(label);
      }
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void taskDisplayOLED(void *pvParameters) {

  oled.begin();
  oled.setFlipMode(0);

  oled.setFont(u8x8_font_chroma48medium8_r);

  oled.inverse();
  oled.setCursor(2,0);
  oled.print("CURL TRACKER");
  oled.noInverse();

  for(;;) {
    oled.setCursor(0,2);
    oled.print("REPS:");
    oled.setCursor(5,2);
    oled.print(reps, 1);

    oled.setCursor(0,4);
    oled.print("SETS:");
    oled.setCursor(5,4);
    oled.print(sets, 1);

    oled.setCursor(0,6);
    oled.print("MQTT:OK");

    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void taskSampleButton(void *pvParameters) {

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  for (;;) {
    isPressed = digitalRead(buttonPin);


    if (isPressed == HIGH) {
      digitalWrite(ledPin, LOW);
    } else {
      digitalWrite(ledPin, HIGH);
    }

    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

// Haptic feedback for curls
void taskVibrateMotor(void *pvParamters) {

  ledcSetup(motorChannel, motorFrequency, motorResolution);
  ledcAttachPin(motorPin, motorChannel);
  ledcWrite(motorChannel, 0);

  for(;;) {
    if (isPressed == LOW) {
      ledcWrite(motorChannel, 220);
      vTaskDelay(pdMS_TO_TICKS(20));
      ledcWrite(motorChannel, 0);
    }
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

// Toggle for data collection
void taskSerialControl(void *pvParameters) {
  for (;;) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == 's') {
        collecting = !collecting;
        if (collecting) {
          Serial.println("Collecting start");
        } else {
          Serial.println("Collecting stop");
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000);
  xTaskCreate(taskSampleIMU, "sampleIMU", 2048, NULL, 1, NULL);
  xTaskCreate(taskDisplayOLED, "displayOLED", 2048, NULL, 1, NULL);
  xTaskCreate(taskSampleButton, "sampleButton", 2048, NULL, 1, NULL);
  xTaskCreate(taskVibrateMotor, "vibrateMotor", 2048, NULL, 1, NULL);
  xTaskCreate(taskSerialControl, "serialCtrl", 2048, NULL, 1, NULL);
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}

