#include <SoftwareSerial.h>
#define BAUD_RATE 9600
#define IS_MASTER true    // Set to false when connecting to another system as slave

#define trafficRedLedPin 13 // Pin for the Traffic RED LED
#define trafficYellowLedPin 12 // Pin for the Traffic YELLOW LED
#define trafficGreenLedPin 11 // Pin for the Traffic GREEN LED
#define pedestrianRedLedPin 6 // Pin for the Pedestrian RED LED
#define pedestrianGreenLedPin 7 // Pin for the Pedestrian GREEN LED
#define buttonPin 5 // Pin for the BUTTON

class ExternalInterfaceBase {
public:
    virtual void sendButtonPress() = 0;
    virtual void sendRedStateSignal() = 0;
    virtual char checkIncomingSignal() = 0;
};

class MultiSerialInterface : public ExternalInterfaceBase {
private:
    static const char BUTTON_SIGNAL = 'B';  // Signal for button press
    static const char RED_ACK = 'R';        // Acknowledgment that system is in RED state
    static const char SYSTEM_ID = '1';      // Unique ID for this system (change for each Arduino: 1, 2, or 3)
    unsigned long lastSignalTime = 0;
    const unsigned long debounceTime = 100;  // Debounce for sending signals

    // Second Serial port for third Arduino
    SoftwareSerial SecondSerial(2, 3); // RX, TX pins for second serial connection

public:
    MultiSerialInterface() {
        Serial.begin(BAUD_RATE);
        SecondSerial.begin(BAUD_RATE);
    }

    void sendButtonPress() override {
        if (millis() - lastSignalTime > debounceTime) {
            char signal[3] = {SYSTEM_ID, BUTTON_SIGNAL, '\0'};
            Serial.write(signal);
            SecondSerial.write(signal);
            lastSignalTime = millis();
        }
    }

    void sendRedStateSignal() override {
        if (millis() - lastSignalTime > debounceTime) {
            char signal[3] = {SYSTEM_ID, RED_ACK, '\0'};
            Serial.write(signal);
            SecondSerial.write(signal);
            lastSignalTime = millis();
        }
    }

    char checkIncomingSignal() override {
        char receivedChar = '\0';

        // Check first Serial connection
        if (Serial.available() > 1) {
            char sourceId = Serial.read();
            char signal = Serial.read();

            // Forward the message to the other connection (if it didn't originate from this system)
            if (sourceId != SYSTEM_ID) {
                char forwardSignal[3] = {sourceId, signal, '\0'};
                SecondSerial.write(forwardSignal);
                receivedChar = signal;
            }
        }

        // Check second Serial connection
        if (SecondSerial.available() > 1) {
            char sourceId = SecondSerial.read();
            char signal = SecondSerial.read();

            // Forward the message to the other connection (if it didn't originate from this system)
            if (sourceId != SYSTEM_ID) {
                char forwardSignal[3] = {sourceId, signal, '\0'};
                Serial.write(forwardSignal);
                receivedChar = signal;
            }
        }

        return receivedChar;
    }
};

// Add ExternalInterface as a global object
ExternalInterfaceBase* externalSystem;

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

    void startTrafficLight(unsigned long currentTime, bool resetRedTime) {
        if (resetRedTime && currentState == RED && redStateStarted) {
            previousTime = currentTime;
            redStateStarted = false;
        }

        switch (currentState) {
            case RED:
                digitalWrite(trafficRedLedPin, HIGH);
                digitalWrite(trafficYellowLedPin, LOW);
                digitalWrite(trafficGreenLedPin, LOW);

                if (!redStateStarted) {
                    redStateStarted = true;
                    previousTime = currentTime;
                    if (IS_MASTER) {
                        externalSystem->sendRedStateSignal();  // Notify when entering RED
                    }
                }

                if (currentTime - previousTime >= timeRed*1000) {
                    currentState = RED_YELLOW;
                    previousTime = currentTime;
                    redStateStarted = false;
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
    unsigned long debounceDelay = 100; // To avoid multiple triggers within a short time
public:
    PedestrianLight(TrafficLight &trafficLight) : trafficLight(trafficLight) {}

    bool activateButtonPress(unsigned long currentTime, int buttonState) {
        if (buttonState == LOW && (currentTime - buttonPressTime > debounceDelay)) {
            buttonPressTime = currentTime;
            requestForGreen = true;

            // Send button press to external system
            externalSystem->sendButtonPress();

            return (trafficLight.getCurrentState() == 0);
        }
        return false;
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

    // Choose the specific interface
    externalSystem = new MultiSerialInterface();
}

void loop() {
    unsigned long currentTime = millis();
    int buttonState = digitalRead(buttonPin);
    bool resetRedTime = false;

    // Check external signals
    char signal = externalSystem->checkIncomingSignal();
    if (signal == 'B' && !IS_MASTER) {  // Button press from other system
        resetRedTime = true;
    } else {
        resetRedTime = pedestrianLight.activateButtonPress(currentTime, buttonState);
    }

    trafficLight.startTrafficLight(currentTime, resetRedTime);
    pedestrianLight.changePedestrianLight();
}
