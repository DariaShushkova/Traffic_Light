# The Traffic Light System on Arduino Uno
Program Overview:

This program simulates a traffic light system using an Arduino Uno to model the change of the 
following traffic light states: Red, Red-Yellow, Green, and Yellow. Timing events ensure that each light state 
remains on for a specific duration. State transitions based on the following delays:

RED: 10 seconds
RED_YELLOW: 1 second
GREEN: 10 seconds
YELLOW: 3 seconds

The attached state machine diagram shows states, transitions and events used in this project. 

The simulation is set up on the Wokwi platform, where the Arduino Uno controls three LEDs (red, yellow, and green) 
through a TrafficLight class with the switch state pattern written in C++ language. The attached class diagram shows the 
structure of the TrafficLight class.

# Components and Circuit

The project is structured with the following components:

1) Arduino Uno
2) Red LED: Indicates the "Stop" signal.
3) Yellow LED: Indicates the "Prepare to Stop/Go" signal
4) Green LED: Indicates the "Go" signal.
5) Resistors: Each LED has a 220-ohm resistor to limit the current.

Each LED has its anode connected to the appropriate pin as follows:

Red LED: Pin 13
Yellow LED: Pin 12
Green LED: Pin 11

Each LED has its cathode connected through a 220-ohm resistor to the GND pin of the Arduino Uno.

Mentioned components and their connection are indicated in the diagram.json file and the circuit diagram.

# Wokwi Simulation Setup

1) Open the Wokwi platform
2) Use the diagram.json and the circuit diagram to connect all necessary components
3) Copy and paste the provided code TrafficLight.cpp into the Wokwi code editor
4) Run the simulation

# Files Included
1) ClassDiagram.jpg 
2) StateMachineDiagram.jpg 
3) TrafficLight.cpp  
4) CircuitDiagram.jpg
5) diagram.json 
