
#define trafficRedLedPin 13 // Pin for the Traffic RED LED
#define trafficYellowLedPin 12 // Pin for the Traffic YELLOW LED
#define trafficGreenLedPin 11 // Pin for the Traffic GREEN LED
#define pedestrianRedLedPin 6 // Pin for the Pedestrian RED LED
#define pedestrianGreenLedPin 7 // Pin for the Pedestrian GREEN LED
#define buttonPin 5 // Pin for the BUTTON

class TrafficLight {
private:
    int timeRed;
    int timeRedYellow;
    int timeYellow;
    int timeGreen;
    unsigned long previousTime = 0;  // To store the last time a state change happened
    bool redStateStarted = false;  // Flag to track if TrafficLight just entered RED
    enum State { RED, RED_YELLOW, GREEN, YELLOW } currentState;

public:
    TrafficLight(int timeRed = 10, int timeRedYellow = 1, int timeGreen = 10, int timeYellow = 5) :
            timeRed(timeRed), timeRedYellow(timeRedYellow), timeGreen(timeGreen), timeYellow(timeYellow) {
        currentState = RED;
    }

    int getTimeRed() {
        return timeRed;
    }
    int getTimeRedYellow() {
        return timeRedYellow;
    }
    int getTimeYellow() {
        return timeYellow;
    }
    int getTimeGreen() {
        return timeGreen;
    }
    State getCurrentState() {
        return currentState;
    }

    void setTimeRed(int newTimeRed) {
        timeRed = newTimeRed;
    }
    void setTimeRedYellow(int newTimeRedYellow) {
        timeRedYellow = newTimeRedYellow;
    }
    void setTimeYellow(int newTimeYellow) {
        timeYellow = newTimeYellow;
    }
    void setTimeGreen(int newTimeGreen) {
        timeGreen = newTimeGreen;
    }

    // This function controls the traffic light state changes based on the current state and time
    void startTrafficLight(unsigned long currentTime, bool resetRedTime) {
        if (resetRedTime && currentState == RED && redStateStarted) {
            previousTime = currentTime;  // Reset RED time only if it is already in RED state
            redStateStarted = false;     // Reset entry flag
        }
            
        switch (currentState) {
            case RED:
                digitalWrite(trafficRedLedPin, HIGH);
                digitalWrite(trafficYellowLedPin, LOW);
                digitalWrite(trafficGreenLedPin, LOW);
                
                if (!redStateStarted) {
                    redStateStarted = true;   // Set entry flag only once per RED cycle
                    previousTime = currentTime;  // Start RED time on first RED entry
                }
                    
                // Transition to the next state after timeRed duration
                if (currentTime - previousTime >= timeRed*1000) {
                    currentState = RED_YELLOW;
                    previousTime = currentTime;
                    redStateStarted = false;  // Reset flag when leaving RED state
                }
                break;

            case RED_YELLOW:
                digitalWrite(trafficRedLedPin, HIGH);
                digitalWrite(trafficYellowLedPin, HIGH);
                digitalWrite(trafficGreenLedPin, LOW);

                // Transition to the next state after timeRedYellow duration
                if (currentTime - previousTime >= timeRedYellow*1000) {
                    currentState = GREEN;
                    previousTime = currentTime;
                }
                break;

            case GREEN:
                digitalWrite(trafficRedLedPin, LOW);
                digitalWrite(trafficYellowLedPin, LOW);
                digitalWrite(trafficGreenLedPin, HIGH);

                // Transition to the next state after timeGreen duration
                if (currentTime - previousTime >= timeGreen*1000) {
                    currentState = YELLOW;
                    previousTime = currentTime;
                }
                break;

            case YELLOW:
                digitalWrite(trafficRedLedPin, LOW);
                digitalWrite(trafficYellowLedPin, HIGH);
                digitalWrite(trafficGreenLedPin, LOW);

                // Transition to the next state after timeYellow duration
                if (currentTime - previousTime >= timeYellow*1000) {
                    currentState = RED;
                    previousTime = currentTime;
                }
                break;
        }
    }
};

class PedestrianLight {
private:
    TrafficLight &trafficLight;
    bool requestForGreen = false;
    bool pedestrianIsGreen = false;
    unsigned long buttonPressTime = 0;
    unsigned long debounceDelay = 50; // To avoid multiple triggers within a short time
public:
    PedestrianLight(TrafficLight &trafficLight) : trafficLight(trafficLight) {}

    bool activateButtonPress(unsigned long currentTime, int buttonState) {
        if (buttonState == LOW && (currentTime - buttonPressTime > debounceDelay)) {
            buttonPressTime = currentTime; // Update buttonPressTime to currentTime to start a new debounce period.
            requestForGreen = true; // Activate command to change the pedestrian light to green.
            return (trafficLight.getCurrentState() == 0);  // True if TrafficLight is RED, PedestrianLight can turn GREEN only when TrafficLight is RED
        }
        return false; // No reset is needed
    }
    
    void changePedestrianLight() {
        if (trafficLight.getCurrentState() == 0 && requestForGreen) {
            pedestrianIsGreen = true;
            digitalWrite(pedestrianRedLedPin, LOW);
            digitalWrite(pedestrianGreenLedPin, HIGH);
            requestForGreen = false;
        } else if (trafficLight.getCurrentState() != 0) {
            pedestrianIsGreen = false;
            digitalWrite(pedestrianRedLedPin, HIGH);
            digitalWrite(pedestrianGreenLedPin, LOW);
        }
    }
};

TrafficLight trafficLight;
PedestrianLight pedestrianLight(trafficLight);

void setup() {
    // Initialize pins as OUTPUT
    pinMode(trafficRedLedPin, OUTPUT);
    pinMode(trafficYellowLedPin, OUTPUT);
    pinMode(trafficGreenLedPin, OUTPUT);
    pinMode(pedestrianRedLedPin, OUTPUT);
    pinMode(pedestrianGreenLedPin, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);

    digitalWrite(pedestrianRedLedPin, HIGH);
    digitalWrite(pedestrianGreenLedPin, LOW);
}

void loop() {
    unsigned long currentTime = millis();  // Get the current time in milliseconds
    int buttonState = digitalRead(buttonPin); // Check if button is pressed
    
    bool resetRedTime = pedestrianLight.activateButtonPress(currentTime, buttonState); // Check if reset of RED time is needed

    // Call TrafficLight to update the state
    trafficLight.startTrafficLight(currentTime, resetRedTime);
    // Call PedestrianLight to update the state
    pedestrianLight.changePedestrianLight();
}
