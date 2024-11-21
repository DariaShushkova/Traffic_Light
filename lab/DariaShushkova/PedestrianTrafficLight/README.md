# The Pedestrian And Traffic Light System

## Overview

This program simulates a traffic light system integrated with the pedestrian lights and button using an Arduino Uno on the Wokwi platform. 
The system models traffic light states (Red, Red-Yellow, Green, and Yellow) and pedestrian lights (Red and Green), changed via the button and specific time events.
The logic of the button work and the time events ensure that the pedestrian light is green only when the traffic light is red and each light state of the traffic light
remains on for a specific duration. 

The traffic light state transitions based on the following delays:

RED: 10 seconds
RED_YELLOW: 1 second
GREEN: 10 seconds
YELLOW: 5 seconds

The pedestrian light system is synchronized with the traffic light.

Button Activation cause the following behavior: 
- if the button is pressed while the traffic light is red, the pedestrian green light turns on, and the red light delay for the traffic light resets to count from the beginning;
- if the button is pressed while the traffic light is not red, the pedestrian light remains red and turns green when the traffic light enters the red state.

The attached state machine diagrams for the traffic light and for the pedestrian light show states, transitions and events used in this project.
The interruction between the objects is shown on the sequence diagram.  

The simulation is set up on the Wokwi platform, where the Arduino Uno controls three LEDs (red, yellow and green) through a TrafficLight class with the switch state pattern and a PedestrianLight class written in C++ language. The attached class diagram shows the structure of the TrafficLight class.

## Components and Circuit

The project is structured with the following components:

1) Arduino Uno
2) Traffic Light LEDs:
- Red LED (Pin 13): Indicates "Stop" signal.
- Yellow LED (Pin 12): Indicates "Prepare to Stop/Go" signal.
- Green LED (Pin 11): Indicates "Go" signal.
3) Pedestrian LEDs:
- Red LED (Pin 6): Indicates "Do Not Cross" for pedestrians.
- Green LED (Pin 7): Indicates "Safe to Cross" for pedestrians.
4) Button (Pin 5): Allows pedestrians to request a green light to cross the road.
5) Resistors: Each LED has a 220-ohm resistor to limit the current.

Each LED has its anode connected to the appropriate pin and its cathode connected through a 220-ohm resistor to the GND of the Arduino Uno.
Button has the first contact (left / right) connected to the pin and the second contact (left / right) connected to the GND of the Arduino Uno.

Mentioned components and their connection are indicated in the diagram.json file and the circuit diagram.

## Wokwi Simulation Setup

1) Open the Wokwi platform
2) Use the diagram.json and the circuit diagram to connect all necessary components
3) Copy and paste the provided code TrafficLight.cpp into the Wokwi code editor
4) Run the simulation

## Files Included

1) ClassDiagram.jpg 
2) SequenceDiagram.jpg
3) StateMachineDiagram.jpg 
4) PedestrianStateMachineDiagram.jpg
5) TrafficLight.cpp  
6) CircuitDiagram.jpg
7) diagram.json 
