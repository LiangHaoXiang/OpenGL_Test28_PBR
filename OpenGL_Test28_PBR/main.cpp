#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Shader.h"
#include <iostream>
//引入openGL数学库
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.hpp"
#include "Model.hpp"

using namespace std;
using namespace glm;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char *path);
void renderSphere();

// settings 屏幕宽高
const unsigned int SCR_WIDTH = 2560;
const unsigned int SCR_HEIGHT = 1600;

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
bool firstMouse = true;     //是否是第一次获取鼠标输入
//鼠标在屏幕的初始位置
float lastX = SCR_WIDTH * 0.5f;
float lastY = SCR_HEIGHT * 0.5f;

Camera camera(vec3(0.0f, 0.0f, 5.0f));

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }
    
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//运行时隐藏鼠标光标
    
    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    
    // build and compile our shader program
    // ------------------------------------
    char* headDir = "/Users/haoxiangliang/Desktop/代码草稿/OpenGL/OpenGL_Test28_PBR/OpenGL_Test28_PBR/";
    string p1 = string(headDir) + "LightShader/PBR.vs";
    string p2 = string(headDir) + "LightShader/PBR.fs";
    Shader shader(p1.c_str(), p2.c_str());
    
//    unsigned int albedoTexture = loadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/Iron-Scuffed_bl/Iron-Scuffed_basecolor.png");
//    unsigned int normalTexture = loadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/Iron-Scuffed_bl/Iron-Scuffed_normal.png");
//    unsigned int metallicTexture = loadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/Iron-Scuffed_bl/Iron-Scuffed_metallic.png");
//    unsigned int roughnessTexture = loadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/Iron-Scuffed_bl/Iron-Scuffed_roughness.png");
    
//    unsigned int albedoTexture =
//    loadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/Aluminum-Scuffed_Unreal-Engine/Aluminum-Scuffed_basecolor.png");
//    unsigned int normalTexture =
//    loadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/Aluminum-Scuffed_Unreal-Engine/Aluminum-Scuffed_normal.png");
//    unsigned int metallicTexture =
//    loadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/Aluminum-Scuffed_Unreal-Engine/Aluminum-Scuffed_metallic.png");
//    unsigned int roughnessTexture =
//    loadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/Aluminum-Scuffed_Unreal-Engine/Aluminum-Scuffed_roughness.png");
    
//    unsigned int albedoTexture = loadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/rustediron-streaks2-bl/rustediron-streaks_basecolor.png");
//    unsigned int normalTexture = loadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/rustediron-streaks2-bl/rustediron-streaks_normal.png");
//    unsigned int metallicTexture = loadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/rustediron-streaks2-bl/rustediron-streaks_metallic.png");
//    unsigned int roughnessTexture = loadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/rustediron-streaks2-bl/rustediron-streaks_roughness.png");
    
    unsigned int albedoTexture = loadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/TEXTUREHAVEN材质包/rock_08/tex/albedo.png");
    unsigned int normalTexture = loadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/TEXTUREHAVEN材质包/rock_08/tex/normal.png");
    unsigned int metallicTexture = loadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/TEXTUREHAVEN材质包/rock_08/tex/specular.png");
    unsigned int roughnessTexture = loadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/TEXTUREHAVEN材质包/rock_08/tex/smoothness.png");
    unsigned int aoTexture = loadTexture("/Users/haoxiangliang/Desktop/未命名文件夹/TEXTUREHAVEN材质包/rock_08/tex/occlusion.png");
    
    // lighting info
    // -------------
    vec3 lightPositions[] = {
        vec3(-10.0f,  10.0f, 10.0f),
        vec3( 10.0f,  10.0f, 10.0f),
        vec3(-10.0f, -10.0f, 10.0f),
        vec3( 10.0f, -10.0f, 10.0f),
    };
    vec3 lightColors[] = {
        vec3(300.0f, 300.0f, 300.0f),
        vec3(300.0f, 300.0f, 300.0f),
        vec3(300.0f, 300.0f, 300.0f),
        vec3(300.0f, 300.0f, 300.0f)
    };
    
    // shader configuration
    // --------------------
    shader.use();
    shader.setInt("albedoMap", 0);
    shader.setInt("normalMap", 1);
    shader.setInt("metallicMap", 2);
    shader.setInt("roughnessMap", 3);
    shader.setVec3("albedo", 0.5f, 0.0f, 0.0f);

    int nrRows = 7;
    int nrColumns = 7;
    float spacing = 2.5f;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 projection = perspective(radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        mat4 view = camera.GetViewMatrix();
        mat4 model = mat4(1.0f);
        shader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, albedoTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalTexture);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, metallicTexture);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, roughnessTexture);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, aoTexture);
        shader.setVec3("camPos", camera.Position);
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        
        for (int row = 0; row < nrRows; ++row)
        {
            for (int col = 0; col < nrColumns; ++col)
            {
                model = mat4(1.0f);
                model = translate(model, vec3((col - (nrColumns / 2)) * spacing, (row - (nrRows / 2)) * spacing, 0.0f));
                shader.setMat4("model", model);
                renderSphere();
            }
        }

        // render light source (simply re-render sphere at light positions)
        // this looks a bit off as we use the same shader, but it'll make their positions obvious and
        // keeps the codeprint small.
        for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
        {
            vec3 newPos = lightPositions[i] + vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
            newPos = lightPositions[i];
            shader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
            shader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

            model = mat4(1.0f);
            model = translate(model, newPos);
            model = scale(model, vec3(0.5f));
            shader.setMat4("model", model);
            renderSphere();
        }

        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int sphereVAO = 0;
unsigned int sphereVBO = 0;
unsigned int sphereEBO = 0;
unsigned int indexCount;
void renderSphere()
{
    // initialize (if necessary)
    if (sphereVAO == 0)
    {
        vector<vec3> positions;
        vector<vec2> uv;
        vector<vec3> normals;
        vector<unsigned int> indices;

        float radius = 1.0f;
        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359;
        
        for (int j = 0; j <= Y_SEGMENTS; ++j)
        {
            for (int i = 0; i <= X_SEGMENTS; ++i)
            {
                float xSegement = (float)i / (float)X_SEGMENTS;
                float ySegement = (float)j / (float)Y_SEGMENTS;
                float xPos = radius * cos(xSegement * 2.0f * PI) * sin(ySegement * PI);
                float yPos = radius * cos(ySegement * PI);
                float zPos = radius * sin(xSegement * 2.0f * PI) * sin(ySegement * PI);
                
                positions.push_back(vec3(xPos, yPos, zPos));
                uv.push_back(vec2(xSegement, ySegement));
                normals.push_back(vec3(xPos, yPos, zPos));
            }
        }
        
        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        indexCount = (unsigned int)indices.size();
        
        vector<float> data;
        for (int i = 0; i < positions.size(); i++)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
        }
        
        glGenVertexArrays(1, &sphereVAO);
        glGenBuffers(1, &sphereVBO);
        glGenBuffers(1, &sphereEBO);
        glBindVertexArray(sphereVAO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        // link vertex attributes
        float stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        default:
            break;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
//    cout<<"x = "<<xpos<<"  ,  y = "<<ypos<<endl;
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
