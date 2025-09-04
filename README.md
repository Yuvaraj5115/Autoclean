\<div align="center"\>  
\<br /\>  
\<h1\>AutoClean: A Smart Guardian for Clean & Safe Air\</h1\>  
\<p\>  
A smart, DIY system that monitors air quality and automatically disinfects a space, but only when it's empty and safe to do so.  
\</p\>  
\<br /\>  
\<p\>  
\<img src="https://www.google.com/search?q=https://img.shields.io/badge/platform-ESP32-blue.svg" alt="Platform" /\>  
\<img src="https://www.google.com/search?q=https://img.shields.io/badge/status-complete-brightgreen.svg" alt="Status" /\>  
\<a href="https://www.google.com/search?q=https://github.com/GCY/wxECGAnalyzer/blob/master/LICENSE"\>\<img src="https://www.google.com/search?q=https://img.shields.io/badge/license-MIT-lightgrey.svg" alt="License" /\>\</a\>  
\</p\>  
\</div\>  
This project was built to answer a simple question: "Is this room *really* clean?" AutoClean is our solution—an autonomous system that ensures a space is not only fresh but also safe, by intelligently deciding when and how to clean.

## **Table of Contents**

* [Key Features](https://www.google.com/search?q=%23-key-features)  
* [How It Works: The Logic Flow](https://www.google.com/search?q=%23-how-it-works-the-logic-flow)  
* [Hardware Required](https://www.google.com/search?q=%23-hardware-required)  
* [Wiring & Connections](https://www.google.com/search?q=%23-wiring--connections)  
* [The Code](https://www.google.com/search?q=%23-the-code)  
* [Future Ideas](https://www.google.com/search?q=%23-future-ideas)  
* [License](https://www.google.com/search?q=%23license)

## **✨ Key Features**

| Feature | Status | Description |
| :---- | :---- | :---- |
| **Presence Detection** | ✅ | Uses an HC-SR04 ultrasonic sensor to ensure the room is empty before operating. Safety first\! |
| **Air Quality Monitoring** | ✅ | An MQ-135 sensor "sniffs" the air for general impurities like smoke and bad odors. |
| **Gas Leak Alert** | ✅ | An MQ-4 sensor provides a critical safety alert by detecting methane or CNG leaks. |
| **Automated Disinfection** | ✅ | A relay-controlled motor activates a disinfectant spray when the room is empty and the air quality is poor. |
| **Clear Visual Feedback** | ✅ | Uses Green and Red LEDs to signal "Safe" or "Cleaning" status, with a buzzer for gas alerts. |
| **Power Efficient** | ✅ | Designed to run from a single 3.7V Li-ion battery (with a recommended boost converter). |

## **🧠 How It Works: The Logic Flow**

The system's decision-making is designed to be simple and reliable. It continuously cycles through these steps:

1. **Scan for Presence**: The HC-SR04 sensor checks if anyone is in the room.  
   * **If a person is detected**: The system enters a safe, idle state. It turns the **Green LED** ON and pauses all other functions.  
   * **If the room is empty**: It proceeds to the next step.  
2. **Check the Air**: The MQ-135 and MQ-4 sensors analyze the air.  
   * **If a methane leak is detected** (MQ-4 \> threshold): The **Buzzer** sounds an immediate alarm. This is the highest priority.  
   * **If the general air quality is poor** (MQ-135 \> threshold): The system begins the cleaning cycle. It turns the **Red LED** ON and activates the **Motor** via the relay.  
   * **If the air is clean**: The system confirms the safe state by turning the **Green LED** ON.  
3. **Repeat**: The loop starts over, ensuring constant monitoring.

## **🛠️ Hardware Required**

Here’s a complete list of the components we used to build AutoClean.

| Component | Quantity | Purpose |
| :---- | :---- | :---- |
| ESP32 Dev Board | 1 | The brain of the operation |
| HC-SR04 Sensor | 1 | Ultrasonic presence detection |
| MQ-135 Sensor Module | 1 | General air quality sensing |
| MQ-4 Sensor Module | 1 | Methane gas detection |
| 5V Single Channel Relay | 1 | Switch for the spray motor |
| 3-6V DC Motor | 1 | Powers the disinfectant spray |
| LEDs (Red, Green) | 1 each | Visual status indicators |
| Piezo Buzzer | 1 | Audible alarm for gas alerts |
| 3.7V Li-ion Battery | 1 | Power Source |
| 5V Boost Converter | 1 | **Highly Recommended** for stable power |
| Resistors (220Ω) | 2 | Current limiting for the LEDs |
| Breadboard & Jumper Wires | Set | For assembly and connections |

## **🔌 Wiring & Connections**

Proper wiring is crucial for stable performance. We strongly recommend using a **5V boost converter** to step up the battery's 3.7V. This provides a clean 5V supply to the ESP32's VIN and the sensors, preventing unexpected brownouts, especially during Wi-Fi use.

* **Power Distribution**: The 3.7V battery connects to the boost converter's input. The converter's 5V output and GND then supply power to all other components.  
* **Common Ground**: Ensure all GND pins (ESP32, sensors, relay, motor, battery) are connected to a common ground rail.

### **Pinout Table**

| Component | Connects to ESP32 Pin | Notes |
| :---- | :---- | :---- |
| **HC-SR04** TRIG | GPIO 25 | A standard digital output pin. |
| **HC-SR04** ECHO | GPIO 26 | **Important:** Use a voltage divider (e.g., 1kΩ/2kΩ) to step down the 5V signal to a 3.3V level safe for the ESP32. |
| **MQ-135** A0 | GPIO 36 (ADC1\_0) | Analog pin for air quality reading. |
| **MQ-4** A0 | GPIO 39 (ADC1\_3) | Analog pin for methane gas reading. |
| **Relay** IN | GPIO 27 | Digital output to trigger the motor. |
| **Red LED** (+) | GPIO 32 | Connect in series with a 220Ω resistor. |
| **Green LED** (+) | GPIO 33 | Connect in series with a 220Ω resistor. |
| **Buzzer** (+) | GPIO 14 | Connect the other leg to GND. |

## **💻 The Code**

This is the complete Arduino sketch that runs the AutoClean system.

### **Quick Setup**

1. Ensure you have the **Arduino IDE** installed.  
2. Add the **ESP32 board manager** to your IDE. You can find many tutorials online for this.  
3. In the IDE, go to Tools \> Board and select your specific ESP32 board model.  
4. Select the correct COM port under Tools \> Port.  
5. Copy the code below, paste it into the IDE, and click Upload.

### **Arduino Sketch**

// Pin Definitions for our AutoClean project  
\#define TRIG\_PIN 25     // Ultrasonic sensor Trigger pin  
\#define ECHO\_PIN 26     // Ultrasonic sensor Echo pin  
\#define MQ135\_PIN 36    // Air Quality sensor Analog pin  
\#define MQ4\_PIN 39      // Methane Gas sensor Analog pin  
\#define RELAY\_PIN 27    // Relay module IN pin to control the motor

// Outputs for user feedback  
\#define RED\_LED 32      // Tells us when it's cleaning  
\#define GREEN\_LED 33    // Tells us everything is safe  
\#define BUZZER 14       // The alarm for gas leaks

// These are our thresholds. You'll want to tweak these for your specific room and sensors\!  
\#define DISTANCE\_THRESHOLD 50   // How close someone needs to be to be "present" (in cm)  
\#define MQ135\_THRESHOLD 400     // The analog value that we consider "bad air"  
\#define MQ4\_THRESHOLD 300       // The analog value for a methane alert

void setup() {  
  // Start up the serial monitor for debugging  
  Serial.begin(115200);

  // Set up our pins  
  pinMode(TRIG\_PIN, OUTPUT);  
  pinMode(ECHO\_PIN, INPUT);  
  pinMode(RELAY\_PIN, OUTPUT);  
  pinMode(RED\_LED, OUTPUT);  
  pinMode(GREEN\_LED, OUTPUT);  
  pinMode(BUZZER, OUTPUT);

  // Make sure everything is off when we start  
  digitalWrite(RELAY\_PIN, LOW);  
  digitalWrite(RED\_LED, LOW);  
  digitalWrite(GREEN\_LED, LOW);  
  digitalWrite(BUZZER, LOW);  
}

void loop() {  
  // First, let's get fresh readings from our sensors  
  int distance \= getDistance();  
  int mq135Value \= analogRead(MQ135\_PIN);  
  int mq4Value \= analogRead(MQ4\_PIN);

  // The most important rule: is someone in the room?  
  if (distance \< DISTANCE\_THRESHOLD) {  
    // If yes, stop everything and show the green "safe" light.  
    digitalWrite(GREEN\_LED, HIGH);  
    digitalWrite(RED\_LED, LOW);  
    digitalWrite(RELAY\_PIN, LOW);  
    digitalWrite(BUZZER, LOW);  
  } else {  
    // If no, the room is empty. Now we can check the air.  
    digitalWrite(GREEN\_LED, LOW); // Turn off the "safe" light for now

    // Is there a methane leak? This is priority \#1.  
    if (mq4Value \> MQ4\_THRESHOLD) {  
      digitalWrite(BUZZER, HIGH); // Sound the alarm\!  
    } else {  
      digitalWrite(BUZZER, LOW); // No leak, keep the buzzer off.  
    }  
      
    // Does the room need cleaning (bad air OR methane)?  
    if (mq135Value \> MQ135\_THRESHOLD || mq4Value \> MQ4\_THRESHOLD) {  
      // Time to clean\! Turn on the motor and the red light.  
      digitalWrite(RELAY\_PIN, HIGH);  
      digitalWrite(RED\_LED, HIGH);  
    } else {  
      // The room is empty AND the air is clean. Show the green light.  
      digitalWrite(RELAY\_PIN, LOW);  
      digitalWrite(RED\_LED, LOW);  
      digitalWrite(GREEN\_LED, HIGH);  
      digitalWrite(BUZZER, LOW); // Just in case.  
    }  
  }  
    
  // Wait a second before we check everything again.  
  delay(1000);  
}

// A little helper function to get the distance from the HC-SR04  
int getDistance() {  
  // Send out a little ultrasonic ping  
  digitalWrite(TRIG\_PIN, LOW);  
  delayMicroseconds(2);  
  digitalWrite(TRIG\_PIN, HIGH);  
  delayMicroseconds(10);  
  digitalWrite(TRIG\_PIN, LOW);  
    
  // Listen for the echo and calculate the distance  
  long duration \= pulseIn(ECHO\_PIN, HIGH);  
  return duration \* 0.034 / 2;  
}

## **🚀 Future Ideas**

This project is a great foundation, and we're already thinking about what could come next:

* **Get it Online:** Connect the ESP32 to Wi-Fi to send phone notifications about cleaning cycles or gas alerts.  
* **Track the Data:** Log air quality data over time to a cloud service to visualize trends.  
* **Smarter Logic:** Implement a timer so it only cleans during specific off-hours.  
* **Go Solar:** Add a small solar panel and battery management system to make it truly autonomous.

## **License**

This project is licensed under the MIT License. See the [LICENSE](https://www.google.com/search?q=LICENSE) file for details.