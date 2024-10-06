
#define redLedPin 13 // Pin for the RED LED
#define yellowLedPin 12 // Pin for the YELLOW LED
#define greenLedPin 11 // Pin for the GREEN LED

class TrafficLight {
private:
    int timeRed;
    int timeRedYellow;
    int timeYellow;
    int timeGreen;
    unsigned long previousTime = 0;  // To store the last time a state change happened
    enum State { RED, RED_YELLOW, GREEN, YELLOW } currentState;

public:
    TrafficLight(int timeRed = 10, int timeRedYellow = 1, int timeGreen = 10, int timeYellow = 3) :
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
    void startTrafficLight(unsigned long currentTime) {
        switch (currentState) {
            case RED:
                digitalWrite(redLedPin, HIGH);
                digitalWrite(yellowLedPin, LOW);
                digitalWrite(greenLedPin, LOW);

                // Transition to the next state after timeRed duration
                if (currentTime - previousTime >= timeRed*1000) {
                    currentState = RED_YELLOW;
                    previousTime = currentTime;
                }
                break;

            case RED_YELLOW:
                digitalWrite(redLedPin, HIGH);
                digitalWrite(yellowLedPin, HIGH);
                digitalWrite(greenLedPin, LOW);

                // Transition to the next state after timeRedYellow duration
                if (currentTime - previousTime >= timeRedYellow*1000) {
                    currentState = GREEN;
                    previousTime = currentTime;
                }
                break;

            case GREEN:
                digitalWrite(redLedPin, LOW);
                digitalWrite(yellowLedPin, LOW);
                digitalWrite(greenLedPin, HIGH);

                // Transition to the next state after timeGreen duration
                if (currentTime - previousTime >= timeGreen*1000) {
                    currentState = YELLOW;
                    previousTime = currentTime;
                }
                break;

            case YELLOW:
                digitalWrite(redLedPin, LOW);
                digitalWrite(yellowLedPin, HIGH);
                digitalWrite(greenLedPin, LOW);

                // Transition to the next state after timeYellow duration
                if (currentTime - previousTime >= timeYellow*1000) {
                    currentState = RED;
                    previousTime = currentTime;
                }
                break;
        }
    }
};

TrafficLight trafficLight;

void setup() {
    // Initialize the LED pins as OUTPUT
    pinMode(redLedPin, OUTPUT);
    pinMode(yellowLedPin, OUTPUT);
    pinMode(greenLedPin, OUTPUT);
}

void loop() {
    unsigned long currentTime = millis();  // Get the current time in milliseconds

    // Call the traffic light controller to update the state
    trafficLight.startTrafficLight(currentTime);
}
