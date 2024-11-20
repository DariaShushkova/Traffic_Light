int buttonPin = 5;
int trafficRedLedPin = 13;
int trafficYellowLedPin = 12;
int trafficGreenLedPin = 11;

#define RED 0
#define RED_YELLOW 1
#define GREEN 2
#define YELLOW 3

class TrafficLight {
  private:
    int currentState;
    unsigned long previousTime = 0;
    bool pedestrianWalk = false;
    bool messageSent = false;
    int timeGreen = 10000;
    int timeYellow = 5000;
    int timeRedYellow = 1000;
    int timeRed = 10000;

  public:
    TrafficLight() {
      currentState = RED;
    }

    void beginTraffic() {
      setTrafficLight(RED);
      previousTime = millis();
    }

    void setTrafficLight(int state) {
      currentState = state;
      digitalWrite(trafficRedLedPin, state == RED || state == RED_YELLOW ? HIGH : LOW);
      digitalWrite(trafficYellowLedPin, state == YELLOW || state == RED_YELLOW ? HIGH : LOW);
      digitalWrite(trafficGreenLedPin, state == GREEN ? HIGH : LOW);
    }

    void updateTrafficLight(unsigned long currentTime) {
      if (digitalRead(buttonPin) == LOW) {
          if (!pedestrianWalk) {
            pedestrianWalk = true;
            delay(100); // Debounce delay
          }
      }

      if (Serial.available() > 0) {
        int resumeTraffic = Serial.read();
        if (resumeTraffic == 200) { // Message 200 to resume the traffic received
          pedestrianWalk = false;
          setTrafficLight(RED_YELLOW); // Change state is allowed
          previousTime = currentTime;
        }
      }

      switch (currentState) {
        case GREEN:
          if (currentTime - previousTime >= timeGreen) {
            setTrafficLight(YELLOW);
            previousTime = currentTime;
          }
          break;
        case YELLOW:
          if (currentTime - previousTime >= timeYellow) {
            setTrafficLight(RED);
            previousTime = currentTime;
          }
          break;
        case RED:
          if (pedestrianWalk && !messageSent) {
            Serial.write(100);         // Send message 100 to allow pedestrian crossing
            messageSent = true;        // To prevent multiple messages
            previousTime = currentTime; // Reset RED timer for 10 seconds
          }
          if (!pedestrianWalk && currentTime - previousTime >= timeRed) {
            setTrafficLight(RED_YELLOW);
            previousTime = currentTime;
          }
          break;
        case RED_YELLOW:
          messageSent = false; // Reset the flag to allow to send the message 100 when in RED state
          if (currentTime - previousTime >= timeRedYellow) {
            setTrafficLight(GREEN);
            previousTime = currentTime;
          }
          break;
      }
    }
};

TrafficLight trafficLight;

void setup() {
  pinMode(trafficRedLedPin, OUTPUT);
  pinMode(trafficYellowLedPin, OUTPUT);
  pinMode(trafficGreenLedPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);  // Initialize serial communication
  trafficLight.beginTraffic();
}

void loop() {
  unsigned long currentTime = millis();
  trafficLight.updateTrafficLight(currentTime);
}
