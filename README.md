# Driving Sim

## Overview
Driving Sim is a 2D driving simulator designed to simulate vehicle physics, including acceleration, steering, and loss of grip on different road conditions. It utilizes **SFML** for graphics and input handling and implements **Runge-Kutta 4 (RK4) numerical integration** for precise physics calculations.

## Features
- **Physics-based vehicle dynamics**
    - Longitudinal and lateral dynamics
    - Slip computation for tire grip
    - Realistic acceleration and steering mechanics
- **Circuit creation system**
    - Various road segment types
    - Adjustable rotation and mirroring
- **Debugging tools**
    - FPS Counter
    - Debug Mode displaying vehicle state (speed, position, slip, etc.)
- **Runge-Kutta 4 integration for accurate motion simulation**

## Installation
### Prerequisites
- **C++17 or later**
- **CMake** (for building)
- **SFML** V2.5 or later (but lower than V3.0)
- **Installed by the `CMakeLists.txt` file**:
  - **Google Test** (for unit testing)

### Build Instructions
1. Clone the repository:
   ```sh
   git clone 'https://github.com/QuentinRdl/Driving-Sim'
   cd Driving_Sim
   ```
2. Create a build directory and compile the project:
   ```sh
   mkdir -p build && cd build
   cmake ..
   make
   ```
3. Run the simulator:
   ```sh
   ./Driving_Sim
   ```

## Usage
### Controls
- **Arrow Up**: Accelerate
- **Arrow Down**: Brake
- **Arrow Left/Right**: Steer
- **F**: Toggle FPS counter
- **F3**: Toggle Debug Mode
- **F7**: Print car position and yaw rate
- **ESC**: Exit the simulation

## Project Structure
```
Driving_Sim/
├── src/
│   ├── car.cpp
│   ├── circuit.cpp
│   ├── game.cpp
│   ├── vehicle.cpp
│   ├── debugmode.cpp
│   ├── fpscounter.cpp
│   └── main.cpp
├── include/
│   ├── car.h
│   ├── circuit.h
│   ├── game.h
│   ├── vehicle.h
│   ├── debugmode.h
│   └── fpscounter.h
├── assets/
│   └── images/
│       ├── car.png
│       ├── large_turn.png
│       ├── long_straight.png
│       ├── medium_turn.png
│       ├── small_straight.png
│       ├── small_turn.png
│       ├── s_turn.png
│       └── u_turn.png
├── tests/
│   └── rk4_integration_test.cpp
├── CMakeLists.txt
└── README.md
```

## License
This project is licensed under the MIT License.

## Author
**Quentin RADLO** & **Romain GALLAND**

