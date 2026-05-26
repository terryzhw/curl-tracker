#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>
#include <U8g2lib.h>

// SSD1306 128x64 OLED via I2C (SDA=21, SCL=22)
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

MPU6050 mpu;

#define MOTOR_PIN   25
#define MOTOR_CH    0       // LEDC channel
#define MOTOR_FREQ  1000    // Hz
#define MOTOR_RES   8       // 8-bit resolution (0-255)


unsigned long prevTime = 0;
const float ALPHA = 0.90;
float degrees = 100.0;
int rep = 0; 

enum ArmState {
    ARM_DOWN,
    ARM_UP
};

// Vibrate at given strength (0=off, 255=full) for durationMs
void vibrate(uint8_t strength, uint32_t durationMs) {
    ledcWrite(MOTOR_CH, strength);
    delay(durationMs);
    ledcWrite(MOTOR_CH, 0);
}


void setup() {
    Serial.begin(115200);
    Wire.begin();

    prevTime = micros();

    // Motor PWM setup
    ledcSetup(MOTOR_CH, MOTOR_FREQ, MOTOR_RES);

    ledcAttachPin(MOTOR_PIN, MOTOR_CH);
    ledcWrite(MOTOR_CH, 0);

    u8g2.begin();
    u8g2.setFont(u8g2_font_5x7_tr);

    mpu.initialize();
    if (!mpu.testConnection()) {
        u8g2.clearBuffer();
        u8g2.drawStr(0, 10, "MPU6050 FAIL");
        u8g2.sendBuffer();
        while (true);
    }

}

void loop() {
    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // Convert raw to g and degs
    float axG = ax / 16384.0f;
    float ayG = ay / 16384.0f;
    float azG = az / 16384.0f;
    float gyD = gy / 131.0f;

    // Complementary filter
    float accelPitch = atan2(axG, azG) * (180.0f/PI);
    unsigned long currTime = micros();
    float dt = (currTime - prevTime) / 1000000.0f;
    prevTime = currTime;
    degrees = (ALPHA * (degrees + gyD * dt) + (1-ALPHA) * accelPitch);

    char buf[32];
    u8g2.clearBuffer();
    u8g2.drawStr(2, 24, "Degrees: ");
    snprintf(buf, sizeof(buf), "%.1f", degrees);
    u8g2.drawStr(2, 34, buf);
    u8g2.sendBuffer();

    // magnitude of accelerometer 
    float speed = sqrt(axG*axG + ayG*ayG + azG*azG);                                                  
    if (speed > 2.0f) {                                                                               
      vibrate(220, 80); 
    }

}
