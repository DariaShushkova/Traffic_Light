int buttonPin = 5;
int d1d2RedLedPin = 13;
int d1d2YellowLedPin = 12;
int d1d2GreenLedPin = 11;
int d3RedLedPin = 4;
int d3YellowLedPin = 3;
int d3GreenLedPin = 2;

#define RED 0
#define RED_YELLOW 1
#define GREEN 2
#define YELLOW 3

class TrafficLightD1D2 {
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
    TrafficLightD1D2() {
      currentState = RED;
    }

    bool isRed() {
      return currentState == RED;
    }

    void beginTraffic() {
      setTrafficLight(RED);
      previousTime = millis();
    }

    void setTrafficLight(int state) {
      currentState = state;
      digitalWrite(d1d2RedLedPin, state == RED || state == RED_YELLOW ? HIGH : LOW);
      digitalWrite(d1d2YellowLedPin, state == YELLOW || state == RED_YELLOW ? HIGH : LOW);
      digitalWrite(d1d2GreenLedPin, state == GREEN ? HIGH : LOW);
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

class TrafficLightD3 {
  private:
    int currentState;
    unsigned long previousTime = 0;
    TrafficLightD1D2 &trafficLight;
    int timeGreen = 10000;
    int timeYellow = 5000;
    int timeRedYellow = 1000;
    int timeRed = 10000;

  public:
    TrafficLightD3(TrafficLightD1D2 &trafficLight): trafficLight(trafficLight) {
      currentState = GREEN;
    }

    void beginTraffic() {
      setTrafficLight(GREEN);
      previousTime = millis();
    }

    void setTrafficLight(int state) {
      currentState = state;
      digitalWrite(d3RedLedPin, state == RED || state == RED_YELLOW ? HIGH : LOW);
      digitalWrite(d3YellowLedPin, state == YELLOW || state == RED_YELLOW ? HIGH : LOW);
      digitalWrite(d3GreenLedPin, state == GREEN ? HIGH : LOW);
    }

    void updateTrafficLight(unsigned long currentTime) {
      if (trafficLight.isRed()) {
        if (currentState != GREEN) {
          setTrafficLight(GREEN);
          previousTime = currentTime;
        }
      } else {
        if (currentState == GREEN) {
          setTrafficLight(YELLOW);
          previousTime = currentTime;
        }
        switch (currentState) {
          case RED:
            if (currentTime - previousTime >= timeRed) {
              setTrafficLight(RED_YELLOW);
              previousTime = currentTime;
            }
            break;
          case YELLOW:
            if (currentTime - previousTime >= timeYellow) {
              setTrafficLight(RED);
              previousTime = currentTime;
            }
            break;
          case RED_YELLOW:
            if (currentTime - previousTime >= timeRedYellow) {
              setTrafficLight(GREEN);
              previousTime = currentTime;
            }
            break;
        }
      }
    }
};

TrafficLightD1D2 trafficLightD1D2;
TrafficLightD3 trafficLightD3(trafficLightD1D2);

void setup() {
  pinMode(d1d2RedLedPin, OUTPUT);
  pinMode(d1d2YellowLedPin, OUTPUT);
  pinMode(d1d2GreenLedPin, OUTPUT);
  pinMode(d3RedLedPin, OUTPUT);
  pinMode(d3YellowLedPin, OUTPUT);
  pinMode(d3GreenLedPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
  trafficLightD1D2.beginTraffic();
  trafficLightD3.beginTraffic();
}

void loop() {
  unsigned long currentTime = millis();
  trafficLightD1D2.updateTrafficLights(currentTime);
  trafficLightD3.updateTrafficLight(currentTime);
}
