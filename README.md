
# Ball and Static Object Game
<img width="362" alt="Screenshot 2024-12-04 at 6 53 40 PM" src="https://github.com/user-attachments/assets/6acf4a29-a7d9-46e6-9724-f8c3903249ef">

<img width="380" alt="Screenshot 2024-12-04 at 7 01 01 PM" src="https://github.com/user-attachments/assets/183943c2-1364-44d2-8a94-c0b9b95e8a8f">

<img width="380" alt="Screenshot 2024-12-04 at 7 01 01 PM" src="https://github.com/user-attachments/assets/79c2ae0f-86e7-4c26-a14c-8b7e521fe36a">


A simple OpenGL-based game where a ball moves around the screen, squashes on collisions, and interacts with a static rectangle. This project demonstrates rendering, movement, collision detection, and transformations in OpenGL.

## Features

1. **Ball Movement**:
   - The ball moves in response to user clicks.
   - The ball bounces off the screen edges and interacts with the static rectangle.

2. **Squashing Effect**:
   - The ball squashes and stretches dynamically upon collisions with walls or the static object.

3. **Static Object**:
   - A green rectangle is positioned on the screen.
   - The ball detects and reacts to collisions with the rectangle.

4. **OpenGL Basics**:
   - Rendering primitives like circles and rectangles.
   - Transformations using the OpenGL matrix stack (translation, scaling).
   - Shader programming for vertex and fragment shaders.

---

## Getting Started

### Prerequisites

- **C++ Compiler**: Supports C++11 or later.
- **OpenGL**: Version 3.3 or later.
- **GLFW**: For windowing and input handling.
- **GLAD**: For loading OpenGL functions.
- **GLM**: For mathematical operations and transformations.

### Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/your-repo/ball-game.git
   cd ball-game
   ```

2. **Install Dependencies**:
   - GLFW: [Download and integrate GLFW](https://www.glfw.org/)
   - GLAD: [Download and integrate GLAD](https://glad.dav1d.de/)
   - GLM: [Download and integrate GLM](https://github.com/g-truc/glm)

3. **Compile the Code**:
   ```bash
   g++ main.cpp -o BallGame -lglfw -ldl -lGL
   ```

4. **Run the Program**:
   ```bash
   ./BallGame
   ```

---

## Code Structure

### `main.cpp`
This file contains the entire implementation:
1. **Initialization**:
   - Sets up GLFW and GLAD for OpenGL context and function loading.
   - Creates the shaders for rendering.

2. **Structures**:
   - `Ball`: Represents the moving ball, including position, velocity, radius, color, and squash factor.
   - `StaticObject`: Represents the static rectangle, including position, size, and color.

3. **Rendering**:
   - A vertex and fragment shader pipeline is used to render the ball and rectangle.
   - Projection and model transformations are applied for proper positioning and scaling.

4. **Collision Detection**:
   - AABB (Axis-Aligned Bounding Box) is used to detect collisions between the ball and the rectangle.

---

## How It Works

### User Interaction
- **Clicking**: Clicking on the window sends the ball toward the clicked location.
- **Bouncing**: The ball bounces off screen edges and the static object.

### Ball Behavior
- The ball squashes along the axis of collision and stretches perpendicular to it.
- The squash/stretch effect gradually recovers over time.

### Static Object
- A green rectangle is rendered on the screen and interacts with the ball when they collide.

---

## Controls

- **Left Click**: Directs the ball to the clicked location.

---

## Dependencies

- **GLFW**: Handles window creation and input.
- **GLAD**: Loads OpenGL functions.
- **GLM**: Provides vector and matrix math utilities.
- **OpenGL 3.3**: Used for rendering graphics.

---

## Example Output

- A turquoise window with:
  - An orange ball that moves, bounces, and squashes upon collision.
  - A static green rectangle that interacts with the ball.

---

## Future Improvements

1. Add multiple static objects for the ball to interact with.
2. Introduce scoring mechanisms for player interaction.
3. Add textures to the ball and rectangle.

---

## License

This project is licensed under the MIT License - see the LICENSE file for details.
