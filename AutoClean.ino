// Pin Definitions
#define TRIG_PIN 9
#define ECHO_PIN 8
#define MQ135_PIN A0
#define MQ4_PIN A1
#define MOTOR 11
#define BUZZER 10
#define RED_LED 12
#define GREEN_LED 13

// Thresholds (adjust after calibration)
#define DISTANCE_THRESHOLD 0   // cm, for human presence
#define MQ135_THRESHOLD 600
#define MQ4_THRESHOLD  300

// Variables
long duration;
int distance;
int mq135Value;
int mq4Value;

void setup() {
  Serial.begin(9600);

  // Gas Sensors
  pinMode(MQ135_PIN, INPUT);
  pinMode(MQ4_PIN, INPUT);

  // Ultrasonic Sensor Pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Outputs
  pinMode(MOTOR, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);


  // Initialize outputs
  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(MOTOR, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BUZZER, LOW);
}

void loop() {
  distance = getDistance();
  mq135Value = analogRead(MQ135_PIN);
  mq4Value = analogRead(MQ4_PIN);

  Serial.print("Distance: "); Serial.print(distance); Serial.print(" cm\t");
  Serial.print("MQ135: "); Serial.print(mq135Value); Serial.print("\t");
  Serial.print("MQ4: "); Serial.println(mq4Value);

  if (distance < DISTANCE_THRESHOLD) {
    // 🚻 Human present → do not clean
    digitalWrite(GREEN_LED,HIGH );
    digitalWrite(RED_LED, LOW);
    digitalWrite(MOTOR, LOW);
    digitalWrite(BUZZER, LOW);
  } else {
    // 🚫 No human → check gases
    digitalWrite(GREEN_LED, LOW);

    if (mq4Value > MQ4_THRESHOLD) {
      // ⚠ Abnormal gas detected → Buzzer ON
      digitalWrite(BUZZER, HIGH);
    } else {
      digitalWrite(BUZZER, LOW);
    }

    if (mq135Value > MQ135_THRESHOLD || mq4Value > MQ4_THRESHOLD) {
      // 🚿 Cleaning process → Motor ON, Red LED ON
      digitalWrite(MOTOR, HIGH);
      digitalWrite(RED_LED, HIGH);
    } else {
      // ✅ Safe → Green LED ON
      digitalWrite(MOTOR, LOW);
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(BUZZER, LOW);
    }
  }

  delay(1000); // 1 second loop delay
}

// Function to read distance from HC-SR04
int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  int dist = duration * 0.034 / 2; // convert to cm
  return dist;
}
