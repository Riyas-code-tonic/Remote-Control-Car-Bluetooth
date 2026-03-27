#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
  #error "Bluetooth is not enabled!"
#endif

BluetoothSerial SerialBT;

#define MOTOR_A_IN1   27
#define MOTOR_A_IN2   26
#define MOTOR_A_EN    14

#define MOTOR_B_IN3   25
#define MOTOR_B_IN4   33
#define MOTOR_B_EN    12

#define MOTOR_SPEED   200
#define PWM_FREQ      5000
#define PWM_RES       8

bool btConnected = false;

void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void stopMotors();

void btCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    Serial.println(">> Client Connected!");
    btConnected = true;
  }
  if (event == ESP_SPP_CLOSE_EVT) {
    Serial.println(">> Client Disconnected!");
    btConnected = false;
    stopMotors();
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  SerialBT.register_callback(btCallback);
  SerialBT.begin("ESP32_RC_Car");
  Serial.println("=== BT Ready ===");

  pinMode(MOTOR_A_IN1, OUTPUT);
  pinMode(MOTOR_A_IN2, OUTPUT);
  pinMode(MOTOR_B_IN3, OUTPUT);
  pinMode(MOTOR_B_IN4, OUTPUT);

  ledcAttach(MOTOR_A_EN, PWM_FREQ, PWM_RES);
  ledcAttach(MOTOR_B_EN, PWM_FREQ, PWM_RES);

  stopMotors();
}

void loop() {
  if (SerialBT.available()) {
    char cmd = SerialBT.read();
    Serial.print("Received: [");
    Serial.print(cmd);
    Serial.print("] ASCII: ");
    Serial.println((int)cmd);      // keep this during testing so you can see all button chars

    switch (cmd) {
      case 'w': moveForward();  Serial.println(">> Forward");  break;
      case 's': moveBackward(); Serial.println(">> Backward"); break;
      case 'a': turnLeft();     Serial.println(">> Left");     break;
      case 'd': turnRight();    Serial.println(">> Right");    break;
      case 'x': stopMotors();   Serial.println(">> Stop");     break;
      default:  break;
    }
  }
}

void moveForward() {
  digitalWrite(MOTOR_A_IN1, HIGH); 
  digitalWrite(MOTOR_A_IN2, LOW);
  digitalWrite(MOTOR_B_IN3, HIGH); 
  digitalWrite(MOTOR_B_IN4, LOW);
  ledcWrite(MOTOR_A_EN, MOTOR_SPEED);
  ledcWrite(MOTOR_B_EN, MOTOR_SPEED);
}
void moveBackward() {
  digitalWrite(MOTOR_A_IN1, LOW);  
  digitalWrite(MOTOR_A_IN2, HIGH);
  digitalWrite(MOTOR_B_IN3, LOW);  
  digitalWrite(MOTOR_B_IN4, HIGH);
  ledcWrite(MOTOR_A_EN, MOTOR_SPEED);
  ledcWrite(MOTOR_B_EN, MOTOR_SPEED);
}
void turnLeft() {
  digitalWrite(MOTOR_A_IN1, LOW);  
  digitalWrite(MOTOR_A_IN2, HIGH);
  digitalWrite(MOTOR_B_IN3, HIGH); 
  digitalWrite(MOTOR_B_IN4, LOW);
  ledcWrite(MOTOR_A_EN, MOTOR_SPEED);
  ledcWrite(MOTOR_B_EN, MOTOR_SPEED);
}
void turnRight() {
  digitalWrite(MOTOR_A_IN1, HIGH); 
  digitalWrite(MOTOR_A_IN2, LOW);
  digitalWrite(MOTOR_B_IN3, LOW);  
  digitalWrite(MOTOR_B_IN4, HIGH);
  ledcWrite(MOTOR_A_EN, MOTOR_SPEED);
  ledcWrite(MOTOR_B_EN, MOTOR_SPEED);
}
void stopMotors() {
  digitalWrite(MOTOR_A_IN1, LOW); 
  digitalWrite(MOTOR_A_IN2, LOW);
  digitalWrite(MOTOR_B_IN3, LOW); 
  digitalWrite(MOTOR_B_IN4, LOW);
  ledcWrite(MOTOR_A_EN, 0);
  ledcWrite(MOTOR_B_EN, 0);
}
