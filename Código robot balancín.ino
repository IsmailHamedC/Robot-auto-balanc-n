#include <Wire.h>

// Pines del motor A
const int ENA = 5;
const int IN1 = 3;
const int IN2 = 4;

// Pines del motor B
const int ENB = 6;
const int IN3 = 7;
const int IN4 = 8;

const int ADXL_ADDR = 0x53; // Dirección I2C del ADXL345

int16_t ax, ay, az;
float pitch = 0.0;

// Filtro de promedio móvil
const int FILTER_SIZE = 5;
float pitch_buffer[FILTER_SIZE];
int buffer_index = 0;

// PID variables
float setpoint = 0.0;  // Se calibrará automáticamente
float Kp = 25.0;       // Ganancia proporcional
float Ki = 2.0;       // Ganancia integral
float Kd = 6.0;        // Ganancia derivativa
float integral = 0.0;
float prev_error = 0.0;
unsigned long lastTime;
float dt;
const float INTEGRAL_LIMIT = 50.0;
const int MOTOR_THRESHOLD = 50;

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Wire.begin();
  Serial.begin(9600);

  Wire.beginTransmission(ADXL_ADDR);
  Wire.write(0x2D);
  Wire.write(0x08);
  Wire.endTransmission(true);

  Wire.beginTransmission(ADXL_ADDR);
  Wire.write(0x31);
  Wire.write(0x00);
  Wire.endTransmission(true);

  for (int i = 0; i < FILTER_SIZE; i++) {
    pitch_buffer[i] = 0.0;
  }

  Serial.println("Calibrando... Mantén el robot vertical y quieto por 5 segundos.");
  float sum_pitch = 0.0;
  const int samples = 100;
  for (int i = 0; i < samples; i++) {
    readADXL345();
    float ax_f = (float)ax / 256.0;
    float ay_f = (float)ay / 256.0;
    float az_f = (float)az / 256.0;
    float accel_pitch = atan2(-ax_f, sqrt(ay_f * ay_f + az_f * az_f)) * 180.0 / PI;
    sum_pitch += accel_pitch;
    delay(50);
  }
  setpoint = sum_pitch / samples;
  
  Serial.print("Calibración completa. Setpoint: ");
  Serial.println(setpoint, 2);

  lastTime = millis();
  motorParar('A');
  motorParar('B');
}

void loop() {
  readADXL345();

  unsigned long now = millis();
  dt = (now - lastTime) / 1000.0;
  lastTime = now;

  float ax_f = (float)ax / 256.0;
  float ay_f = (float)ay / 256.0;
  float az_f = (float)az / 256.0;
  float accel_pitch = atan2(-ax_f, sqrt(ay_f * ay_f + az_f * az_f)) * 180.0 / PI;

  pitch_buffer[buffer_index] = accel_pitch;
  buffer_index = (buffer_index + 1) % FILTER_SIZE;
  float filtered_pitch = 0.0;
  for (int i = 0; i < FILTER_SIZE; i++) {
    filtered_pitch += pitch_buffer[i];
  }
  pitch = filtered_pitch / FILTER_SIZE;

  float error = pitch - setpoint;
  integral += error * dt;
  if (integral > INTEGRAL_LIMIT) integral = INTEGRAL_LIMIT;
  if (integral < -INTEGRAL_LIMIT) integral = -INTEGRAL_LIMIT;
  float derivative = (error - prev_error) / dt;
  float output = Kp * error + Ki * integral + Kd * derivative;
  prev_error = error;

  if (output > 255) output = 255;
  if (output < -255) output = -255;

  int speed = abs(output);
  if (speed < MOTOR_THRESHOLD) speed = 0;
  if (output > 0) {
    motorAdelante(speed, 'A');
    motorAdelante(speed, 'B');
  } else if (output < 0) {
    motorAtras(speed, 'A');
    motorAtras(speed, 'B');
  } else {
    motorParar('A');
    motorParar('B');
  }

  Serial.print("Ax: "); Serial.print(ax_f, 2);
  Serial.print(" | Ay: "); Serial.print(ay_f, 2);
  Serial.print(" | Az: "); Serial.print(az_f, 2);
  Serial.print(" | Pitch: "); Serial.print(pitch, 2);
  Serial.print(" | Error: "); Serial.print(error, 2);
  Serial.print(" | PID Output: "); Serial.println(output, 2);

  delay(10);
}

void readADXL345() {
  Wire.beginTransmission(ADXL_ADDR);
  Wire.write(0x32);
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL_ADDR, 6, true);
  ax = Wire.read() | (Wire.read() << 8);
  ay = Wire.read() | (Wire.read() << 8);
  az = Wire.read() | (Wire.read() << 8);
}

void motorAdelante(int velocidad, char motor) {
  if (motor == 'A') {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, velocidad);
  } else if (motor == 'B') {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, velocidad);
  }
}

void motorAtras(int velocidad, char motor) {
  if (motor == 'A') {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, velocidad);
  } else if (motor == 'B') {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, velocidad);
  }
}

void motorParar(char motor) {
  if (motor == 'A') {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
  } else if (motor == 'B') {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 0);
  }
}
