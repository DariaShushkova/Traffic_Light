int pedestrianRedLedPin = 6;
int pedestrianGreenLedPin = 7;

#define RED 0
#define GREEN 1

class PedestrianTrafficLight {
  private:
    int currentState;
    unsigned long previousTime;
    int crossingDuration;

  public:
    PedestrianTrafficLight() : currentState(RED), previousTime(0), crossingDuration(10000) {}

    void beginPedestrian() {
      setLight(RED);
    }

    void setLight(int state) {
      currentState = state;
      digitalWrite(pedestrianRedLedPin, state == RED ? HIGH : LOW);
      digitalWrite(pedestrianGreenLedPin, state == GREEN ? HIGH : LOW);
    }

    void updatePedestrian(unsigned long currentTime) {
      if (Serial.available() > 0) {
        int command = Serial.read();
        if (command == 100) { // Request to walk pedestrians
          if (currentState == RED) {
            setLight(GREEN);
            previousTime = currentTime;
          }
        }
      }

      if (currentState == GREEN && currentTime - previousTime >= crossingDuration) {
        setLight(RED);
        Serial.write(200); // traffic lights to resume
        previousTime = currentTime;
      }
    }
};

PedestrianTrafficLight pedestrianLight;

void setup() {
  pinMode(pedestrianRedLedPin, OUTPUT);
  pinMode(pedestrianGreenLedPin, OUTPUT);
  Serial.begin(9600);
  pedestrianLight.beginPedestrian();
}

void loop() {
  unsigned long currentTime = millis();
  pedestrianLight.updatePedestrian(currentTime);
}
