int pedestrianRedLedPin = 6;
int pedestrianGreenLedPin = 7;

#define RED 0
#define GREEN 1

class PedestrianTrafficLight {
  public:
    int currentState;
    unsigned long previousTime;
    int crossingDuration;

  public:
    PedestrianTrafficLight() : currentState(RED), previousTime(0), crossingDuration(10000) {}

    void beginPedestrian() {
      pinMode(pedestrianRedLedPin, OUTPUT);
      pinMode(pedestrianGreenLedPin, OUTPUT);
      setLight(RED);
    }

    void updatePedestrian(unsigned long currentTime) {
      if (Serial.available() > 0) {
        int command = Serial.read();
        if (command == 100) { // Message 100 request to walk received
          setLight(GREEN); // Change state is allowed
          previousTime = currentTime;
        }
      }

      if (currentState == GREEN && currentTime - previousTime >= crossingDuration) {
        setLight(RED);
        Serial.write(200); // Send message 200 to resume the traffic
      }
    }

  private:
    void setLight(int state) {
      currentState = state;
      digitalWrite(pedestrianRedLedPin, state == RED ? HIGH : LOW);
      digitalWrite(pedestrianGreenLedPin, state == GREEN ? HIGH : LOW);
    }
};

PedestrianTrafficLight pedestrianLight;

void setup() {
  Serial.begin(9600);  // Initialize serial communication (pins 0 and 1)
  pedestrianLight.beginPedestrian();
}

void loop() {
  unsigned long currentTime = millis();
  pedestrianLight.updatePedestrian(currentTime);
}
