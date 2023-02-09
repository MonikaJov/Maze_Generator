#include <OpenGLPrj.hpp>

#include <GLFW/glfw3.h>

#include <Camera.hpp>
#include <Shader.hpp>

#include <iostream>
#include <string>
#include <vector>

const std::string program_name = ("Colour");

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

std::vector<std::vector<std::string>> generateMaze();
std::vector<std::vector<std::string>> maze = generateMaze();

//maze rows and columns
const unsigned int ROWS = 11;
const unsigned int COLS = 11;

// camera
static Camera camera(glm::vec3((COLS-1)/2, 0.0f, 0.0f));
static float lastX = SCR_WIDTH / 2.0f;
static float lastY = SCR_HEIGHT / 2.0f;
static bool firstMouse = true;

// timing
static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

// lighting
static glm::vec3 lightPos(3.2f, 1.0f, (-1.0 * (ROWS/2)));

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(
      GLFW_OPENGL_FORWARD_COMPAT,
      GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
                                          program_name.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader program
    // ------------------------------------
    std::string shader_location("../res/shaders/");

    std::string material_shader("material");
    std::string lamp_shader("lamp");
    std::string grass_shader("shader");

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader(
            shader_location + material_shader + std::string(".vert"),
            shader_location + material_shader + std::string(".frag"));
    Shader lampShader(shader_location + lamp_shader + std::string(".vert"),
                      shader_location + lamp_shader + std::string(".frag"));
    Shader grass(shader_location + grass_shader + std::string(".vert"),
                 shader_location + grass_shader + std::string(".frag"));

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    std::vector<float> vertices;

//    std::vector<std::vector<std::string>> maze = generateMaze();

    float k = 1.0;
    float x = 0.0;
    float z = -1.0 * ROWS;
    int count = 0;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if(maze[i][j] == "##"){
                //positions                                                                 //colors                                                                        //texture coords
                vertices.push_back(x);vertices.push_back(-0.5);vertices.push_back(z-k);     vertices.push_back(0.0f);vertices.push_back(0.0f);vertices.push_back(-1.0f);     vertices.push_back(0.0f);vertices.push_back(0.0f); //
                vertices.push_back(x+k);vertices.push_back(-0.5);vertices.push_back(z-k);   vertices.push_back(0.0f);vertices.push_back(0.0f);vertices.push_back(-1.0f);    vertices.push_back(1.0f);vertices.push_back(0.0f);//
                vertices.push_back(x+k);vertices.push_back(0.5);vertices.push_back(z-k);    vertices.push_back(0.0f);vertices.push_back(0.0f);vertices.push_back(-1.0f);     vertices.push_back(1.0f);vertices.push_back(1.0f);//
                vertices.push_back(x+k);vertices.push_back(0.5);vertices.push_back(z-k);    vertices.push_back(0.0f);vertices.push_back(0.0f);vertices.push_back(-1.0f);     vertices.push_back(1.0f);vertices.push_back(1.0f);//
                vertices.push_back(x);vertices.push_back(0.5);vertices.push_back(z-k);      vertices.push_back(0.0f);vertices.push_back(0.0f);vertices.push_back(-1.0f);     vertices.push_back(0.0f);vertices.push_back(1.0f);//
                vertices.push_back(x);vertices.push_back(-0.5);vertices.push_back(z-k);     vertices.push_back(0.0f);vertices.push_back(0.0f);vertices.push_back(-1.0f);     vertices.push_back(0.0f);vertices.push_back(0.0f);//

                vertices.push_back(x);vertices.push_back(-0.5);vertices.push_back(z);     vertices.push_back(0.0f);vertices.push_back(0.0f);vertices.push_back(1.0f);      vertices.push_back(0.0f);vertices.push_back(0.0f);  //
                vertices.push_back(x+k);vertices.push_back(-0.5);vertices.push_back(z);   vertices.push_back(0.0f);vertices.push_back(0.0f);vertices.push_back(1.0f);    vertices.push_back(1.0f);vertices.push_back(0.0f);//
                vertices.push_back(x+k);vertices.push_back(0.5);vertices.push_back(z);   vertices.push_back(0.0f);vertices.push_back(0.0f);vertices.push_back(1.0f);     vertices.push_back(1.0f);vertices.push_back(1.0f);//
                vertices.push_back(x+k);vertices.push_back(0.5);vertices.push_back(z);   vertices.push_back(0.0f);vertices.push_back(0.0f);vertices.push_back(1.0f);     vertices.push_back(1.0f);vertices.push_back(1.0f);//
                vertices.push_back(x);vertices.push_back(0.5);vertices.push_back(z);    vertices.push_back(0.0f);vertices.push_back(0.0f);vertices.push_back(1.0f);     vertices.push_back(0.0f);vertices.push_back(1.0f);//
                vertices.push_back(x);vertices.push_back(-0.5);vertices.push_back(z);     vertices.push_back(0.0f);vertices.push_back(0.0f);vertices.push_back(1.0f);     vertices.push_back(0.0f);vertices.push_back(0.0f);//

                vertices.push_back(x);vertices.push_back(0.5);vertices.push_back(z);    vertices.push_back(-1.0f);vertices.push_back(0.0f);vertices.push_back(0.0f);    vertices.push_back(1.0f);vertices.push_back(0.0f);//
                vertices.push_back(x);vertices.push_back(0.5);vertices.push_back(z-k);   vertices.push_back(-1.0f);vertices.push_back(0.0f);vertices.push_back(0.0f);    vertices.push_back(1.0f);vertices.push_back(1.0f);//
                vertices.push_back(x);vertices.push_back(-0.5);vertices.push_back(z-k);   vertices.push_back(-1.0f);vertices.push_back(0.0f);vertices.push_back(0.0f);     vertices.push_back(0.0f);vertices.push_back(1.0f);//
                vertices.push_back(x);vertices.push_back(-0.5);vertices.push_back(z-k);   vertices.push_back(-1.0f);vertices.push_back(0.0f);vertices.push_back(0.0f);     vertices.push_back(0.0f);vertices.push_back(1.0f);//
                vertices.push_back(x);vertices.push_back(-0.5);vertices.push_back(z);    vertices.push_back(-1.0f);vertices.push_back(0.0f);vertices.push_back(0.0f);     vertices.push_back(0.0f);vertices.push_back(0.0f);
                vertices.push_back(x);vertices.push_back(0.5);vertices.push_back(z);     vertices.push_back(-1.0f);vertices.push_back(0.0f);vertices.push_back(0.0f);     vertices.push_back(1.0f);vertices.push_back(0.0f);

                vertices.push_back(x+k);vertices.push_back(0.5);vertices.push_back(z);  vertices.push_back(-1.0f);vertices.push_back(0.0f);vertices.push_back(0.0f);    vertices.push_back(1.0f);vertices.push_back(0.0f);//
                vertices.push_back(x+k);vertices.push_back(0.5);vertices.push_back(z-k);   vertices.push_back(-1.0f);vertices.push_back(0.0f);vertices.push_back(0.0f);    vertices.push_back(1.0f);vertices.push_back(1.0f);//
                vertices.push_back(x+k);vertices.push_back(-0.5);vertices.push_back(z-k);   vertices.push_back(-1.0f);vertices.push_back(0.0f);vertices.push_back(0.0f);     vertices.push_back(0.0f);vertices.push_back(1.0f);//
                vertices.push_back(x+k);vertices.push_back(-0.5);vertices.push_back(z-k);   vertices.push_back(-1.0f);vertices.push_back(0.0f);vertices.push_back(0.0f);     vertices.push_back(0.0f);vertices.push_back(1.0f);//
                vertices.push_back(x+k);vertices.push_back(-0.5);vertices.push_back(z);    vertices.push_back(-1.0f);vertices.push_back(0.0f);vertices.push_back(0.0f);     vertices.push_back(0.0f);vertices.push_back(0.0f);
                vertices.push_back(x+k);vertices.push_back(0.5);vertices.push_back(z);     vertices.push_back(-1.0f);vertices.push_back(0.0f);vertices.push_back(0.0f);     vertices.push_back(1.0f);vertices.push_back(0.0f);

                vertices.push_back(x);vertices.push_back(-0.5);vertices.push_back(z-k);     vertices.push_back(0.0f);vertices.push_back(-1.0f);vertices.push_back(0.0f);    vertices.push_back(0.0f);vertices.push_back(1.0f);
                vertices.push_back(x+k);vertices.push_back(-0.5);vertices.push_back(z-k);   vertices.push_back(0.0f);vertices.push_back(-1.0f);vertices.push_back(0.0f);    vertices.push_back(1.0f);vertices.push_back(1.0f);
                vertices.push_back(x+k);vertices.push_back(-0.5);vertices.push_back(z);     vertices.push_back(0.0f);vertices.push_back(-1.0f);vertices.push_back(0.0f);    vertices.push_back(1.0f);vertices.push_back(0.0f);
                vertices.push_back(x+k);vertices.push_back(-0.5);vertices.push_back(z);     vertices.push_back(0.0f);vertices.push_back(-1.0f);vertices.push_back(0.0f);    vertices.push_back(1.0f);vertices.push_back(0.0f);
                vertices.push_back(x);vertices.push_back(-0.5);vertices.push_back(z);       vertices.push_back(0.0f);vertices.push_back(-1.0f);vertices.push_back(0.0f);    vertices.push_back(0.0f);vertices.push_back(0.0f);
                vertices.push_back(x);vertices.push_back(-0.5);vertices.push_back(z-k);     vertices.push_back(0.0f);vertices.push_back(-1.0f);vertices.push_back(0.0f);    vertices.push_back(0.0f);vertices.push_back(1.0f);

                vertices.push_back(x);vertices.push_back(0.5);vertices.push_back(z-k);      vertices.push_back(0.0f);vertices.push_back(-1.0f);vertices.push_back(0.0f);    vertices.push_back(0.0f);vertices.push_back(1.0f);
                vertices.push_back(x+k);vertices.push_back(0.5);vertices.push_back(z-k);   vertices.push_back(0.0f);vertices.push_back(-1.0f);vertices.push_back(0.0f);    vertices.push_back(1.0f);vertices.push_back(1.0f);
                vertices.push_back(x+k);vertices.push_back(0.5);vertices.push_back(z);     vertices.push_back(0.0f);vertices.push_back(-1.0f);vertices.push_back(0.0f);    vertices.push_back(1.0f);vertices.push_back(0.0f);
                vertices.push_back(x+k);vertices.push_back(0.5);vertices.push_back(z);     vertices.push_back(0.0f);vertices.push_back(-1.0f);vertices.push_back(0.0f);    vertices.push_back(1.0f);vertices.push_back(0.0f);
                vertices.push_back(x);vertices.push_back(0.5);vertices.push_back(z);       vertices.push_back(0.0f);vertices.push_back(-1.0f);vertices.push_back(0.0f);    vertices.push_back(0.0f);vertices.push_back(0.0f);
                vertices.push_back(x);vertices.push_back(0.5);vertices.push_back(z-k);     vertices.push_back(0.0f);vertices.push_back(-1.0f);vertices.push_back(0.0f);    vertices.push_back(0.0f);vertices.push_back(1.0f);

                count=count+36;
            }
            x = x + k;
        }
        x = 0.0;
        z = z - k;

    }
    unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
    };
    vertices.push_back(-100.0);vertices.push_back(-0.46);vertices.push_back(100.0);  vertices.push_back(0.0f);vertices.push_back(-1.0f);vertices.push_back(0.0f);    vertices.push_back(0.0f);vertices.push_back(1.0f);
    vertices.push_back(100.0);vertices.push_back(-0.46);vertices.push_back(100.0);  vertices.push_back(0.0f);vertices.push_back(-1.0f);vertices.push_back(0.0f);    vertices.push_back(1.0f);vertices.push_back(1.0f);
    vertices.push_back(-100.0);vertices.push_back(-0.46);vertices.push_back(-100.0);     vertices.push_back(0.0f);vertices.push_back(-1.0f);vertices.push_back(0.0f);    vertices.push_back(1.0f);vertices.push_back(0.0f);
    vertices.push_back(100.0);vertices.push_back(-0.46);vertices.push_back(100.0);     vertices.push_back(0.0f);vertices.push_back(-1.0f);vertices.push_back(0.0f);    vertices.push_back(1.0f);vertices.push_back(0.0f);
    vertices.push_back(-100.0);vertices.push_back(-0.46);vertices.push_back(-100.0);       vertices.push_back(0.0f);vertices.push_back(-1.0f);vertices.push_back(0.0f);    vertices.push_back(0.0f);vertices.push_back(0.0f);
    vertices.push_back(100.0);vertices.push_back(-0.46);vertices.push_back(-100.0);     vertices.push_back(0.0f);vertices.push_back(-1.0f);vertices.push_back(0.0f);    vertices.push_back(0.0f);vertices.push_back(1.0f);

    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture 1
    // ---------
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("../res/textures/grass_texture.png", &width,&height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // texture 2
    // ---------
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("../res/textures/container.jpg", &width,&height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    lightingShader.use();
    lightingShader.setInt("texture2", 0);
    grass.use();
    grass.setInt("texture1", 1);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.25f, 0.22f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //lavirint
        lightingShader.use();
        lightingShader.setVec3("light.position", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
        lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("material.specular", 0.1f, 0.1f, 0.1f);
        lightingShader.setFloat("material.shininess", 10.0f);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                 (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, count);

        //pod
        grass.use();
        grass.setVec3("light.position", lightPos);
        grass.setVec3("viewPos", camera.Position);
        grass.setVec3("light.ambient", 0.6f, 0.6f, 0.6f);
        grass.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        grass.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        grass.setVec3("material.specular", 0.05f, 0.05f, 0.05f);
        grass.setFloat("material.shininess", 10.0f);
        projection = glm::perspective(glm::radians(camera.Zoom),(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        grass.setMat4("projection", projection);
        grass.setMat4("view", view);
        model = glm::mat4(1.0f);
        grass.setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, count, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}
// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposd, double yposd) {
    float xpos = static_cast<float>(xposd);
    float ypos = static_cast<float>(yposd);
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, 0);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
bool checkWalls(int currI, int currJ, std::vector<int> &trailI, std::vector<int> &trailJ) {
    bool pom1 = true, pom2 = true, pom3 = true, pom4 = true, pom = true;

    for (int i = 0; i < trailI.size(); i++) {
        if (((currI - 2) == trailI[i] && currJ == trailJ[i]) || (currI - 2) < 0)
            pom1 = false;
        if (((currI + 2) == trailI[i] && currJ == trailJ[i]) || (currI + 2) > (ROWS - 1))
            pom2 = false;
        if (((currJ - 2) == trailJ[i] && currI == trailI[i]) || (currJ - 2) < 0)
            pom3 = false;
        if (((currJ + 2) == trailJ[i] && currI == trailI[i]) || (currJ + 2) > (COLS - 1)) {
            pom4 = false;
        }
    }

    if (!pom1 && !pom2 && !pom3 && !pom4)
        pom = false;

    return pom; //if it returns false there are no free paths
}

bool checkWall(int wallI, int wallJ, std::vector<int> &trailI, std::vector<int> &trailJ) {
    bool pom = true;
    for (int i = 0; i < trailI.size(); i++) {
        if ((wallI == trailI[i] && wallJ == trailJ[i]) || wallI < 0 || wallJ < 0 || wallI > (ROWS - 1) || wallJ > (COLS - 1)) {
            pom = false;
            break;
        }
    }
    return pom;
}
std::vector<int> goBack(int currI, int currJ, std::vector<int> trailI, std::vector<int> trailJ){
    std::vector<int> curr = {currI, currJ};
    int index = 0;

    for(int i = 0; i < trailI.size(); i++){ //finds the last value in trailI and trailJ
        if(curr[0]==trailI[i] && curr[1]==trailJ[i]){
            index = i;
        }
    }
    while(index > 0){ //go back the trail and for every values check if there are free paths, if there are return the index of the block where there are free paths
        index--;
        curr[0]=trailI[index];
        curr[1]=trailJ[index];
        if(checkWalls(curr[0], curr[1], trailI, trailJ)){
            return curr;
        }
    }

    return {}; //if there are no free paths left return an empty vector and that means we completed the maze
}
std::vector<std::vector<std::string>> generateMaze() {
    std::vector<std::vector<std::string>> maze(ROWS, std::vector<std::string>(COLS));
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (!(i % 2 == 1 && j % 2 == 1)) {
                maze[i][j] = "##";
            } else {
                maze[i][j] = "  ";
            }
        }
    }
    srand(time(0));
    std::vector<int> trailI(ROWS * COLS * 3);
    std::vector<int> trailJ(ROWS * COLS * 3);
    int currI = 1;
    int currJ = 1;
    int rand_val;
    int rand_start;
    int rand_end;
    int k = 0;

    while (true) {
        trailI[k] = currI;
        trailJ[k] = currJ;
        if (checkWalls(currI, currJ, trailI, trailJ)) {
            rand_val = rand() % 4;
            if (rand_val == 0) {
                if (checkWall(currI - 2, currJ, trailI, trailJ)) {
                    maze[currI - 1][currJ] = "  ";
                    currI = currI - 2;
                }
            }
            if (rand_val == 1) {
                if (checkWall(currI + 2, currJ, trailI, trailJ)) {
                    maze[currI + 1][currJ] = "  ";
                    currI = currI + 2;
                }
            }
            if (rand_val == 2) {
                if (checkWall(currI, currJ - 2, trailI, trailJ)) {
                    maze[currI][currJ - 1] = "  ";
                    currJ = currJ - 2;
                }
            }
            if (rand_val == 3) {
                if (checkWall(currI, currJ + 2, trailI, trailJ)) {
                    maze[currI][currJ + 1] = "  ";
                    currJ = currJ + 2;
                }
            }
        } else if (!(goBack(currI, currJ, trailI, trailJ).empty())) {
            std::vector<int> curr = goBack(currI, currJ, trailI, trailJ);
            currI = curr[0];
            currJ = curr[1];
        } else {
            while (true) {
                rand_start = rand() % COLS;
                if (!(maze[ROWS - 2][rand_start] == "##")) {
                    maze[ROWS - 1][rand_start] = "  ";
                    break;
                }
            }
            while (true) {
                rand_end = rand() % COLS;
                if (maze[1][rand_end] != "##") {
                    maze[0][rand_end] = "  ";
                    break;
                }
            }
            break;
        }
        k++;
    }
    return maze;
}
