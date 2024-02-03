//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "directionLight.h"
#include "sphere.h"
#include "spotLight.h"
#include "cube.h"
#include "stb_image.h"
#include <iostream>
#include <time.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void board(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
void dice_roll(float& dice_number, bool& turnEnd, bool& cardshown, int& dice_roll_number, bool& player1_turn, bool& player2_turn, bool& community_chest_b, bool& chance_b, bool& income_tax_b, bool& luxury_tax_b, bool& gameend, bool &dice_flag);
void showcard(bool& cardshown);
void table(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
void floor(Cube cube, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
long long nCr(int n, int r);
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L);
void drawCurveObj(unsigned int& bezierVAO_, Shader& lightingShader, glm::mat4 model, float r, float g, float b, float shine);
unsigned int hollowBezier(GLfloat ctrlpoints[], int L, vector <float>& coordinates,
    vector <float>& normals,
    vector <int>& indices,
    vector <float>& vertices);

//curve

vector <float> lightpoints = {
-1.0750, 2.0650, 5.1000,
-1.0850, 2.0050, 5.1000,
-1.0650, 1.8900, 5.1000,
-1.0150, 1.7500, 5.1000,
-0.9800, 1.6350, 5.1000,
-0.8950, 1.5300, 5.1000,
-0.7800, 1.4600, 5.1000,
-0.6350, 1.4050, 5.1000,
-0.5300, 1.3850, 5.1000,
-0.3750, 1.3550, 5.1000,
-0.3050, 1.3550, 5.1000,
-0.2300, 1.3550, 5.1000,
};

vector <float> hatpoints = {
-1.0100, 0.7400, 5.1000,
-0.9600, 0.7400, 5.1000,
-0.9050, 0.7350, 5.1000,
-0.8400, 0.7350, 5.1000,
-0.7900, 0.7350, 5.1000,
-0.7300, 0.7350, 5.1000,
-0.6150, 0.7350, 5.1000,
-0.5600, 0.7300, 5.1000,
-0.5150, 0.7300, 5.1000,
-0.4750, 0.7650, 5.1000,
-0.4550, 0.8000, 5.1000,
-0.4450, 0.8800, 5.1000,
-0.4350, 0.9250, 5.1000,
-0.4350, 0.9950, 5.1000,
-0.4150, 1.1200, 5.1000,
-0.4100, 1.2250, 5.1000,
-0.4100, 1.3200, 5.1000,
-0.4000, 1.4000, 5.1000,
-0.3900, 1.4600, 5.1000,
-0.3800, 1.5300, 5.1000,
-0.3300, 1.6050, 5.1000,
-0.2950, 1.6400, 5.1000,
-0.2350, 1.6700, 5.1000,
-0.1400, 1.6800, 5.1000,
-0.0700, 1.6850, 5.1000,
-0.0300, 1.6900, 5.1000,
0.0150, 1.6900, 5.1000,
0.0600, 1.6900, 5.1000,
0.1050, 1.6900, 5.1000,
0.1900, 1.6700, 5.1000,
};
vector <float> coordinates;
vector <float> normals;
vector <int> indices;
vector <float> vertices;

vector <float> hatCoordinates;
vector <float> hatNormals;
vector <int> hatIndices;
vector <float> hatVertices;

class point
{
public:
    point()
    {
        x = 0;
        y = 0;
    }
    int x;
    int y;
} clkpt[2];

const double pi = 3.14159265389;
const int nt = 40;
const int ntheta = 20;

bool loadBezierCurvePoints = true;
bool showHollowBezier = true;
bool lineMode = false;
unsigned int bezierVAO;
unsigned int bezierVAO2;
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;

//diceTranslate
float translate_diceX = 0.5f;
float translate_diceY = 1.2f;
float translate_diceZ = -3.5f;
// camera
Camera camera(glm::vec3(0.0f, 3.5f, 5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 10.0, eyeZ = 1.0;
float lookAtX = 0.0, lookAtY = 15.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);
SpotLight spotlight(
    -2.1f, 1.0f, -0.5f,
    0.0f, -1.0f, 0.0f,
    .2f, .2f, .2f,
    .8f, .8f, .8f,
    1.0f, 1.0f, 1.0f,
    15.5f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);

DirectionLight directlight(
    0.0f, 1.0f, 0.0f,
    .2f, .2f, .2f,
    .8f, .8f, .8f,
    1.0f, 1.0f, 1.0f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);
// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(5.50f,  5.3f,  0.0f),
    glm::vec3(-1.5f,  5.3f,  0.0f),
    glm::vec3(1.5f,  2.5f,  -3.0f),
    glm::vec3(-1.5f,  2.5f,  -3.0f)
};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);


// light settings
bool pointLightOn = true;
bool directionLighton = true;
bool spotlighton = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
//turnbased

bool turnEnd = true;
bool cardshown = false;
bool player1_turn = false;
bool player2_turn = true;
bool dice_flag = true;
//dice_number
float dice_number ; 
bool gameend = false;
int count = 0;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;
float dice_speed = 0.0f;
//money
int player1_money = 2000;
int player2_money = 2000;

//property_price
int player1_property_price = 0;
int player2_property_price = 0;
//price of all property
int med_value = 60;
int baltic_value = 60;
int income_tax = 200;
int reading_rr_value = 200;
int oriental_value = 100;
int vermont_value = 100;
int connect_value = 120;
int charles_value = 140;
int electric_value = 150;
int state_value = 140;
int virginia_value = 160;
int penn_rr_value = 200;
int james_value = 180, ten_value = 180, newyork_value = 200, kentucky_value = 220, indiana_value = 220, illinois_value = 240, bo_rr_value = 200, atlanta_value = 260, ventnor_value = 260, waterwork_value = 150, marvin_value = 280;
int pacific_value = 300, carolina_value = 300, penn_value = 320, shortline_rr_value = 200, park_value = 350, board_value = 400, luxury_tax = 100;
int community_chest = 100;
int chance = 100;

//propertybought or not
bool med_b = false, baltic_b = false, reading_rr_b = false, oriental_b = false, vermont_b = false, connect_b = false;
bool charles_b = false, electric_b = false, state_b = false, virginia_b = false, penn_rr_b = false, james_b = false, ten_b = false, newyork_b = false;
bool kentucky_b = false, indiana_b = false, illinois_b = false, bo_rr_b = false, atlanta_b = false, ventnor_b = false, waterwork_b = false, marvin_b = false;
bool pacific_b = false, carolina_b = false, penn_b = false, short_rr_b = false, park_b = false, board_b = false;
bool community_chest_b = false;
bool chance_b = false;
bool income_tax_b = false;
bool luxury_tax_b = false;
//rent
int med_rent = 2;
int baltic_rent = 4;
int reading_rr_rent = 25;
int oriental_rent = 6;
int vermont_rent = 6;
int connect_rent = 8;
int charles_rent = 10;
int electric_rent = 10;
int state_rent = 10;
int virginia_rent = 12;
int penn_rr_rent = 25;
int james_rent = 14, ten_rent = 14, newyork_rent = 16, kentucky_rent = 18, indiana_rent = 18, illinois_rent = 20, bo_rr_rent = 25, atlanta_rent = 22, ventnor_rent = 22, waterwork_rent = 10, marvin_rent = 24;
int pacific_rent = 26, carolina_rent = 26, penn_rent = 28, shortline_rr_rent = 25, park_rent = 35, board_rent = 50;

int dice_roll_number;
int main()
{   
    
    while(!gameend){

        cout << dice_number << endl;
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Sphere sphere = Sphere();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    
    

    //board
    string diffuseMapPath = "monopoly.jpg";
    string specularMapPath = "Black.png";


    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //floor
    string diffuseMapPath_floor = "floor2.png";
    string specularMapPath_floor = "Black.png";


    unsigned int diffMap_floor = loadTexture(diffuseMapPath_floor.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_floor = loadTexture(specularMapPath_floor.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube floor1 = Cube(diffMap_floor, specMap_floor, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    //sidewall
    string diffuseMapPath_sd = "sidewall.png";
    string specularMapPath_sd = "Black.png";


    unsigned int diffMap_sd = loadTexture(diffuseMapPath_sd.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_sd = loadTexture(specularMapPath_sd.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube sidewall = Cube(diffMap_sd, specMap_sd, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    //ceiling
    string diffuseMapPath_ce = "ceiling.png";
    string specularMapPath_ce = "Black.png";


    unsigned int diffMap_ce = loadTexture(diffuseMapPath_ce.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_ce = loadTexture(specularMapPath_ce.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube ceiling = Cube(diffMap_ce, specMap_ce, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    //medeterian card
    string diffuseMapPath_med = "meddeterian.png";
    string specularMapPath_med = "Black.png";

    
    unsigned int diffMap_med = loadTexture(diffuseMapPath_med.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_med = loadTexture(specularMapPath_med.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube medeterian = Cube(diffMap_med, specMap_med, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //baltic card
    string diffuseMapPath_bal = "baltic.png";
    string specularMapPath_bal = "Black.png";
    unsigned int diffMap_bal = loadTexture(diffuseMapPath_bal.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_bal = loadTexture(specularMapPath_bal.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube baltic = Cube(diffMap_bal, specMap_bal, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    //oriental
    string diffuseMapPath_or = "orientak.png";
    string specularMapPath_or = "Black.png";
    unsigned int diffMap_or = loadTexture(diffuseMapPath_or.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_or = loadTexture(specularMapPath_or.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube oriental = Cube(diffMap_or, specMap_or, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //vermont
    string diffuseMapPath_ver = "vermont.png";
    string specularMapPath_ver = "Black.png";
    unsigned int diffMap_ver = loadTexture(diffuseMapPath_ver.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_ver = loadTexture(specularMapPath_ver.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube vermont = Cube(diffMap_ver, specMap_ver, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //Connect
    string diffuseMapPath_cnn = "connect.png";
    string specularMapPath_cnn = "Black.png";
    unsigned int diffMap_cnn = loadTexture(diffuseMapPath_cnn.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_cnn = loadTexture(specularMapPath_cnn.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube connect = Cube(diffMap_cnn, specMap_cnn, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //charles
    string diffuseMapPath_char = "charles.png";
    string specularMapPath_char = "Black.png";
    unsigned int diffMap_char = loadTexture(diffuseMapPath_char.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_char = loadTexture(specularMapPath_char.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube charles = Cube(diffMap_char, specMap_char, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    //state
    string diffuseMapPath_st = "states.png";
    string specularMapPath_st = "Black.png";
    unsigned int diffMap_st = loadTexture(diffuseMapPath_st.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_st = loadTexture(specularMapPath_st.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube state = Cube(diffMap_st, specMap_st, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //virgina
    string diffuseMapPath_vr = "virginia.png";
    string specularMapPath_vr = "Black.png";
    unsigned int diffMap_vr = loadTexture(diffuseMapPath_vr.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_vr = loadTexture(specularMapPath_vr.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube virginia = Cube(diffMap_vr, specMap_vr, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    //james
    string diffuseMapPath_jm = "james.png";
    string specularMapPath_jm = "Black.png";
    unsigned int diffMap_jm = loadTexture(diffuseMapPath_jm.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_jm = loadTexture(specularMapPath_jm.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube james = Cube(diffMap_jm, specMap_jm, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //tenesee
    string diffuseMapPath_tn = "tennesse.png";
    string specularMapPath_tn = "Black.png";
    unsigned int diffMap_tn = loadTexture(diffuseMapPath_tn.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_tn = loadTexture(specularMapPath_tn.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube tenesee = Cube(diffMap_tn, specMap_tn, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //newyork
    string diffuseMapPath_ny = "newyork.png";
    string specularMapPath_ny = "Black.png";
    unsigned int diffMap_ny = loadTexture(diffuseMapPath_ny.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_ny = loadTexture(specularMapPath_ny.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube newyork = Cube(diffMap_ny, specMap_ny, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    

    //kentucky
    string diffuseMapPath_kn = "kentucky.png";
    string specularMapPath_kn = "Black.png";
    unsigned int diffMap_kn = loadTexture(diffuseMapPath_kn.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_kn = loadTexture(specularMapPath_kn.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube kentucky = Cube(diffMap_kn, specMap_kn, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //indiana
    string diffuseMapPath_in = "indiana.png";
    string specularMapPath_in = "Black.png";
    unsigned int diffMap_in = loadTexture(diffuseMapPath_in.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_in = loadTexture(specularMapPath_in.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube indiana = Cube(diffMap_in, specMap_in, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //illinois
    string diffuseMapPath_il = "illinois.png";
    string specularMapPath_il = "Black.png";
    unsigned int diffMap_il = loadTexture(diffuseMapPath_il.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_il = loadTexture(specularMapPath_il.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube illinois= Cube(diffMap_il, specMap_il, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //atlanta
    string diffuseMapPath_at = "atlantic.png";
    string specularMapPath_at = "Black.png";
    unsigned int diffMap_at = loadTexture(diffuseMapPath_at.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_at = loadTexture(specularMapPath_at.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube atlanta = Cube(diffMap_at, specMap_at, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //ventnor
    string diffuseMapPath_vt = "ventnor.png";
    string specularMapPath_vt = "Black.png";
    unsigned int diffMap_vt = loadTexture(diffuseMapPath_vt.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_vt = loadTexture(specularMapPath_vt.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube ventnor = Cube(diffMap_vt, specMap_vt, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //marvin
    string diffuseMapPath_mv = "marvin.png";
    string specularMapPath_mv = "Black.png";
    unsigned int diffMap_mv = loadTexture(diffuseMapPath_mv.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_mv = loadTexture(specularMapPath_mv.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube marvin = Cube(diffMap_mv, specMap_mv, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //pacific
    string diffuseMapPath_pc = "pacific.png";
    string specularMapPath_pc = "Black.png";
    unsigned int diffMap_pc = loadTexture(diffuseMapPath_pc.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_pc = loadTexture(specularMapPath_pc.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube pacific = Cube(diffMap_pc, specMap_pc, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //north
    string diffuseMapPath_cr = "carolina.png";
    string specularMapPath_cr = "Black.png";
    unsigned int diffMap_cr = loadTexture(diffuseMapPath_cr.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_cr = loadTexture(specularMapPath_cr.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube carolina = Cube(diffMap_cr, specMap_cr, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    //penselv
    string diffuseMapPath_pn = "pennsyl.png";
    string specularMapPath_pn = "Black.png";
    unsigned int diffMap_pn = loadTexture(diffuseMapPath_pn.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_pn = loadTexture(specularMapPath_pn.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube penselv = Cube(diffMap_pn, specMap_pn, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //park place
    string diffuseMapPath_pk = "park place.png";
    string specularMapPath_pk = "Black.png";
    unsigned int diffMap_pk = loadTexture(diffuseMapPath_pk.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_pk = loadTexture(specularMapPath_pk.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube park = Cube(diffMap_pk, specMap_pk, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    //boardwalk
    string diffuseMapPath_bd = "board.png";
    string specularMapPath_bd = "Black.png";
    unsigned int diffMap_bd = loadTexture(diffuseMapPath_bd.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_bd = loadTexture(specularMapPath_bd.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube boardwalk = Cube(diffMap_bd, specMap_bd, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    
    //reading_rr
    string diffuseMapPath_read = "reading_rr.png";
    string specularMapPath_read = "Black.png";
    unsigned int diffMap_read = loadTexture(diffuseMapPath_read.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_read = loadTexture(specularMapPath_read.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube read = Cube(diffMap_read, specMap_read, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //pen_rr
    string diffuseMapPath_pen = "pennRail.png";
    string specularMapPath_pen = "Black.png";
    unsigned int diffMap_pen = loadTexture(diffuseMapPath_pen.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_pen = loadTexture(specularMapPath_pen.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube penRail = Cube(diffMap_pen, specMap_pen, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    //B&O
    string diffuseMapPath_bo = "pennRail.png";
    string specularMapPath_bo = "Black.png";
    unsigned int diffMap_bo = loadTexture(diffuseMapPath_bo.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_bo = loadTexture(specularMapPath_bo.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube B_rail = Cube(diffMap_pen, specMap_pen, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    //Short
    string diffuseMapPath_srt = "short.png";
    string specularMapPath_srt = "Black.png";
    unsigned int diffMap_srt = loadTexture(diffuseMapPath_srt.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_srt = loadTexture(specularMapPath_srt.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube Short = Cube(diffMap_srt, specMap_srt, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    //water works
    string diffuseMapPath_ww = "water works.png";
    string specularMapPath_ww = "Black.png";
    unsigned int diffMap_ww = loadTexture(diffuseMapPath_ww.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_ww = loadTexture(specularMapPath_ww.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube WaterWork = Cube(diffMap_ww, specMap_ww, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //electric

    string diffuseMapPath_ee = "electric.png";
    string specularMapPath_ee = "Black.png";
    unsigned int diffMap_ee = loadTexture(diffuseMapPath_ee.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_ee = loadTexture(specularMapPath_ee.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube electric = Cube(diffMap_ee, specMap_ee, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //dice
    string diffuseMapPath_dice = "whiteBackground.png";
    string specularMapPath_dice = "Black.png";
    unsigned int diffMap_dice = loadTexture(diffuseMapPath_dice.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_dice = loadTexture(specularMapPath_dice.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dice = Cube(diffMap_dice, specMap_dice, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    //dice1
    string diffuseMapPath_dice1 = "dice1.png";
    string specularMapPath_dice1 = "Black.png";
    unsigned int diffMap_dice1 = loadTexture(diffuseMapPath_dice1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_dice1 = loadTexture(specularMapPath_dice1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dice1 = Cube(diffMap_dice1, specMap_dice1, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    //dice2
    string diffuseMapPath_dice2 = "dice2.png";
    string specularMapPath_dice2 = "Black.png";
    unsigned int diffMap_dice2 = loadTexture(diffuseMapPath_dice2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_dice2 = loadTexture(specularMapPath_dice2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dice2 = Cube(diffMap_dice2, specMap_dice2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    //dice3
    string diffuseMapPath_dice3 = "dice3.png";
    string specularMapPath_dice3 = "Black.png";
    unsigned int diffMap_dice3 = loadTexture(diffuseMapPath_dice3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_dice3 = loadTexture(specularMapPath_dice3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dice3 = Cube(diffMap_dice3, specMap_dice3, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath_dice4 = "dice4.png";
    string specularMapPath_dice4 = "Black.png";
    unsigned int diffMap_dice4 = loadTexture(diffuseMapPath_dice4.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_dice4 = loadTexture(specularMapPath_dice4.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dice4 = Cube(diffMap_dice4, specMap_dice4, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath_dice5 = "dice5.png";
    string specularMapPath_dice5 = "Black.png";
    unsigned int diffMap_dice5 = loadTexture(diffuseMapPath_dice5.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_dice5 = loadTexture(specularMapPath_dice5.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dice5 = Cube(diffMap_dice5, specMap_dice5, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath_dice6 = "dice6.png";
    string specularMapPath_dice6 = "Black.png";
    unsigned int diffMap_dice6 = loadTexture(diffuseMapPath_dice6.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_dice6 = loadTexture(specularMapPath_dice6.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube dice6 = Cube(diffMap_dice6, specMap_dice6, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    //bezier LIGHTS
    bezierVAO = hollowBezier(lightpoints.data(), ((unsigned int)lightpoints.size() / 3) - 1, coordinates, normals, indices, vertices);
    loadBezierCurvePoints = false;
    showHollowBezier = true;

    bezierVAO2 = hollowBezier(hatpoints.data(), ((unsigned int)hatpoints.size() / 3) - 1, hatCoordinates, hatNormals, hatIndices, hatVertices);
    loadBezierCurvePoints = false;
    showHollowBezier = true;
    //ourShader.use();
    //lightingShader.use();

    // render loop
    // -----------
    
    float r = 0.0f;
    float translate_red_ball_x_left = -1.2f;
    float translate_red_ball_z_up = -1.3f;
    float translate_red_ball_x_right = 1.2f;
    float translate_red_ball_z_down = 1.1f;
    float translate_blue_cube_x_left = -1.2f;
    float translate_blue_cube_z_up = -1.3f;
    float translate_blue_cube_x_right = 1.2f;
    float translate_blue_cube_z_down = 1.1f;


    
    while (!glfwWindowShouldClose(window))
    {   
        
        
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);

        directlight.setUpDirectionLight(lightingShader);

        spotlight.setUpSpotLight(lightingShader);

        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        board(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.75f, -0.3f, -1.5f));
        
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        table(cubeVAO, lightingShader, model);

        //glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 36);


        //SphereRed
        //for lamps
        //pointlight1
        lightingShader.use();
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(pointLightPositions[0].x + 0.05f, pointLightPositions[0].y - 0.065, pointLightPositions[0].z + 0.05f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, 0.4f, 0.4f));
        glm::mat4 model_for_pointLight = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

        drawCurveObj(bezierVAO, lightingShader, model_for_pointLight, 0.545f, 0.278f, 0.051f, 32.0f);
        //pontlight2
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(pointLightPositions[1].x + 0.05f, pointLightPositions[1].y - 0.065, pointLightPositions[1].z + 0.05f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, 0.4f, 0.4f));
        model_for_pointLight = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

        drawCurveObj(bezierVAO, lightingShader, model_for_pointLight, 0.545f, 0.278f, 0.051f, 32.0f);

        //sphere_red
        
        if (player1_turn) {
            glm::mat4 modelForSphere = glm::mat4(1.0f);
            if (translate_red_ball_x_left >= 1.1f && translate_red_ball_z_up >= 1.0f && translate_red_ball_x_right <= -1.1f && translate_red_ball_z_down <= -1.2f)
            {
                translate_red_ball_x_left = -1.2f;
                translate_red_ball_z_up = -1.3f;
                translate_red_ball_x_right = 1.2f;
                translate_red_ball_z_down = 1.1f;
            }
            if (translate_red_ball_x_left <= 1.2f) {
                if (translate_red_ball_x_left == 1.2f)
                {
                    dice_number = dice_number - r;
                }
                else if (!turnEnd) {
                    if (r >= dice_number) {
                        translate_red_ball_x_left = translate_red_ball_x_left;
                        turnEnd = true;
                        r = 0.0f;
                    }
                    else if (r < dice_number) {
                        r = r + 0.01f;
                        translate_red_ball_x_left = translate_red_ball_x_left + 0.01f;
                        turnEnd = false;
                    }
                }
                translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_red_ball_x_left, 0.16f, -1.3f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.06f, 0.06f, 0.06f));
                modelForSphere = translateMatrix * scaleMatrix;
                sphere.drawSphere(lightingShader, modelForSphere);
            }
            if (translate_red_ball_x_left > 1.2f && translate_red_ball_z_up <= 1.1f) {
                if (translate_red_ball_z_up == 1.1f)
                {
                    dice_number = dice_number - r;
                }


                else if (!turnEnd) {
                    if (r >= dice_number) {
                        translate_red_ball_z_up = translate_red_ball_z_up;
                        turnEnd = true;
                        r = 0.0f;
                    }
                    else if (r < dice_number) {
                        r = r + 0.01f;
                        translate_red_ball_z_up = translate_red_ball_z_up + 0.01f;
                        turnEnd = false;
                    }
                }
                translateMatrix = glm::translate(identityMatrix, glm::vec3(1.2f, 0.16f, translate_red_ball_z_up));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.06f, 0.06f, 0.06f));
                modelForSphere = translateMatrix * scaleMatrix;
                sphere.drawSphere(lightingShader, modelForSphere);
            }
            if (translate_red_ball_z_up > 1.1f && translate_red_ball_x_right >= -1.2f) {
                if (translate_red_ball_x_right == -1.2f)
                {
                    dice_number = dice_number - r;
                }


                else if (!turnEnd) {
                    if (r >= dice_number) {
                        translate_red_ball_x_right = translate_red_ball_x_right;
                        turnEnd = true;
                        r = 0.0f;
                    }
                    else if (r < dice_number) {
                        r = r + 0.01f;
                        translate_red_ball_x_right = translate_red_ball_x_right - 0.01f;
                        turnEnd = false;
                    }
                }
                translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_red_ball_x_right, 0.16f, 1.1f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.06f, 0.06f, 0.06f));
                modelForSphere = translateMatrix * scaleMatrix;
                sphere.drawSphere(lightingShader, modelForSphere);
            }
            if (translate_red_ball_x_right < -1.2f && translate_red_ball_z_down >= -1.3f) {
                if (translate_red_ball_z_down == -1.2f)
                {
                    dice_number = dice_number - r;
                }
                

                if (!turnEnd) {
                    if (r >= dice_number) {
                        translate_red_ball_z_down = translate_red_ball_z_down;
                        turnEnd = true;
                        r = 0.0f;
                    }
                    else if (r < dice_number) {
                        r = r + 0.01f;
                        translate_red_ball_z_down = translate_red_ball_z_down - 0.01f;
                        turnEnd = false;
                    }
                }
                translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.2f, 0.16f, translate_red_ball_z_down));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.06f, 0.06f, 0.06f));
                modelForSphere = translateMatrix * scaleMatrix;
                sphere.drawSphere(lightingShader, modelForSphere);
            }
        }
        if (!player1_turn) {
            glm::mat4 modelForSphere = glm::mat4(1.0f);
            if (translate_red_ball_x_left <= 1.2f) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_red_ball_x_left, 0.16f, -1.3f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.06f, 0.06f, 0.06f));
                modelForSphere = translateMatrix * scaleMatrix;
                sphere.drawSphere(lightingShader, modelForSphere);
            }
            if (translate_red_ball_x_left > 1.2f && translate_red_ball_z_up <= 1.1f) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(1.2f, 0.16f, translate_red_ball_z_up));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.06f, 0.06f, 0.06f));
                modelForSphere = translateMatrix * scaleMatrix;
                sphere.drawSphere(lightingShader, modelForSphere);
            }
            if (translate_red_ball_z_up > 1.1f && translate_red_ball_x_right >= -1.2f) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_red_ball_x_right, 0.16f, 1.1f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.06f, 0.06f, 0.06f));
                modelForSphere = translateMatrix * scaleMatrix;
                sphere.drawSphere(lightingShader, modelForSphere);
            }
            if (translate_red_ball_x_right < -1.2f && translate_red_ball_z_down >= -1.3f) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.2f, 0.16f, translate_red_ball_z_down));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.06f, 0.06f, 0.06f));
                modelForSphere = translateMatrix * scaleMatrix;
                sphere.drawSphere(lightingShader, modelForSphere);
            }


        }
        //hat blue
        if (player2_turn) {
            glm::mat4 modelForBlueCube = glm::mat4(1.0f);
            if (translate_blue_cube_x_left >= 1.1f && translate_blue_cube_z_up >= 1.0f && translate_blue_cube_x_right <= -1.1f && translate_blue_cube_z_down <= -1.2f)
            {
                translate_blue_cube_x_left = -1.2f;
                translate_blue_cube_z_up = -1.3f;
                translate_blue_cube_x_right = 1.2f;
                translate_blue_cube_z_down = 1.1f;
            }
            if (translate_blue_cube_x_left <= 1.2f) {
                if (translate_blue_cube_x_left == 1.2f)
                {
                    dice_number = dice_number - r;
                }
                else if (!turnEnd) {
                    if (r >= dice_number) {
                        translate_blue_cube_x_left = translate_blue_cube_x_left;
                        turnEnd = true;
                        r = 0.0f;
                    }
                    else if (r < dice_number) {
                        r = r + 0.01f;
                        translate_blue_cube_x_left = translate_blue_cube_x_left + 0.01f;
                        turnEnd = false;
                    }
                }
                translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_blue_cube_x_left, 0.06f, -1.3f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.08f, 0.08f, 0.08f));
                modelForBlueCube = translateMatrix * scaleMatrix;
                //sphere.drawSphere(lightingShader, modelForBlueCube);
                drawCurveObj(bezierVAO2, lightingShader, modelForBlueCube, 0.0f, 0.0f, 1.0f, 32.0f);
            }
            if (translate_blue_cube_x_left > 1.2f && translate_blue_cube_z_up <= 1.1f) {
                if (translate_blue_cube_z_up == 1.1f)
                {
                    dice_number = dice_number - r;
                }


                else if (!turnEnd) {
                    if (r >= dice_number) {
                        translate_blue_cube_z_up = translate_blue_cube_z_up;
                        turnEnd = true;
                        r = 0.0f;
                    }
                    else if (r < dice_number) {
                        r = r + 0.01f;
                        translate_blue_cube_z_up = translate_blue_cube_z_up + 0.01f;
                        turnEnd = false;
                    }
                }
                translateMatrix = glm::translate(identityMatrix, glm::vec3(1.2f, 0.06f, translate_blue_cube_z_up));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.08f, 0.08f, 0.08f));
                modelForBlueCube = translateMatrix * scaleMatrix;
                drawCurveObj(bezierVAO2, lightingShader, modelForBlueCube, 0.0f, 0.0f, 1.0f, 32.0f);
            }
            if (translate_blue_cube_z_up > 1.1f && translate_blue_cube_x_right >= -1.2f) {
                if (translate_blue_cube_x_right == -1.2f)
                {
                    dice_number = dice_number - r;
                }


                else if (!turnEnd) {
                    if (r >= dice_number) {
                        translate_blue_cube_x_right = translate_blue_cube_x_right;
                        turnEnd = true;
                        r = 0.0f;
                    }
                    else if (r < dice_number) {
                        r = r + 0.01f;
                        translate_blue_cube_x_right = translate_blue_cube_x_right - 0.01f;
                        turnEnd = false;
                    }
                }
                translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_blue_cube_x_right, 0.06f, 1.1f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.08f, 0.08f, 0.08f));
                modelForBlueCube = translateMatrix * scaleMatrix;
                drawCurveObj(bezierVAO2, lightingShader, modelForBlueCube, 0.0f, 0.0f, 1.0f, 32.0f);
            }
            if (translate_blue_cube_x_right < -1.2f && translate_blue_cube_z_down >= -1.3f) {
                if (translate_blue_cube_z_down == -1.2f)
                {
                    dice_number = dice_number - r;
                }
                

                if (!turnEnd) {
                    if (r >= dice_number) {
                        translate_blue_cube_z_down = translate_blue_cube_z_down;
                        turnEnd = true;
                        r = 0.0f;
                    }
                    else if (r < dice_number) {
                        r = r + 0.01f;
                        translate_blue_cube_z_down = translate_blue_cube_z_down - 0.01f;
                        turnEnd = false;
                    }
                }
                translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.2f, 0.06f, translate_blue_cube_z_down));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.08f, 0.08f, 0.08f));
                modelForBlueCube = translateMatrix * scaleMatrix;
                drawCurveObj(bezierVAO2, lightingShader, modelForBlueCube, 0.0f, 0.0f, 1.0f, 32.0f);
            }
        }

        if (!player2_turn) {
            glm::mat4 modelForBlueCube = glm::mat4(1.0f);
            if (translate_blue_cube_x_left <= 1.2f) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_blue_cube_x_left, 0.06f, -1.3f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.08f, 0.08f, 0.08f));
                modelForBlueCube = translateMatrix * scaleMatrix;
                drawCurveObj(bezierVAO2, lightingShader, modelForBlueCube, 0.0f, 0.0f, 1.0f, 32.0f);
            }
            if (translate_blue_cube_x_left > 1.2f && translate_blue_cube_z_up <= 1.1f) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(1.2f, 0.06f, translate_blue_cube_z_up));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.08f, 0.08f, 0.08f));
                modelForBlueCube = translateMatrix * scaleMatrix;
                drawCurveObj(bezierVAO2, lightingShader, modelForBlueCube, 0.0f, 0.0f, 1.0f, 32.0f);
            }
            if (translate_blue_cube_z_up > 1.1f && translate_blue_cube_x_right >= -1.2f) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_blue_cube_x_right, 0.06f, 1.1f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.08f, 0.08f, 0.08f));
                modelForBlueCube = translateMatrix * scaleMatrix;
                drawCurveObj(bezierVAO2, lightingShader, modelForBlueCube, 0.0f, 0.0f, 1.0f, 32.0f);
            }
            if (translate_blue_cube_x_right < -1.2f && translate_blue_cube_z_down >= -1.3f) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.2f, 0.06f, translate_blue_cube_z_down));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.08f, 0.08f, 0.08f));
                modelForBlueCube = translateMatrix * scaleMatrix;
                drawCurveObj(bezierVAO2, lightingShader, modelForBlueCube, 0.0f, 0.0f, 1.0f, 32.0f);
            }

        }
        

        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        //glBindVertexArray(lightCubeVAO);
        //for (unsigned int i = 0; i < 4; i++)
        //{
        //    model = glm::mat4(1.0f);
        //    model = glm::translate(model, pointLightPositions[i]);
        //    model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        //    ourShader.setMat4("model", model);
        //    ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
        //    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //    //glDrawArrays(GL_TRIANGLES, 0, 36);
        //}

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);

        // pass projection matrix to shader (note that in this case it could change every frame)
        //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShaderWithTexture.setMat4("projection", projection);

        // camera/view transformation
        //glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShaderWithTexture.setMat4("view", view);
        lightingShaderWithTexture.use();
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);

        directlight.setUpDirectionLight(lightingShaderWithTexture);

        spotlight.setUpSpotLight(lightingShaderWithTexture);

        float baseHeight = 0.1;
        float width = 3;
        float length = 3;

        //board
        
        glm::mat4 x = glm::mat4(1.0f);


         // make sure to initialize matrix to identity matrix first
        glm::mat4 translateM, rotateX,scaleM, rotateY,rotateZ,translate_sidewall1;
        /*translate = glm::translate(x, glm::vec3(-0.5, 0, -0.5));
        scale = glm::scale(x, glm::vec3(width, baseHeight, length));
        rotateX = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));*/

        translateM = glm::translate(identityMatrix, glm::vec3(1.5f, 0.0f, 1.5f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(width, baseHeight, length));
        
        
        x = translateM * rotateX * scaleM;
        cube.drawCubeWithTexture(lightingShaderWithTexture, x);

        //floor
        glm::mat4 model_floor = glm::mat4(1.0f);
        
        
        floor(floor1, lightingShaderWithTexture, model_floor);

        //sidewall
        glm::mat4 model_sidewall = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(7.0f, 3.6f, 8.4f));
        rotateX = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateZ = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 0.5f));
        model_sidewall = translateM * rotateX * rotateZ * scaleM;
        floor(sidewall, lightingShaderWithTexture, model_sidewall);
        //sidewall1
        translate_sidewall1 = glm::translate(identityMatrix, glm::vec3(-9.4f, -2.5f, 0.0f));
        rotateY = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        
        glm::mat4 model_sidewall1 = glm::mat4(1.0f);
        model_sidewall1 = model_sidewall * rotateY * translate_sidewall1;
        floor(sidewall, lightingShaderWithTexture, model_sidewall1);
        //sidewall2
        translate_sidewall1 = glm::translate(identityMatrix, glm::vec3(2.4f, -9.5f, 0.0f));
        rotateY = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        glm::mat4 model_sidewall2 = glm::mat4(1.0f);
        model_sidewall2 = model_sidewall * rotateY * translate_sidewall1;
        floor(sidewall, lightingShaderWithTexture, model_sidewall2);

        //ceiling
        translate_sidewall1 = glm::translate(identityMatrix, glm::vec3(0.0f, -3.5f, -8.4f));
        rotateX = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 model_ceiling = glm::mat4(1.0f);
        model_ceiling = model_sidewall * rotateX * translate_sidewall1;
        floor(ceiling, lightingShaderWithTexture, model_ceiling);
        //medeterian card
        glm::mat4 model_med = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(1.5f, 0.1f, -1.7f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));
        
        model_med = translateM * rotateX * scaleM;
        medeterian.drawCubeWithTexture(lightingShaderWithTexture, model_med);

        //baltic card
        glm::mat4 model_bal = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(1.1f, 0.1f, -1.7f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_bal = translateM * rotateX * scaleM;
        baltic.drawCubeWithTexture(lightingShaderWithTexture, model_bal);

        //oriental
        glm::mat4 model_oriental = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(0.7f, 0.1f, -1.7f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_oriental = translateM * rotateX * scaleM;
        oriental.drawCubeWithTexture(lightingShaderWithTexture, model_oriental);

        //vermont
        glm::mat4 model_ver = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(0.3f, 0.1f, -1.7f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_ver = translateM * rotateX * scaleM;
        vermont.drawCubeWithTexture(lightingShaderWithTexture, model_ver);

        //conect
        glm::mat4 model_cnn = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(-0.1f, 0.1f, -1.7f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_cnn = translateM * rotateX * scaleM;
        connect.drawCubeWithTexture(lightingShaderWithTexture, model_cnn);

        //charles
        glm::mat4 model_char = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(-0.5f, 0.1f, -1.7f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_char = translateM * rotateX * scaleM;
        charles.drawCubeWithTexture(lightingShaderWithTexture, model_char);
        //states
        glm::mat4 model_st = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(-0.9f, 0.1f, -1.7f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_st = translateM * rotateX * scaleM;
        state.drawCubeWithTexture(lightingShaderWithTexture, model_st);

        //virginia
        glm::mat4 model_vr = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(1.5f, 0.1f, -2.1f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_vr = translateM * rotateX * scaleM;
        virginia.drawCubeWithTexture(lightingShaderWithTexture, model_vr);

        //james
        glm::mat4 model_jm = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(1.1f, 0.1f, -2.1f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_jm = translateM * rotateX * scaleM;
        james.drawCubeWithTexture(lightingShaderWithTexture, model_jm);

        //tenesee
        glm::mat4 model_tn = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(0.7f, 0.1f, -2.1f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_tn = translateM * rotateX * scaleM;
        tenesee.drawCubeWithTexture(lightingShaderWithTexture, model_tn);

        //newyork
        glm::mat4 model_ny = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(0.3f, 0.1f, -2.1f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_ny = translateM * rotateX * scaleM;
        newyork.drawCubeWithTexture(lightingShaderWithTexture, model_ny);

        //kentucky
        glm::mat4 model_kn = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(-0.1f, 0.1f, -2.1f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_kn = translateM * rotateX * scaleM;
        kentucky.drawCubeWithTexture(lightingShaderWithTexture, model_kn);

        //indiana
        glm::mat4 model_in = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(-0.5f, 0.1f, -2.1f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_in = translateM * rotateX * scaleM;
        indiana.drawCubeWithTexture(lightingShaderWithTexture, model_in);

        //illinois
        glm::mat4 model_il = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(-0.9f, 0.1f, -2.1f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_il = translateM * rotateX * scaleM;
        illinois.drawCubeWithTexture(lightingShaderWithTexture, model_il);

        //atlanta
        glm::mat4 model_at = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(1.5f, 0.1f, -2.5f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_at = translateM * rotateX * scaleM;
        atlanta.drawCubeWithTexture(lightingShaderWithTexture, model_at);

        //ventnor
        glm::mat4 model_vt = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(1.1f, 0.1f, -2.5f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_vt = translateM * rotateX * scaleM;
        ventnor.drawCubeWithTexture(lightingShaderWithTexture, model_vt);

        //marvin
        glm::mat4 model_mv = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(0.7f, 0.1f, -2.5f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_mv = translateM * rotateX * scaleM;
        marvin.drawCubeWithTexture(lightingShaderWithTexture, model_mv);

        //pacific

        glm::mat4 model_pc = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(0.3f, 0.1f, -2.5f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_pc = translateM * rotateX * scaleM;
        pacific.drawCubeWithTexture(lightingShaderWithTexture, model_pc);


        //carolina
        glm::mat4 model_cr = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(-0.1f, 0.1f, -2.5f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));
        model_cr = translateM * rotateX * scaleM;
        carolina.drawCubeWithTexture(lightingShaderWithTexture, model_cr);

        //penselv
        glm::mat4 model_pn = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(-0.5f, 0.1f, -2.5f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));
        model_pn = translateM * rotateX * scaleM;
        penselv.drawCubeWithTexture(lightingShaderWithTexture, model_pn);

        //park
        glm::mat4 model_pk = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(-0.9f, 0.1f, -2.5f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));
        model_pk = translateM * rotateX * scaleM;
        park.drawCubeWithTexture(lightingShaderWithTexture, model_pk);

        //boardwalk
        glm::mat4 model_bd = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(1.5f, 0.1f, -2.9f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));
        model_bd = translateM * rotateX * scaleM;
        boardwalk.drawCubeWithTexture(lightingShaderWithTexture, model_bd);

        
        //reading_rr
        glm::mat4 model_read = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(1.1f, 0.1f, -2.9f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_read = translateM * rotateX * scaleM;
        read.drawCubeWithTexture(lightingShaderWithTexture, model_read);
        //penrail
        glm::mat4 model_pen = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(0.7f, 0.1f, -2.9f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_pen = translateM * rotateX * scaleM;
        penRail.drawCubeWithTexture(lightingShaderWithTexture, model_pen);

        //B&O
        glm::mat4 model_bo = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(0.3f, 0.1f, -2.9f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_bo = translateM * rotateX * scaleM;
        B_rail.drawCubeWithTexture(lightingShaderWithTexture, model_bo);

        //short
        glm::mat4 model_srt = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(-0.1f, 0.1f, -2.9f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_srt = translateM * rotateX * scaleM;
        Short.drawCubeWithTexture(lightingShaderWithTexture, model_srt);

        //water works
        glm::mat4 model_ww = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(-0.5f, 0.1f, -2.9f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_ww = translateM * rotateX * scaleM;
        WaterWork.drawCubeWithTexture(lightingShaderWithTexture, model_ww);

        //electric
        glm::mat4 model_ee = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(-0.9f, 0.1f, -2.9f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.3f));

        model_ee = translateM * rotateX * scaleM;
        electric.drawCubeWithTexture(lightingShaderWithTexture, model_ee);

        //dice
        if (dice_roll_number == 0) {
            glm::mat4 model_dice = glm::mat4(1.0f);
            translateM = glm::translate(identityMatrix, glm::vec3(0.5f, 1.2f, -3.5f));
            rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            scaleM = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 0.2f));

            model_dice = translateM * rotateX * scaleM;
            dice.drawCubeWithTexture(lightingShaderWithTexture, model_dice);
        }

        else if (dice_roll_number == 1) {
            
            /*if (dice_flag) {
                dice_speed = dice_speed + 0.1f;
                glm::mat4 model_dice1 = glm::mat4(1.0f);
                translate_diceX = translate_diceX - 0.01f;
                translateM = glm::translate(identityMatrix, glm::vec3(translate_diceX, translate_diceY, -3.5f));
                rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                scaleM = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 0.2f));

                model_dice1 = translateM * rotateX * scaleM;
                dice1.drawCubeWithTexture(lightingShaderWithTexture, model_dice1);
                dice_flag = false;
            }*/
            glm::mat4 model_dice = glm::mat4(1.0f);
            translateM = glm::translate(identityMatrix, glm::vec3(0.5f, 1.2f, -3.5f));
            rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            scaleM = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 0.2f));

            model_dice = translateM * rotateX * scaleM;
            dice1.drawCubeWithTexture(lightingShaderWithTexture, model_dice);
        }
        else if (dice_roll_number == 2) {
            glm::mat4 model_dice2 = glm::mat4(1.0f);
            translateM = glm::translate(identityMatrix, glm::vec3(0.5f, 1.2f, -3.5f));
            rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            scaleM = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 0.2f));

            model_dice2 = translateM * rotateX * scaleM;
            dice2.drawCubeWithTexture(lightingShaderWithTexture, model_dice2);
        }
        else if (dice_roll_number == 3) {
            glm::mat4 model_dice3 = glm::mat4(1.0f);
            translateM = glm::translate(identityMatrix, glm::vec3(0.5f, 1.2f, -3.5f));
            rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            scaleM = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 0.2f));

            model_dice3 = translateM * rotateX * scaleM;
            dice3.drawCubeWithTexture(lightingShaderWithTexture, model_dice3);
        }
        else if (dice_roll_number == 4) {
            glm::mat4 model_dice4 = glm::mat4(1.0f);
            translateM = glm::translate(identityMatrix, glm::vec3(0.5f, 1.2f, -3.5f));
            rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            scaleM = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 0.2f));

            model_dice4 = translateM * rotateX * scaleM;
            dice4.drawCubeWithTexture(lightingShaderWithTexture, model_dice4);
        }
        else if (dice_roll_number == 5) {
            glm::mat4 model_dice5 = glm::mat4(1.0f);
            translateM = glm::translate(identityMatrix, glm::vec3(0.5f, 1.2f, -3.5f));
            rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            scaleM = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 0.2f));

            model_dice5 = translateM * rotateX * scaleM;
            dice5.drawCubeWithTexture(lightingShaderWithTexture, model_dice5);
        }
        else if (dice_roll_number == 6) {
            glm::mat4 model_dice6 = glm::mat4(1.0f);
            translateM = glm::translate(identityMatrix, glm::vec3(0.5f, 1.2f, -3.5f));
            rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            scaleM = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 0.2f));

            model_dice6 = translateM * rotateX * scaleM;
            dice6.drawCubeWithTexture(lightingShaderWithTexture, model_dice6);
        }
        
        //front
        glm::mat4 model_front = glm::mat4(1.0f);
        translateM = glm::translate(identityMatrix, glm::vec3(0.5f, 2.0f, -2.2f));
        rotateX = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateY = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        scaleM = glm::scale(identityMatrix, glm::vec3(0.8f, 0.1f, 0.8f));

        model_front = translateM * rotateX * rotateY * scaleM;
        if (player1_turn) {
            if (turnEnd && cardshown) {
                if (translate_red_ball_x_left > -1.2f && translate_red_ball_x_left <= -1.2f + 2 * .22f)
                {   
                    medeterian.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!med_b) {
                        if (player1_money > med_value) {
                            player1_money = player1_money - med_value;
                            player1_property_price = player1_property_price + med_value;
                            med_b = true;
                        }
                    }
                    
                }
                else if (translate_red_ball_x_left > -1.2f + 2 * .22f && translate_red_ball_x_left <= -1.2f + 3 * .22f)
                {
                    if (!community_chest_b) {
                        player1_money = player1_money - community_chest;
                        community_chest_b = true;
                    }
                }
                else if (translate_red_ball_x_left > -1.2f + 3 * .22f && translate_red_ball_x_left <= -1.2f + 4 * .22f)
                {
                    baltic.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!baltic_b) {
                        if (player1_money > baltic_value) {
                            player1_money = player1_money - baltic_value;
                            player1_property_price = player1_property_price + baltic_value;
                            baltic_b = true;
                        }
                    }
                }
                else if (translate_red_ball_x_left > -1.2f + 4 * .22f && translate_red_ball_x_left <= -1.2f + 5 * .22f)
                {
                    if (!income_tax_b) {
                        player1_money = player1_money - income_tax;
                        income_tax_b = true;
                    }
                }

                else if (translate_red_ball_x_left > -1.2f + 5 * .22f && translate_red_ball_x_left <= -1.2f + 6 * .22f)
                {
                    read.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!reading_rr_b) {
                        if (player1_money > reading_rr_value) {
                            player1_money = player1_money - reading_rr_value;
                            player1_property_price = player1_property_price + reading_rr_value;
                            reading_rr_b = true;
                        }
                    }
                }
                else if (translate_red_ball_x_left > -1.2f + 6 * .22f && translate_red_ball_x_left <= -1.2f + 7 * .22f)
                {
                    oriental.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!oriental_b) {
                        if (player1_money > oriental_value) {
                            player1_money = player1_money - oriental_value;
                            player1_property_price = player1_property_price + oriental_value;
                            oriental_b = true;
                        }
                    }
                }
                else if (translate_red_ball_x_left > -1.2f + 7 * .22f && translate_red_ball_x_left <= -1.2f + 8 * .22f)
                {
                    if (!chance_b) {
                        player1_money = player1_money - chance;
                        chance_b = true;
                    }
                }
                else if (translate_red_ball_x_left > -1.2f + 8 * .22f && translate_red_ball_x_left <= -1.2f + 9 * .22f)
                {
                    vermont.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!vermont_b) {
                        if (player1_money > vermont_value) {
                            player1_money = player1_money - vermont_value;
                            player1_property_price = player1_property_price + vermont_value;
                            vermont_b = true;
                        }
                    }
                }
                else if (translate_red_ball_x_left > -1.2f + 9 * .22f && translate_red_ball_x_left <= -1.2f + 10 * .22f)
                {
                    connect.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!connect_b) {
                        if (player1_money > connect_value) {
                            player1_money = player1_money - connect_value;
                            player1_property_price = player1_property_price + connect_value;
                            connect_b = true;
                        }
                    }
                }
                else if (translate_red_ball_x_left > -1.2f + 10 * .22f && translate_red_ball_x_left <= -1.2f + 11 * .22f)
                {

                }
                if (translate_red_ball_z_up > -1.3f && translate_red_ball_z_up <= -1.3f + 2 * .23f)
                {
                    charles.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!charles_b) {
                        if (player1_money > charles_value) {
                            player1_money = player1_money - charles_value;
                            player1_property_price = player1_property_price + charles_value;
                            charles_b = true;
                        }
                    }
                }
                else if (translate_red_ball_z_up > -1.3f + 2 * .23f && translate_red_ball_z_up <= -1.3f + 3 * .23f)
                {
                    electric.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!electric_b) {
                        if (player1_money > electric_value) {
                            player1_money = player1_money - electric_value;
                            player1_property_price = player1_property_price + electric_value;
                            electric_b = true;
                        }
                    }
                }
                else if (translate_red_ball_z_up > -1.3f + 3 * .23f && translate_red_ball_z_up <= -1.3f + 4 * .23f)
                {
                    state.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!state_b) {
                        if (player1_money > state_value) {
                            player1_money = player1_money - state_value;
                            player1_property_price = player1_property_price + state_value;
                            state_b = true;
                        }
                    }
                }
                else if (translate_red_ball_z_up > -1.3f + 4 * .23f && translate_red_ball_z_up <= -1.3f + 5 * .23f)
                {
                    virginia.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!virginia_b) {
                        if (player1_money > virginia_value) {
                            player1_money = player1_money - virginia_value;
                            player1_property_price = player1_property_price + virginia_value;
                            virginia_b = true;
                        }
                    }
                }
                else if (translate_red_ball_z_up > -1.3f + 5 * .23f && translate_red_ball_z_up <= -1.3f + 6 * .23f)
                {
                    penRail.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!penn_rr_b) {
                        if (player1_money > penn_rr_value) {
                            player1_money = player1_money - penn_rr_value;
                            player1_property_price = player1_property_price + penn_rr_value;
                            penn_rr_b = true;
                        }
                    }
                }
                else if (translate_red_ball_z_up > -1.3f + 6 * .23f && translate_red_ball_z_up <= -1.3f + 7 * .23f)
                {
                    james.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!james_b) {
                        if (player1_money > james_value) {
                            player1_money = player1_money - james_value;
                            player1_property_price = player1_property_price + james_value;
                            james_b = true;
                        }
                    }
                }
                else if (translate_red_ball_z_up > -1.3f + 7 * .23f && translate_red_ball_z_up <= -1.3f + 8 * .23f)
                {   
                    if (!community_chest_b) {
                        player1_money = player1_money - community_chest;
                        community_chest_b = true;
                    }
                }
                else if (translate_red_ball_z_up > -1.3f + 8 * .23f && translate_red_ball_z_up <= -1.3f + 9 * .23f)
                {
                    tenesee.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!ten_b) {
                        if (player1_money > ten_value) {
                            player1_money = player1_money - ten_value;
                            player1_property_price = player1_property_price + ten_value;
                            ten_b = true;
                        }
                    }
                }
                else if (translate_red_ball_z_up > -1.3f + 9 * .23f && translate_red_ball_z_up <= -1.3f + 10 * .23f)
                {
                    newyork.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!newyork_b) {
                        if (player1_money > newyork_value) {
                            player1_money = player1_money - newyork_value;
                            player1_property_price = player1_property_price + newyork_value;
                            newyork_b = true;
                        }
                    }
                }
                else if (translate_red_ball_z_up > -1.3f + 10 * .23f && translate_red_ball_z_up <= -1.3f + 11 * .23f)
                {
                    
                }
                if (translate_red_ball_x_right < 1.2f && translate_red_ball_x_right >= 1.2f - 2 * .23f) {
                    kentucky.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!kentucky_b) {
                        if (player1_money > kentucky_value) {
                            player1_money = player1_money - kentucky_value;
                            player1_property_price = player1_property_price + kentucky_value;
                            kentucky_b = true;
                        }
                    }

                }
                else if (translate_red_ball_x_right < 1.2f - 2 * .23f && translate_red_ball_x_right >= 1.2f - 3 * .23f) {
                    if (!chance_b) {
                        player1_money = player1_money - chance;
                        chance_b = true;
                    }
                }
                else if (translate_red_ball_x_right < 1.2f - 3 * .23f && translate_red_ball_x_right >= 1.2f - 4 * .23f) {
                    indiana.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!indiana_b) {
                        if (player1_money > indiana_value) {
                            player1_money = player1_money - indiana_value;
                            player1_property_price = player1_property_price + indiana_value;
                            indiana_b = true;
                        }
                    }
                }
                else if (translate_red_ball_x_right < 1.2f - 4 * .23f && translate_red_ball_x_right >= 1.2f - 5 * .23f) {
                    illinois.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!illinois_b) {
                        if (player1_money > illinois_value) {
                            player1_money = player1_money - illinois_value;
                            player1_property_price = player1_property_price + illinois_value;
                            illinois_b = true;
                        }
                    }
                }
                else if (translate_red_ball_x_right < 1.2f - 5 * .23f && translate_red_ball_x_right >= 1.2f - 6 * .23f) {
                    B_rail.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!bo_rr_b) {
                        if (player1_money > bo_rr_value) {
                            player1_money = player1_money - bo_rr_value;
                            player1_property_price = player1_property_price + bo_rr_value;
                            bo_rr_b = true;
                        }
                    }
                }
                else if (translate_red_ball_x_right < 1.2f - 6 * .23f && translate_red_ball_x_right >= 1.2f - 7 * .23f) {
                    atlanta.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!atlanta_b) {
                        if (player1_money > atlanta_value) {
                            player1_money = player1_money - atlanta_value;
                            player1_property_price = player1_property_price + atlanta_value;
                            atlanta_b = true;
                        }
                    }
                }
                else if (translate_red_ball_x_right < 1.2f - 7 * .23f && translate_red_ball_x_right >= 1.2f - 8 * .23f) {
                    ventnor.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!ventnor_b) {
                        if (player1_money > ventnor_value) {
                            player1_money = player1_money - ventnor_value;
                            player1_property_price = player1_property_price + ventnor_value;
                            ventnor_b = true;
                        }
                    }
                }
                else if (translate_red_ball_x_right < 1.2f - 8 * .23f && translate_red_ball_x_right >= 1.2f - 9 * .23f) {
                    WaterWork.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!waterwork_b) {
                        if (player1_money > waterwork_value) {
                            player1_money = player1_money - waterwork_value;
                            player1_property_price = player1_property_price + waterwork_value;
                            waterwork_b = true;
                        }
                    }
                }
                else if (translate_red_ball_x_right < 1.2f - 9 * .23f && translate_red_ball_x_right >= 1.2f - 10 * .23f) {
                    marvin.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!marvin_b) {
                        if (player1_money > marvin_value) {
                            player1_money = player1_money - marvin_value;
                            player1_property_price = player1_property_price + marvin_value;
                            marvin_b = true;
                        }
                    }
                }
                if (translate_red_ball_z_down < 1.0f && translate_red_ball_z_down >= 1.0f - .24f)
                {
                    pacific.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!pacific_b) {
                        if (player1_money > pacific_value) {
                            player1_money = player1_money - pacific_value;
                            player1_property_price = player1_property_price + pacific_value;
                            pacific_b = true;
                        }
                    }
                }
                else if (translate_red_ball_z_down < 1.0F - .23f && translate_red_ball_z_down >= 1.0f - 2 * .24F)
                {
                    carolina.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!carolina_b) {
                        if (player1_money > carolina_value) {
                            player1_money = player1_money - carolina_value;
                            player1_property_price = player1_property_price + carolina_value;
                            carolina_b = true;
                        }
                    }
                }
                else if (translate_red_ball_z_down < 1.0f - .24F && translate_red_ball_z_down >= 1.0f - 3 * .24F)
                {
                    if (!community_chest_b) {
                        player1_money = player1_money - community_chest;
                        community_chest_b = true;
                    }
                }
                else if (translate_red_ball_z_down < 1.0f - 3 * .24F && translate_red_ball_z_down >= 1.0f - 4 * .24F)
                {
                    penselv.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!penn_b) {
                        if (player1_money > penn_value) {
                            player1_money = player1_money - penn_value;
                            player1_property_price = player1_property_price + penn_value;
                            penn_b = true;
                        }
                    }
                }
                else if (translate_red_ball_z_down < 1.0f - 4 * .24F && translate_red_ball_z_down >= 1.0f - 5 * .24F)
                {
                    Short.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!short_rr_b) {
                        if (player1_money > shortline_rr_value) {
                            player1_money = player1_money - shortline_rr_value;
                            player1_property_price = player1_property_price + shortline_rr_value;
                            short_rr_b = true;
                        }
                    }
                }
                else if (translate_red_ball_z_down < 1.0f - 5 * .24F && translate_red_ball_z_down >= 1.0f - 6 * .24F)
                {
                    if (!chance_b) {
                        player1_money = player1_money - chance;
                        chance_b = true;
                    }
                }
                else if (translate_red_ball_z_down < 1.0f - 6 * .24F && translate_red_ball_z_down >= 1.0f - 7 * .24F)
                {
                    park.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!park_b) {
                        if (player1_money > park_value) {
                            player1_money = player1_money - park_value;
                            player1_property_price = player1_property_price + park_value;
                            park_b = true;
                        }
                    }

                }
                else if (translate_red_ball_z_down < 1.0f - 7 * .24F && translate_red_ball_z_down >= 1.0f - 8 * .24F)
                {   
                    if (!income_tax_b) {
                        player1_money = player1_money - income_tax;
                        income_tax_b = true;
                    }
                }
                else if (translate_red_ball_z_down < 1.0f - 8 * .24F && translate_red_ball_z_down >= 1.0f - 9 * .24F)
                {
                    boardwalk.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!board_b) {
                        if (player1_money > board_value) {
                            player1_money = player1_money - board_value;
                            player1_property_price = player1_property_price + board_value;
                            board_b = true;
                        }
                    }
                }

            }
        }
        if (player2_turn) {
            if (turnEnd && cardshown) {
                if (translate_blue_cube_x_left > -1.2f && translate_blue_cube_x_left <= -1.2f + 2 * .22f)
                {
                    medeterian.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!med_b) {
                        if (player2_money > med_value) {
                            player2_money = player2_money - med_value;
                            player2_property_price = player2_property_price + med_value;
                            med_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_x_left > -1.2f + 2 * .22f && translate_blue_cube_x_left <= -1.2f + 3 * .22f)
                {
                    if (!community_chest_b) {
                        player2_money = player2_money - community_chest;
                        community_chest_b = true;
                    }
                }
                else if (translate_blue_cube_x_left > -1.2f + 3 * .22f && translate_blue_cube_x_left <= -1.2f + 4 * .22f)
                {
                    baltic.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!baltic_b) {
                        if (player2_money > baltic_value) {
                            player2_money = player2_money - baltic_value;
                            player2_property_price = player2_property_price + baltic_value;
                            baltic_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_x_left > -1.2f + 4 * .22f && translate_blue_cube_x_left <= -1.2f + 5 * .22f)
                {
                    if (!income_tax_b) {
                        player2_money = player2_money - income_tax;
                        income_tax_b = true;
                    }
                }

                else if (translate_blue_cube_x_left > -1.2f + 5 * .22f && translate_blue_cube_x_left <= -1.2f + 6 * .22f)
                {
                    read.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!reading_rr_b) {
                        if (player2_money > reading_rr_value) {
                            player2_money = player2_money - reading_rr_value;
                            player2_property_price = player2_property_price + reading_rr_value;
                            reading_rr_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_x_left > -1.2f + 6 * .22f && translate_blue_cube_x_left <= -1.2f + 7 * .22f)
                {
                    oriental.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!oriental_b) {
                        if (player2_money > oriental_value) {
                            player2_money = player2_money - oriental_value;
                            player2_property_price = player2_property_price + oriental_value;
                            oriental_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_x_left > -1.2f + 7 * .22f && translate_blue_cube_x_left <= -1.2f + 8 * .22f)
                {
                    if (!chance_b) {
                        player2_money = player2_money - chance;
                        chance_b = true;
                    }
                }
                else if (translate_blue_cube_x_left > -1.2f + 8 * .22f && translate_blue_cube_x_left <= -1.2f + 9 * .22f)
                {
                    vermont.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!vermont_b) {
                        if (player2_money > vermont_value) {
                            player2_money = player2_money - vermont_value;
                            player2_property_price = player2_property_price + vermont_value;
                            vermont_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_x_left > -1.2f + 9 * .22f && translate_blue_cube_x_left <= -1.2f + 10 * .22f)
                {
                    connect.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!connect_b) {
                        if (player2_money > connect_value) {
                            player2_money = player2_money - connect_value;
                            player2_property_price = player2_property_price + connect_value;
                            connect_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_x_left > -1.2f + 10 * .22f && translate_blue_cube_x_left <= -1.2f + 11 * .22f)
                {

                }
                if (translate_blue_cube_z_up > -1.3f && translate_blue_cube_z_up <= -1.3f + 2 * .23f)
                {
                    charles.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!charles_b) {
                        if (player2_money > charles_value) {
                            player2_money = player2_money - charles_value;
                            player2_property_price = player2_property_price + charles_value;
                            charles_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_z_up > -1.3f + 2 * .23f && translate_blue_cube_z_up <= -1.3f + 3 * .23f)
                {
                    electric.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!electric_b) {
                        if (player2_money > electric_value) {
                            player2_money = player2_money - electric_value;
                            player2_property_price = player2_property_price + electric_value;
                            electric_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_z_up > -1.3f + 3 * .23f && translate_blue_cube_z_up <= -1.3f + 4 * .23f)
                {
                    state.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!state_b) {
                        if (player2_money > state_value) {
                            player2_money = player2_money - state_value;
                            player2_property_price = player2_property_price + state_value;
                            state_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_z_up > -1.3f + 4 * .23f && translate_blue_cube_z_up <= -1.3f + 5 * .23f)
                {
                    virginia.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!virginia_b) {
                        if (player2_money > virginia_value) {
                            player2_money = player2_money - virginia_value;
                            player2_property_price = player2_property_price + virginia_value;
                            virginia_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_z_up > -1.3f + 5 * .23f && translate_blue_cube_z_up <= -1.3f + 6 * .23f)
                {
                    penRail.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!penn_rr_b) {
                        if (player2_money > penn_rr_value) {
                            player2_money = player2_money - penn_rr_value;
                            player2_property_price = player2_property_price + penn_rr_value;
                            penn_rr_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_z_up > -1.3f + 6 * .23f && translate_blue_cube_z_up <= -1.3f + 7 * .23f)
                {
                    james.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!james_b) {
                        if (player2_money > james_value) {
                            player2_money = player2_money - james_value;
                            player2_property_price = player2_property_price + james_value;
                            james_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_z_up > -1.3f + 7 * .23f && translate_blue_cube_z_up <= -1.3f + 8 * .23f)
                {
                    if (!community_chest_b) {
                        player2_money = player2_money - community_chest;
                        community_chest_b = true;
                    }
                }
                else if (translate_blue_cube_z_up > -1.3f + 8 * .23f && translate_blue_cube_z_up <= -1.3f + 9 * .23f)
                {
                    tenesee.drawCubeWithTexture(lightingShaderWithTexture, model_front); 
                    if (!ten_b) {
                        if (player2_money > ten_value) {
                            player2_money = player2_money - ten_value;
                            player2_property_price = player2_property_price + ten_value;
                            ten_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_z_up > -1.3f + 9 * .23f && translate_blue_cube_z_up <= -1.3f + 10 * .23f)
                {
                    newyork.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!newyork_b) {
                        if (player2_money > newyork_value) {
                            player2_money = player2_money - newyork_value;
                            player2_property_price = player2_property_price + newyork_value;
                            newyork_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_z_up > -1.3f + 10 * .23f && translate_blue_cube_z_up <= -1.3f + 11 * .23f)
                {

                }
                if (translate_blue_cube_x_right < 1.2f && translate_blue_cube_x_right >= 1.2f - 2 * .23f) {
                    kentucky.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!kentucky_b) {
                        if (player2_money > kentucky_value) {
                            player2_money = player2_money - kentucky_value;
                            player2_property_price = player2_property_price + kentucky_value;
                            kentucky_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_x_right < 1.2f - 2 * .23f && translate_blue_cube_x_right >= 1.2f - 3 * .23f) {
                    if (!chance_b) {
                        player2_money = player2_money - chance;
                        chance_b = true;
                    }
                }
                else if (translate_blue_cube_x_right < 1.2f - 3 * .23f && translate_blue_cube_x_right >= 1.2f - 4 * .23f) {
                    indiana.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!indiana_b) {
                        if (player2_money > indiana_value) {
                            player2_money = player2_money - indiana_value;
                            player2_property_price = player2_property_price + indiana_value;
                            indiana_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_x_right < 1.2f - 4 * .23f && translate_blue_cube_x_right >= 1.2f - 5 * .23f) {
                    illinois.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!illinois_b) {
                        if (player2_money > illinois_value) {
                            player2_money = player2_money - illinois_value;
                            player2_property_price = player2_property_price + illinois_value;
                            illinois_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_x_right < 1.2f - 5 * .23f && translate_blue_cube_x_right >= 1.2f - 6 * .23f) {
                    B_rail.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!bo_rr_b) {
                        if (player2_money > bo_rr_value) {
                            player2_money = player2_money - bo_rr_value;
                            player2_property_price = player2_property_price + bo_rr_value;
                            bo_rr_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_x_right < 1.2f - 6 * .23f && translate_blue_cube_x_right >= 1.2f - 7 * .23f) {
                    atlanta.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!atlanta_b) {
                        if (player2_money > atlanta_value) {
                            player2_money = player2_money - atlanta_value;
                            player2_property_price = player2_property_price + atlanta_value;
                            atlanta_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_x_right < 1.2f - 7 * .23f && translate_blue_cube_x_right >= 1.2f - 8 * .23f) {
                    ventnor.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!ventnor_b) {
                        if (player2_money > ventnor_value) {
                            player2_money = player2_money - ventnor_value;
                            player2_property_price = player2_property_price + ventnor_value;
                            ventnor_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_x_right < 1.2f - 8 * .23f && translate_blue_cube_x_right >= 1.2f - 9 * .23f) {
                    WaterWork.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!waterwork_b) {
                        if (player2_money > waterwork_value) {
                            player2_money = player2_money - waterwork_value;
                            player2_property_price = player2_property_price + waterwork_value;
                            waterwork_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_x_right < 1.2f - 9 * .23f && translate_blue_cube_x_right >= 1.2f - 10 * .23f) {
                    marvin.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!marvin_b) {
                        if (player2_money > marvin_value) {
                            player2_money = player2_money - marvin_value;
                            player2_property_price = player2_property_price + marvin_value;
                            marvin_b = true;
                        }
                    }
                }
                if (translate_blue_cube_z_down < 1.0f && translate_blue_cube_z_down >= 1.0f - .24f)
                {
                    pacific.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!pacific_b) {
                        if (player2_money > pacific_value) {
                            player2_money = player2_money - pacific_value;
                            player2_property_price = player2_property_price + pacific_value;
                            pacific_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_z_down < 1.0F - .23f && translate_blue_cube_z_down >= 1.0f - 2 * .24F)
                {
                    carolina.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!carolina_b) {
                        if (player2_money > carolina_value) {
                            player2_money = player2_money - carolina_value;
                            player2_property_price = player2_property_price + carolina_value;
                            carolina_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_z_down < 1.0f - .24F && translate_blue_cube_z_down >= 1.0f - 3 * .24F)
                {
                    if (!community_chest_b) {
                        player2_money = player2_money - community_chest;
                        community_chest_b = true;
                    }
                }
                else if (translate_blue_cube_z_down < 1.0f - 3 * .24F && translate_blue_cube_z_down >= 1.0f - 4 * .24F)
                {
                    penselv.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!penn_b) {
                        if (player2_money > penn_value) {
                            player2_money = player2_money - penn_value;
                            player2_property_price = player2_property_price + penn_value;
                            penn_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_z_down < 1.0f - 4 * .24F && translate_blue_cube_z_down >= 1.0f - 5 * .24F)
                {
                    Short.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!short_rr_b) {
                        if (player2_money > shortline_rr_value) {
                            player2_money = player2_money - shortline_rr_value;
                            player2_property_price = player2_property_price + shortline_rr_value;
                            short_rr_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_z_down < 1.0f - 5 * .24F && translate_blue_cube_z_down >= 1.0f - 6 * .24F)
                {
                    if (!chance_b) {
                        player2_money = player2_money - chance;
                        chance_b = true;
                    }
                }
                else if (translate_blue_cube_z_down < 1.0f - 6 * .24F && translate_blue_cube_z_down >= 1.0f - 7 * .24F)
                {
                    park.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!park_b) {
                        if (player2_money > park_value) {
                            player2_money = player2_money - park_value;
                            player2_property_price = player2_property_price + park_value;
                            park_b = true;
                        }
                    }
                }
                else if (translate_blue_cube_z_down < 1.0f - 7 * .24F && translate_blue_cube_z_down >= 1.0f - 8 * .24F)
                {
                    if (!income_tax_b) {
                        player2_money = player2_money - income_tax;
                        income_tax_b = true;
                    }
                }
                else if (translate_blue_cube_z_down < 1.0f - 8 * .24F && translate_blue_cube_z_down >= 1.0f - 9 * .24F)
                {
                    boardwalk.drawCubeWithTexture(lightingShaderWithTexture, model_front);
                    if (!board_b) {
                        if (player2_money > board_value) {
                            player2_money = player2_money - board_value;
                            player2_property_price = player2_property_price + board_value;
                            board_b = true;
                        }
                    }
                }

            }
        }
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;}
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 22.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void floor(Cube cube, Shader& lightingShaderWithTexture, glm::mat4 alTogether)
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);


    scale = glm::scale(model, glm::vec3(15.0f, 0.1f, 15.0f));
    translate = glm::translate(model, glm::vec3(-4.0f, -1.5f, -5.0f));
    model = alTogether * translate * scale;
    cube.drawCubeWithTexture(lightingShaderWithTexture, model);
}
void table(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
{

    // Top
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model;

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-.6f, 0.2f, -2.2f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.5f, 0.1f, 6.5f));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.502f, 0.298f, 0.149f);

    // Leg 1
    glm::mat4 identityMatrix1 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix1, scaleMatrix1, model1;
    translateMatrix1 = glm::translate(identityMatrix1, glm::vec3(-0.6f, .2f, -2.2f));
    scaleMatrix1 = glm::scale(identityMatrix1, glm::vec3(0.1f, -1.5f, 0.1f));
    model1 = alTogether * translateMatrix1 * scaleMatrix1;
    drawCube(cubeVAO, lightingShader, model1, .44f, .47f, .49f);

    // Leg 2
    glm::mat4 identityMatrix2 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix2, scaleMatrix2, model2;
    translateMatrix2 = glm::translate(identityMatrix2, glm::vec3(-0.6f, 0.2f, 4.2f));
    scaleMatrix2 = glm::scale(identityMatrix2, glm::vec3(0.1f, -1.5f, 0.1f));
    model2 = alTogether * translateMatrix2 * scaleMatrix2;
    drawCube(cubeVAO, lightingShader, model2, .44f, .47f, .49f);

    // Leg 3
    translateMatrix1 = glm::translate(identityMatrix1, glm::vec3(3.8f, 0.2f, -2.2f));
    scaleMatrix1 = glm::scale(identityMatrix1, glm::vec3(0.1f, -1.5f, 0.1f));
    model1 = alTogether * translateMatrix1 * scaleMatrix1;
    drawCube(cubeVAO, lightingShader, model1, .44f, .47f, .49f);

    // Leg 4
    translateMatrix2 = glm::translate(identityMatrix2, glm::vec3(3.8f, 0.2f, 4.2f));
    scaleMatrix2 = glm::scale(identityMatrix2, glm::vec3(0.1f, -1.5f, 0.1f));
    model2 = alTogether * translateMatrix2 * scaleMatrix2;
    drawCube(cubeVAO, lightingShader, model2, .44f, .47f, .49f);
}
void board(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
{
    

    ////base
    glm::mat4 model = glm::mat4(1.0f);
    

    glm::mat4 identityMatrix = glm::mat4(1.0f); 
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix;

    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f, 0.0f, -1.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 3.0f));
    model = alTogether * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.6f, 0.0f, -1.6f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 3.2f));
    model = alTogether * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.6f, 0.0f, -1.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-2.0f, 0.1f, 3.2f));
    model = alTogether * translateMatrix * rotateXMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.6f, 0.0f, 1.5f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 3.1f));
    model = alTogether * translateMatrix * rotateXMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0);


  

    

}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime * 2);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime * 2);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime * 2);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime * 2);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_RIGHT, deltaTime);
    }
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
//void processInput(GLFWwindow* window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
//        camera.ProcessKeyboard(FORWARD, deltaTime);
//    }
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
//        camera.ProcessKeyboard(BACKWARD, deltaTime);
//    }
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
//        camera.ProcessKeyboard(LEFT, deltaTime);
//    }
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
//        camera.ProcessKeyboard(RIGHT, deltaTime);
//    }
//
//    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
//    {
//        if (rotateAxis_X) rotateAngle_X -= 0.1;
//        else if (rotateAxis_Y) rotateAngle_Y -= 0.1;
//        else rotateAngle_Z -= 0.1;
//    }
//    /*if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.001;
//    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.001;
//    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.001;
//    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.001;
//    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.001;
//    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.001;
//    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.001;
//    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.001;
//    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.001;
//    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.001;
//    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.001;
//    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.001;*/
//
//    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
//    {
//        rotateAngle_X += 0.3;
//        rotateAxis_X = 1.0;
//        rotateAxis_Y = 0.0;
//        rotateAxis_Z = 0.0;
//    }
//    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
//    {
//        rotateAngle_Y += 0.3;
//        rotateAxis_X = 0.0;
//        rotateAxis_Y = 1.0;
//        rotateAxis_Z = 0.0;
//    }
//    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
//    {
//        rotateAngle_Z += 0.3;
//        rotateAxis_X = 0.0;
//        rotateAxis_Y = 0.0;
//        rotateAxis_Z = 1.0;
//    }
//
//    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
//    {
//        eyeX += 2.5 * deltaTime;
//        basic_camera.changeEye(eyeX, eyeY, eyeZ);
//    }
//    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
//    {
//        eyeX -= 2.5 * deltaTime;
//        basic_camera.changeEye(eyeX, eyeY, eyeZ);
//    }
//    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
//    {
//        eyeZ += 2.5 * deltaTime;
//        basic_camera.changeEye(eyeX, eyeY, eyeZ);
//    }
//    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
//    {
//        eyeZ -= 2.5 * deltaTime;
//        basic_camera.changeEye(eyeX, eyeY, eyeZ);
//    }
//    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
//    {
//        eyeY += 2.5 * deltaTime;
//        basic_camera.changeEye(eyeX, eyeY, eyeZ);
//    }
//    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
//    {
//        eyeY -= 2.5 * deltaTime;
//        basic_camera.changeEye(eyeX, eyeY, eyeZ);
//    }
//
//}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (directionLighton)
        {
            directlight.turnOff();
            directionLighton = !directionLighton;
        }
        else
        {
            directlight.turnOn();
            directionLighton = !directionLighton;
        }
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointLightOn = !pointLightOn;
        }
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (spotlighton)
        {
            spotlight.turnOff();
            spotlighton = !spotlighton;
        }
        else
        {
            spotlight.turnOn();
            spotlighton = !spotlighton;
        }
    }
    else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        if (ambientToggle)
        {
            pointlight1.turnAmbientOff();
            pointlight2.turnAmbientOff();
            pointlight3.turnAmbientOff();
            pointlight4.turnAmbientOff();
            directlight.turnAmbientOff();
            spotlight.turnAmbientOff();
            ambientToggle = !ambientToggle;
        }
        else
        {
            pointlight1.turnAmbientOn();
            pointlight2.turnAmbientOn();
            pointlight3.turnAmbientOn();
            pointlight4.turnAmbientOn();
            directlight.turnAmbientOn();
            spotlight.turnAmbientOn();
            ambientToggle = !ambientToggle;
        }
    }
    else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        if (diffuseToggle)
        {
            pointlight1.turnDiffuseOff();
            pointlight2.turnDiffuseOff();
            pointlight3.turnDiffuseOff();
            pointlight4.turnDiffuseOff();
            directlight.turnDiffuseOff();
            spotlight.turnDiffuseOff();
            diffuseToggle = !diffuseToggle;
        }
        else
        {
            pointlight1.turnDiffuseOn();
            pointlight2.turnDiffuseOn();
            pointlight3.turnDiffuseOn();
            pointlight4.turnDiffuseOn();
            directlight.turnDiffuseOn();
            spotlight.turnDiffuseOn();
            diffuseToggle = !diffuseToggle;
        }
    }
    else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        if (specularToggle)
        {
            pointlight1.turnSpecularOff();
            pointlight2.turnSpecularOff();
            pointlight3.turnSpecularOff();
            pointlight4.turnSpecularOff();
            directlight.turnSpecularOff();
            spotlight.turnSpecularOff();
            specularToggle = !specularToggle;
        }
        else
        {
            pointlight1.turnSpecularOn();
            pointlight2.turnSpecularOn();
            pointlight3.turnSpecularOn();
            pointlight4.turnSpecularOn();
            directlight.turnSpecularOn();
            spotlight.turnSpecularOn();
            specularToggle = !specularToggle;
        }
    }
    else if (key == GLFW_KEY_7 && action == GLFW_PRESS)
    {   
        
        dice_roll(dice_number,turnEnd,cardshown,dice_roll_number,player1_turn,player2_turn,community_chest_b,chance_b,income_tax_b,luxury_tax_b,gameend,dice_flag);
        
        cout << "Player 1 money : " << player1_money << endl;
        cout << "Player 2 money : " << player2_money << endl;
        cout << "Player 1 property price : " << player1_property_price << endl;
        cout << "Player 2 property price : " << player2_property_price << endl;
        if (player1_money <= 100 || player2_money <= 100) {
            if (player1_money < 0) {
                player1_property_price = player1_property_price + player1_money;
            }
            if (player2_money < 0) {
                player2_property_price = player2_property_price + player2_money;
            }
            if (player1_property_price > player2_property_price) {
                cout << "Player 1 has won" << endl;
                gameend = true;
            }
            else {
                cout << "Player 2 has won" << endl;
                gameend = true;
            }
        }
    }
    else if (key == GLFW_KEY_B && action == GLFW_PRESS)
    {

        
        showcard(cardshown);

    }

}
void showcard(bool &cardshown)
{
    cardshown = false;
}
void dice_roll(float &dice_number, bool &turnEnd, bool &cardshown, int &dice_roll_number, bool &player1_turn, bool &player2_turn, bool &community_chest_b, bool &chance_b, bool &income_tax_b, bool &luxury_tax_b,bool &gameend,bool &dice_flag)
{   
    if (turnEnd)
    {
        srand(time(NULL));
        int x = rand();
        x = 1 + (x % 6);
        
        dice_roll_number = x;
        dice_number = x * 0.24f;
        turnEnd = false;
        cardshown = true;
        community_chest_b = false;
        chance_b = false;
        income_tax_b = false;
        luxury_tax_b = false;
        player1_turn = !player1_turn;
        player2_turn = !player2_turn;
        dice_flag = false;

        
    }
    
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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
long long nCr(int n, int r)
{
    if (r > n / 2)
        r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for (i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

//polynomial interpretation for N points
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L)
{
    double y = 0;
    double x = 0;
    t = t > 1.0 ? 1.0 : t;
    for (int i = 0; i < L + 1; i++)
    {
        long long ncr = nCr(L, i);
        double oneMinusTpow = pow(1 - t, double(L - i));
        double tPow = pow(t, double(i));
        double coef = oneMinusTpow * tPow * ncr;
        x += coef * ctrlpoints[i * 3];
        y += coef * ctrlpoints[(i * 3) + 1];

    }
    xy[0] = float(x);
    xy[1] = float(y);
}

unsigned int hollowBezier(GLfloat ctrlpoints[], int L, vector <float>& coordinates,
    vector <float>& normals,
    vector <int>& indices,
    vector <float>& vertices)
{
    int i, j;
    float x, y, z, r;                //current coordinates
    float theta;
    float nx, ny, nz, lengthInv;    // vertex normal


    const float dtheta = 2 * pi / ntheta;        //angular step size

    float t = 0;
    float dt = 1.0 / nt;
    float xy[2];

    for (i = 0; i <= nt; ++i)              //step through y
    {
        BezierCurve(t, xy, ctrlpoints, L);
        r = xy[0];
        y = xy[1];
        theta = 0;
        t += dt;
        lengthInv = 1.0 / r;

        for (j = 0; j <= ntheta; ++j)
        {
            double cosa = cos(theta);
            double sina = sin(theta);
            z = r * cosa;
            x = r * sina;

            coordinates.push_back(x);
            coordinates.push_back(y);
            coordinates.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            // center point of the circle (0,y,0)
            nx = (x - 0) * lengthInv;
            ny = (y - y) * lengthInv;
            nz = (z - 0) * lengthInv;

            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            theta += dtheta;
        }
    }

    // generate index list of triangles
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1

    int k1, k2;
    for (int i = 0; i < nt; ++i)
    {
        k1 = i * (ntheta + 1);     // beginning of current stack
        k2 = k1 + ntheta + 1;      // beginning of next stack

        for (int j = 0; j < ntheta; ++j, ++k1, ++k2)
        {
            // k1 => k2 => k1+1
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 1);

            // k1+1 => k2 => k2+1
            indices.push_back(k1 + 1);
            indices.push_back(k2);
            indices.push_back(k2 + 1);
        }
    }

    size_t count = coordinates.size();
    for (int i = 0; i < count; i += 3)
    {
        vertices.push_back(coordinates[i]);
        vertices.push_back(coordinates[i + 1]);
        vertices.push_back(coordinates[i + 2]);

        vertices.push_back(normals[i]);
        vertices.push_back(normals[i + 1]);
        vertices.push_back(normals[i + 2]);
    }

    unsigned int bezierVAO;
    glGenVertexArrays(1, &bezierVAO);
    glBindVertexArray(bezierVAO);

    // create VBO to copy vertex data to VBO
    unsigned int bezierVBO;
    glGenBuffers(1, &bezierVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);           // for vertex data
    glBufferData(GL_ARRAY_BUFFER,                   // target
        (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
        vertices.data(),   // ptr to vertex data
        GL_STATIC_DRAW);                   // usage

    // create EBO to copy index data
    unsigned int bezierEBO;
    glGenBuffers(1, &bezierEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierEBO);   // for index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
        (unsigned int)indices.size() * sizeof(unsigned int),             // data size, # of bytes
        indices.data(),               // ptr to index data
        GL_STATIC_DRAW);                   // usage

    // activate attrib arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // set attrib arrays with stride and offset
    int stride = 24;     // should be 24 bytes
    glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));

    // unbind VAO, VBO and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return bezierVAO;
}
void drawCurveObj(unsigned int& bezierVAO_, Shader& lightingShader, glm::mat4 model, float r = 1.0f, float g = 1.0f, float b = 1.0f, float shine = 32.0f) {
    lightingShader.use();
    lightingShader.setMat4("model", model);
    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    lightingShader.setFloat("material.shininess", shine);

    glBindVertexArray(bezierVAO_);
    glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, (void*)0);

    // unbind VAO
    glBindVertexArray(0);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
