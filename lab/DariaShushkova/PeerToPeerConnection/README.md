# Traffic and Pedestrian Light System
Program Overview:

This part of the project implements an Arduino-based traffic and pedestrian light control system using UART communication. It includes code for managing traffic lights, pedestrian signals, and their interaction. The system is designed for smooth and safe transitions between vehicle and pedestrian traffic. 

# Features
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
        - 100: Allow pedestrian crossing.
        - 200: Resume traffic flow.
- Button Integration: Pedestrian button to request crossing.

# Hardware Components and Circuit
1) Arduino Uno (or compatible board)
2) LEDs for traffic and pedestrian lights:
    - Traffic: Red, Yellow, Green.
    - Pedestrian: Red, Green.
3) Resistors (minimum 220Ω recommended)
4) Push button for pedestrian control
5) Serial communication interface

Refer to the circuit diagram UART_TrafficAndPedestrianLightsCommunication.pdf for detailed connections.

# How To Use
1) Setup Hardware: Assemble the components as per the provided circuit diagram.
2) Upload Code:
    - Load TrafficLight.cpp onto the Arduino controlling traffic lights.
    - Load PedestrianLight.cpp onto the Arduino managing pedestrian signals.
3) Run the System:
    - Power the Arduinos and ensure serial communication is established.
    - Press the pedestrian button to request crossing.

# Files Included
1) TrafficLight.cpp 
2) PedestrianLight.cpp 
3) CircuitDiagram.jpg
