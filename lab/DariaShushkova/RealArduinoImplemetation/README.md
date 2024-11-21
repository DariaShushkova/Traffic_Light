# Realization of Traffic and Pedestrian Light System with real Adruino

## Overview

This folder contains photos of the physical implementation of the Traffic and Pedestrian Light System using an Arduino Uno, which demonstrates the transition of states for both traffic and pedestrian lights based on the code provided in the repository.

The implementation is based on the code from the following folders:
- [CarTrafficLight](../CarTrafficLight/TrafficLight.cpp): when mapping the solution included only the Traffic light for vehicles 
- [PedestrianTrafficLight](../PedestrianTrafficLight/TrafficAndPedestrianLight.cpp): when mapping the solution included the Traffic and Pedestrian lights 

## Photos Description

1) State_TrafficRed_PedestranGreen.jpg: Shows the traffic light in the "Red" state, indicating vehicles should stop, while the pedestrian light is "Green", indicating pedestrians can go
2) State_TrafficRedYellow_PedestranRed.jpg: Shows the traffic light in the "RedYellow" state, indicating a transition between its Red and Green states, while the pedestrian light is "Red", signaling pedestrians to stop
3) State_TrafficGreen_PedestrianRed.jpg: Shows the traffic light in the "Green" state, indicating vehicles can move, while the pedestrian light is "Red", indicating pedestrians should wait
4) State_TrafficYellow_PedestranRed.jpg: Shows the traffic light in the "Yellow" state, indicating vehicles should slow down and prepare to stop, while the pedestrian light is "Red", signaling pedestrians to stop 

## Components Used

- Arduino Uno
- LEDs: Red, Yellow, Green (for traffic), and Red, Green (for pedestrian)
- Push Button
- Breadboard
- Resistors for each LED (220Ω)

## How to Setup

1) Refer to the circuit diagrams provided in the project folders:
- [CarTrafficLight](../CarTrafficLight/CircuitDiagram.jpg): when mapping the solution included only the Traffic light for vehicles
- [PedestrianTrafficLight](../PedestrianTrafficLight/CircuitDiagram.jpg): when mapping the solution included the Traffic and Pedestrian lights 
2) Assemble the Hardware:
- Connect one leg (cathode) of each Traffic LED (red, yellow and green) via a 220Ω resistor to the GND and the second leg (anode) to the corresponding digital pins indicated in the used code 
- Connect one leg (cathode) of each Pedestrian Light LED (red and green) via a 220Ω resistor to the GND and the second leg (anode) to the corresponding digital pins indicated in the used code 
- Connect the Push Button:
- Connect one leg of the push button to a digital input pin indicated in the used code and the other leg to GND.
3) Upload Code to the Arduino using Arduino IDE installed on the computer:
- For implementing the solution only with the Traffic light upload the code from the folder [CarTrafficLight](../CarTrafficLight/TrafficLight.cpp) 
- For implementing the solution with the Traffic and Pedestrian lights load the code from the folder [PedestrianTrafficLight](../PedestrianTrafficLight/TrafficAndPedestrianLight.cpp)
4) Power the System: Keep the Arduino connected to the computer via the USB cable or use a power supply for standalone operation.
5) Test the Setup: Once powered, the traffic lights should cycle through the states (Red → RedYellow → Green → Yellow).
Press the push button to simulate a pedestrian crossing request. The traffic light should switch to Red, and the pedestrian light should turn Green. After the crossing period, the system should reset to normal operation.


