# Traffic and Pedestrian Light System With Pee-To-Peer Communication

## Overview:

This part of the project implements an Arduino-based traffic and pedestrian light control system using UART communication. It includes code for managing traffic lights, pedestrian lights, and their interaction. The system is designed for smooth and safe transitions between vehicle and pedestrian traffic. 

- Traffic Light Control:
    - States: Red, RedYellow, Green, and Yellow.
    - Timers for each state to ensure proper traffic flow.
    - Interaction with pedestrian requests.
- Pedestrian Light Control:
    - States: Red and Green.
    - Green light activated via UART when pedestrians are allowed to cross.
    - Automatic return to Red and notification to resume traffic.
- UART Communication:
    - Enables synchronization between traffic and pedestrian lights.
    - Messages:
        - 100: Request to walk (allows pedestrian crossing).
        - 200: Resume traffic flow.
- Button Integration: Pedestrian button to request crossing.

When the button is pressed, the Traffic Light Arduino should send the message 100  to the Pedestrian Light Arduino, signaling it to activate the pedestrian green light.
After the crossing phase is complete, the Pedestrian Light Arduino should send a message 200 to the Traffic Light Arduino, signaling it to resume the traffic flow.

## Hardware Components and Circuit

1) Two Arduinos Uno (or compatible board)
    - One controls traffic lights
    - The second one controls pedestrian lights
2) LEDs for traffic and pedestrian lights:
    - Traffic: Red, Yellow, Green.
    - Pedestrian: Red, Green.
3) Resistors: Minimum 220Ω recommended for each LED
4) Push button connected to the Arduino, which controls traffic lights   
5) UART Communication: RX and TX pin connections between the two Arduinos

Refer to the circuit diagram (UART_TrafficAndPedestrianLightsCommunication.pdf) for detailed connections.

## How To Use

1) Setup Hardware: 
    - Assemble the components according to the provided circuit diagram.
    - Ensure:
        - Traffic Light Arduino: Connect the red, yellow, and green LEDs and the push button to the corresponding digital pins indicated in the code TrafficLight.cpp
        - Pedestrian Light Arduino: Connect the pedestrian red and green LEDs to the corresponding digital pins indicated in the code PedestrianLight.cpp
2) Establish UART Communication:
    - Connect the TX (transmit) pin of the Traffic Light Arduino to the RX (receive) pin of the Pedestrian Light Arduino.
    - Connect the RX (receive) pin of the Traffic Light Arduino to the TX (transmit) pin of the Pedestrian Light Arduino.
    - Connect the GND (ground) pin of both Arduinos.
3) Upload Code using Arduino IDE installed on the computer:
    - Upload the code TrafficLight.cpp to the Arduino controlling traffic lights.
    - Upload the code PedestrianLight.cpp to the Arduino managing pedestrian lights.
4) Run the System:
    - Power both Arduinos using USB cables or another power source. 
    - Ensure UART communication is correctly established and operational.
    - Press the pedestrian button to request crossing.

This folder contains a photo (PeerToPeerUARTCommunication_20112024.jpg) of the real-world implementation of the system, showcasing the peer-to-peer connection between two Arduinos via UART communication.

## Files Included

1) TrafficLight.cpp 
2) PedestrianLight.cpp 
3) UART_TrafficAndPedestrianLightsCommunication.pdf
4) PeerToPeerUARTCommunication_20112024.jpg
