#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Define M_PI if not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Screen dimensions
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

// Vertex Shader Source
const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;

    uniform mat4 projection;
    uniform mat4 model;

    void main() {
        gl_Position = projection * model * vec4(aPos, 1.0);
    }
)";

// Fragment Shader Source
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    uniform vec3 objectColor;

    void main() {
        FragColor = vec4(objectColor, 1.0);
    }
)";

// Generate Circle Vertices
std::vector<float> GenerateCircleVertices(float radius, unsigned int segments) {
    std::vector<float> vertices;
    float angleIncrement = 2.0f * M_PI / segments;

    for (unsigned int i = 0; i <= segments; ++i) {
        float angle = i * angleIncrement;
        vertices.push_back(radius * cos(angle)); // x
        vertices.push_back(radius * sin(angle)); // y
        vertices.push_back(0.0f);               // z
    }
    return vertices;
}

// Generate Rectangle Vertices
std::vector<float> GenerateRectangleVertices() {
    return {
        // Positions
        0.0f, 1.0f, 0.0f,   // Top-left
        1.0f, 0.0f, 0.0f,   // Bottom-right
        0.0f, 0.0f, 0.0f,   // Bottom-left

        0.0f, 1.0f, 0.0f,   // Top-left
        1.0f, 1.0f, 0.0f,   // Top-right
        1.0f, 0.0f, 0.0f    // Bottom-right
    };
}

// Compile and Link Shaders
unsigned int CreateShaderProgram() {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
            << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
            << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
            << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Ball structure
struct Ball {
    glm::vec2 position;
    glm::vec2 velocity;
    float radius;
    glm::vec3 color;
    glm::vec2 squashFactor; // Scaling factor for squashing/stretching
    float speed = 300.0f;
};

// Static object structure
struct StaticObject {
    glm::vec2 position; // Top-left corner
    glm::vec2 size;     // Width and height
    glm::vec3 color;    // RGB color
};

// AABB Collision Detection
bool CheckCollision(const Ball& ball, const StaticObject& obj) {
    // Ball AABB bounds
    glm::vec2 ballMin = ball.position - glm::vec2(ball.radius);
    glm::vec2 ballMax = ball.position + glm::vec2(ball.radius);

    // Object AABB bounds
    glm::vec2 objMin = obj.position;
    glm::vec2 objMax = obj.position + obj.size;

    // Check for overlap
    return (ballMax.x >= objMin.x && ballMin.x <= objMax.x &&
        ballMax.y >= objMin.y && ballMin.y <= objMax.y);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ball and Static Object with Squashing", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    unsigned int shaderProgram = CreateShaderProgram();

    // Ball setup
    Ball ball = {
        glm::vec2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f),
        glm::vec2(0.0f, 0.0f),
        50.0f,
        glm::vec3(1.0f, 0.5f, 0.2f),
        glm::vec2(1.0f, 1.0f)
    };

    // Static object setup
    StaticObject staticObj = {
        glm::vec2(300.0f, 300.0f), // Position
        glm::vec2(200.0f, 100.0f), // Size
        glm::vec3(0.0f, 1.0f, 0.0f) // Color (green)
    };

    // Generate vertices
    std::vector<float> circleVertices = GenerateCircleVertices(ball.radius, 36);
    std::vector<float> rectangleVertices = GenerateRectangleVertices();

    // VAO and VBO for Circle (Ball)
    unsigned int ballVAO, ballVBO;
    glGenVertexArrays(1, &ballVAO);
    glGenBuffers(1, &ballVBO);
    glBindVertexArray(ballVAO);
    glBindBuffer(GL_ARRAY_BUFFER, ballVBO);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // VAO and VBO for Rectangle (Static Object)
    unsigned int rectVAO, rectVBO;
    glGenVertexArrays(1, &rectVAO);
    glGenBuffers(1, &rectVBO);
    glBindVertexArray(rectVAO);
    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, rectangleVertices.size() * sizeof(float), rectangleVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glm::mat4 projection = glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT, -1.0f, 1.0f);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            mouseY = SCREEN_HEIGHT - mouseY; // Convert Y-axis

            glm::vec2 direction = glm::normalize(glm::vec2(mouseX, mouseY) - ball.position);
            ball.velocity = direction * ball.speed;
        }

        ball.position += ball.velocity * deltaTime;

        // Collision with walls
        if (ball.position.x - ball.radius < 0.0f || ball.position.x + ball.radius > SCREEN_WIDTH) {
            ball.velocity.x = -ball.velocity.x;
            ball.squashFactor = glm::vec2(0.7f, 1.3f);
        }
        if (ball.position.y - ball.radius < 0.0f || ball.position.y + ball.radius > SCREEN_HEIGHT) {
            ball.velocity.y = -ball.velocity.y;
            ball.squashFactor = glm::vec2(1.3f, 0.7f);
        }

        // Collision with static object
        if (CheckCollision(ball, staticObj)) {
            ball.velocity = -ball.velocity;
            ball.squashFactor = glm::vec2(1.3f, 0.7f); // Squash on collision
        }

        // Smoothly restore squash factor
        ball.squashFactor += (glm::vec2(1.0f, 1.0f) - ball.squashFactor) * 5.0f * deltaTime;

        glClearColor(0.0f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        int colorLoc = glGetUniformLocation(shaderProgram, "objectColor");

        // Render ball with squash/stretch
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(ball.position, 0.0f));
        model = glm::scale(model, glm::vec3(ball.squashFactor, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(colorLoc, 1, glm::value_ptr(ball.color));
        glBindVertexArray(ballVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, circleVertices.size() / 3);

        // Render static object
        model = glm::translate(glm::mat4(1.0f), glm::vec3(staticObj.position, 0.0f));
        model = glm::scale(model, glm::vec3(staticObj.size, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(colorLoc, 1, glm::value_ptr(staticObj.color));
        glBindVertexArray(rectVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &ballVAO);
    glDeleteBuffers(1, &ballVBO);
    glDeleteVertexArrays(1, &rectVAO);
    glDeleteBuffers(1, &rectVBO);

    glfwTerminate();
    return 0;
}
