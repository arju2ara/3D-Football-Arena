#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>
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
#include "cylinder.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
using namespace std;

// ─── Forward declarations ─────────────────────────────────────────────────────
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

void drawCube(unsigned int& cubeVAO, Shader& lightingShader,
    glm::mat4 model, float r, float g, float b,
    glm::vec3 emissive = glm::vec3(0.0f));

void drawCubeWithTexture(unsigned int& cubeVAO,
    Shader& lightingShaderWithTexture,
    glm::mat4 model, unsigned int texture);

void drawCubeBlended(unsigned int& cubeVAO,
    Shader& lightingShaderWithTexture,
    glm::mat4 model, unsigned int texture,
    float r, float g, float b);

void drawCone(unsigned int coneVAO, int vertCount,
    Shader& lightingShader, glm::mat4 model,
    float r, float g, float b);
void drawConeWithTexture(unsigned int coneVAO, int vertCount,
    Shader& lightingShaderWithTexture,
    glm::mat4 model, unsigned int texture);
void drawConeBlended(unsigned int coneVAO, int vertCount,
    Shader& lightingShaderWithTexture,
    glm::mat4 model, unsigned int texture,
    float r, float g, float b);

void drawFloodlight(unsigned int& cubeVAO, Shader& lightingShader,
    glm::vec3 position, bool emissive);
void drawFractalTree(unsigned int& cubeVAO, Shader& lightingShader,
    glm::mat4 parentModel, int depth, float branchLen);
void drawGoalPost(unsigned int& cubeVAO, Shader& lightingShader,
    glm::mat4 model, glm::vec3 position);

void   BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L);
void BezierCurve3D(double t, float xyz[3], GLfloat ctrl[], int L);
long long nCr(int n, int r);
void   buildFootballNet(float goalWidth, float goalHeight, float netDepth,
    int cols, int rows);
void   drawFootballNet(Shader& lightingShader, glm::mat4 model,
    float r, float g, float b);
void buildStadiumRoof(float rx, float rz, float centerX, float centerZ, int sweepSegs, int splineSegs);
void drawStadiumRoof(Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void drawSkyGradient(Shader& skyShader, unsigned int skyVAO, unsigned int skyTexture);
void drawCloud(unsigned int& cubeVAO, Shader& lightingShader, Sphere& sphere,
    glm::vec3 center, float scale);
void drawEnvironment(unsigned int& cubeVAO, Shader& lightingShader,
    Shader& lightingShaderWithTexture, Sphere& sphere,
    unsigned int grassTexture, int textureMode, float cloudDrift);

void stadium(unsigned int& cubeVAO, unsigned int coneVAO, int coneVertCount,
    Shader& lightingShader, Shader& lightingShaderWithTexture,
    glm::mat4 alTogether,
    unsigned int fieldTexture, unsigned int grassTexture,
    unsigned int footballTexture, unsigned int scoreTexture,
    int textureMode);

void drawPerson(unsigned int& cubeVAO, Shader& lightingShader,
    Shader& lightingShaderWithTexture,
    Sphere& sphere,
    float posX, float posY, float posZ,
    float facingAngle, float legSwingAngle,
    float armRaiseAngle, float jumpY,
    unsigned int headTex, unsigned int shirtTex,
    unsigned int shortsTex, unsigned int legsTex,
    unsigned int bootsTex, unsigned int hairTex);
void buildFootballNet(float goalWidth, float goalHeight, float netDepth,
    int cols, int rows, float waveTime);

void renderScene(unsigned int& cubeVAO, unsigned int coneVAO, int coneVertCount,
    Shader& lightingShader, Shader& lightingShaderWithTexture,
    Sphere& sphere, glm::mat4 projection, glm::mat4 view,
    bool emissiveOn, int textureMode,
    unsigned int fieldTexture, unsigned int grassTexture,
    unsigned int footballTexture, unsigned int scoreTexture,
    float bar_x);


glm::mat4 myLookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
glm::mat4 myOrthographic(float left, float right, float bottom, float top, float near, float far);
glm::mat4 myProjection(float fov, float aspect, float near, float far);
unsigned int loadTexture(char const* path, GLenum wS, GLenum wT, GLenum miF, GLenum maF);

// ─── Window ───────────────────────────────────────────────────────────────────
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
int framebufferWidth = SCR_WIDTH;
int framebufferHeight = SCR_HEIGHT;

// ─── Camera ───────────────────────────────────────────────────────────────────
Camera camera(glm::vec3(0.0f, 3.0f, -8.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// ─── Lights ───────────────────────────────────────────────────────────────────
glm::vec3 pointLightPositions[] = {
    glm::vec3(-8.0f, 10.0f, -6.5f),
    glm::vec3(-8.0f, 10.0f, 23.5f),
    glm::vec3(8.0f, 10.0f, -6.5f),
    glm::vec3(8.0f, 10.0f, 23.5f)
};
PointLight pointlight1(pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,
    0.02f, 0.02f, 0.02f, 1.5f, 1.5f, 1.5f, 2.0f, 2.0f, 2.0f, 1.0f, 0.045f, 0.0075f, 1);
PointLight pointlight2(pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,
    0.02f, 0.02f, 0.02f, 1.5f, 1.5f, 1.5f, 2.0f, 2.0f, 2.0f, 1.0f, 0.045f, 0.0075f, 2);
PointLight pointlight3(pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,
    0.02f, 0.02f, 0.02f, 1.5f, 1.5f, 1.5f, 2.0f, 2.0f, 2.0f, 1.0f, 0.045f, 0.0075f, 3);
PointLight pointlight4(pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,
    0.02f, 0.02f, 0.02f, 1.5f, 1.5f, 1.5f, 2.0f, 2.0f, 2.0f, 1.0f, 0.045f, 0.0075f, 4);
DirectionLight directlight(0.0f, 20.0f, 0.0f,
    0.05f, 0.05f, 0.05f, 0.2f, 0.2f, 0.2f, 0.1f, 0.1f, 0.1f, 1.0f, 0.09f, 0.032f);

SpotLight spotlight(0.0f, 5.0f, 8.5f, 0.0f, -1.0f, 0.0f,
    0.1f, 0.1f, 0.1f, 0.8f, 0.8f, 0.8f, 0.5f, 0.5f, 0.5f, 30.0f, 1.0f, 0.09f, 0.032f);

// ─── Toggles ──────────────────────────────────────────────────────────────────
bool pointLightOn = true;
bool directionLighton = true;
bool spotlighton = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool emissiveOn = false;
int  textureMode = 0;
int  activeViewport = 0;
enum KickState { IDLE, RUN_TO_BALL, BACKSWING, KICK_FORWARD, FOLLOW_THROUGH, DONE };
KickState kickState = IDLE;
float kickTimer = 0.0f;
float runCycle = 0.0f;
float runLegAngle = 0.0f;
float runArmAngle = 0.0f;
float personRunZ = 0.0f;
float legAngle = 0.0f;
float bodyLean = 0.0f;        // forward lean during kick (degrees around X)
float leftLegAngle = 0.0f;    // standing leg counter-swing


float armAngle = 0.0f;
bool  celebrating = false;
float celebTimer = 0.0f;
float personJumpY = 0.0f;
float cloudDriftTime = 0.0f;   // slowly increases, drives cloud X-drift
unsigned int backgroundTexture = 0;   // global — used by drawEnvironment()
// ═══════════════════════════════════════════════════════════════════════════════
//  INTERACTIVE GAME STATE
//  Shoot a ball toward a moving target goal on the far end (z ≈ 17.5).
//  J / L     → rotate aim arrow left / right
//  G         → shoot ball
//  B         → restart after game-over
//  I / K / P → difficulty 1 / 2 / 3 (target speed: slow / medium / fast)
// ═══════════════════════════════════════════════════════════════════════════════
float angle_ball = 0.0f;   // aim angle (degrees, around Y axis)

bool ball_shoot = false;
float ball_x = 0.0f;
float bz = 0.0f;

float bar_x = 0.0f;   // moving target X offset
float bar_inc = 1.0f;   // direction multiplier (+1.0 / -1.0)

// Ball trajectory is controlled using curve interpolation (cubic Bezier)
bool  ball_returning = false; // true → ball arcs back to start after goal
float ball_ret_t = 0.0f; // return parameter, goes 1.0 → 0.0
float ball_ret_x0 = 0.0f; // ball's X position captured at goal moment
float ball_ret_z0 = 0.0f; // ball's Z position captured at goal moment

bool  ball_in_net = false; // ball net-এর ভেতরে settle হচ্ছে
float ball_net_t = 0.0f;  // settle progress: 0 → 1
float netWaveTime = 0.0f;      // drives the wave animation
bool  netWaveActive = false;   // true while net should be rippling
bool  showGoalText = false;   // true → "GOAL!" visible on scoreboard
float goalTextTimer = 0.0f;   // counts up; hide when ball_returning ends
int   score = 0;      // current score (0-9)
//int   score = 5;      // starting lives (0-9); game over when it hits 0
bool  start = true;   // false after first shot
bool  gameOver = false;
float slope_ball = 0.0f;   // tan(angle_ball) — lateral drift per unit Z
int   ball_count = 0;      // total shots fired (ever-incrementing)
int   serial_num = 0;      // matches ball_count to prevent double-score
int   difficult = 0;      // 0=static target, 1/2/3=speeds

// Ball world-Z starts at 4.0; goal-2 net is at world-Z = 17.5.
// We trigger scoring when bz >= HIT_DIST (ball reaches just in front of net).
//static const float BALL_START_Z = 1.0f;    // close to camera → appears centered
static const float BALL_START_Z = 8.5f;
static const float HIT_DIST = 9.0f;   // 8.5 + 9.0 = 17.5 (Goal 2)  // 4.0 + 13.5 = 17.5   // 1 + 16.5 = 17.5 (Goal 2)

// ═══════════════════════════════════════════════════════════════════════════════
//  FOOTBALL NET GLOBALS  (Bezier arch top + Ruled Surface mesh)
// ═══════════════════════════════════════════════════════════════════════════════
GLfloat netArchCtrlPts[] = {
    -1.00f,  0.00f, 0.0f,
    -0.50f, -0.05f, 0.0f,
     0.00f, -0.08f, 0.0f,
     0.50f, -0.05f, 0.0f,
     1.00f,  0.00f, 0.0f
};
const int NET_ARCH_L = 4;

vector<float>        netVertices;
vector<unsigned int> netIndices;
unsigned int         netVAO = 0, netVBO = 0, netEBO = 0;
// ── ROOF GLOBALS (Catmull-Rom Spline Surface) ─────────────────────────────────
struct SplinePoint { float x, y; };
// x = unified radial index (world: wx≈x*kx, wz−cz≈z*kz); y = height. Band sits outside all seat tiers (~rd≤16.8) + gap.
SplinePoint roofSplineCtrl[] = {
    { 17.20f, 5.90f}, { 17.45f, 6.10f}, { 18.00f, 5.15f},
    { 18.55f, 4.35f}, { 19.10f, 3.95f}, { 19.60f, 3.82f},
    { 20.00f, 3.88f}, { 20.45f, 4.08f}
};
const int ROOF_CTRL_COUNT = 8;
vector<float>        roofVertices;
vector<unsigned int> roofIndices;
unsigned int roofVAO = 0, roofVBO = 0, roofEBO = 0;

// ─── myLookAt / myOrthographic / myProjection ────────────────────────────────
glm::mat4 myLookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
    glm::vec3 f = glm::normalize(center - eye);
    glm::vec3 r = glm::normalize(glm::cross(f, up));
    glm::vec3 u = glm::cross(r, f);
    glm::mat4 m(1.0f);
    m[0][0] = r.x; m[1][0] = r.y; m[2][0] = r.z;
    m[0][1] = u.x; m[1][1] = u.y; m[2][1] = u.z;
    m[0][2] = -f.x; m[1][2] = -f.y; m[2][2] = -f.z;
    m[3][0] = -glm::dot(r, eye); m[3][1] = -glm::dot(u, eye); m[3][2] = glm::dot(f, eye);
    return m;
}
glm::mat4 myOrthographic(float l, float r, float b, float t, float n, float f)
{
    glm::mat4 m(1.0f);
    m[0][0] = 2 / (r - l); m[1][1] = 2 / (t - b); m[2][2] = -2 / (f - n);
    m[3][0] = -(r + l) / (r - l); m[3][1] = -(t + b) / (t - b); m[3][2] = -(f + n) / (f - n);
    return m;
}
glm::mat4 myProjection(float fov, float aspect, float near, float far)
{
    float t = tan(fov / 2.0f);
    glm::mat4 m(0.0f);
    m[0][0] = 1 / (aspect * t); m[1][1] = 1 / t;
    m[2][2] = -(far + near) / (far - near); m[2][3] = -1;
    m[3][2] = -(2 * far * near) / (far - near);
    return m;
}

// ─── loadTexture ─────────────────────────────────────────────────────────────
unsigned int loadTexture(char const* path, GLenum wS, GLenum wT, GLenum miF, GLenum maF)
{
    unsigned int id; glGenTextures(1, &id);
    int w, h, nc; stbi_set_flip_vertically_on_load(true);
    unsigned char* d = stbi_load(path, &w, &h, &nc, 0);
    if (d) {
        GLenum fmt = (nc == 1) ? GL_RED : (nc == 3) ? GL_RGB : GL_RGBA;
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, d);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, miF);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maF);
        stbi_image_free(d); cout << "Texture loaded: " << path << "\n";
    }
    else { cout << "Texture FAILED: " << path << "\n"; stbi_image_free(d); }
    return id;
}

// ═══════════════════════════════════════════════════════════════════════════════
//  BEZIER + RULED SURFACE — Football Net
// ═══════════════════════════════════════════════════════════════════════════════
long long nCr(int n, int r)
{
    if (r > n / 2) r = n - r;
    long long ans = 1;
    for (int i = 1; i <= r; i++) { ans *= n - r + i; ans /= i; }
    return ans;
}

void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L)
{
    double x = 0, y = 0;
    t = t > 1.0 ? 1.0 : t;
    for (int i = 0; i <= L; i++) {
        long long ncr = nCr(L, i);
        double coef = pow(1.0 - t, double(L - i)) * pow(t, double(i)) * ncr;
        x += coef * ctrlpoints[i * 3];
        y += coef * ctrlpoints[i * 3 + 1];
    }
    xy[0] = float(x); xy[1] = float(y);
}

// Ball trajectory is controlled using curve interpolation (3D cubic Bezier)
void BezierCurve3D(double t, float xyz[3], GLfloat ctrl[], int L)
{
    double x = 0, y = 0, z = 0;
    if (t > 1.0) t = 1.0;
    for (int i = 0; i <= L; i++) {
        double c = (double)nCr(L, i) * pow(1.0 - t, L - i) * pow(t, i);
        x += c * ctrl[i * 3];
        y += c * ctrl[i * 3 + 1];
        z += c * ctrl[i * 3 + 2];
    }
    xyz[0] = (float)x;
    xyz[1] = (float)y;
    xyz[2] = (float)z;
}
void buildFootballNet(float goalWidth, float goalHeight, float netDepth,
    int cols, int rows, float waveTime)
{
    netVertices.clear(); netIndices.clear();
    for (int r = 0; r <= rows; r++) {
        float rowT = float(r) / float(rows);
        for (int c = 0; c <= cols; c++) {
            float colT = float(c) / float(cols);
            float bxy[2];
            BezierCurve(double(colT), bxy, netArchCtrlPts, NET_ARCH_L);
            float topX = bxy[0] * (goalWidth * 0.5f);
            float topY = goalHeight + bxy[1];
            float botX = topX, botY = 0.0f, botZ = -netDepth;
            float x = (1.0f - rowT) * topX + rowT * botX;
            float y = (1.0f - rowT) * topY + rowT * botY;
            float z = (1.0f - rowT) * 0.0f + rowT * botZ;

            // ── Wave deformation ──────────────────────────────────────────
            if (waveTime > 0.0f) {
                float decay = expf(-waveTime * 1.8f);          // fades out over time
                float waveAmp = 0.06f * decay;
                float freq = 6.0f;
                float speed = 8.0f;
                // ripple spreads outward from centre of net
                float dist = sqrtf(x * x + (y - goalHeight * 0.5f) * (y - goalHeight * 0.5f));
                float wave = waveAmp * sinf(freq * dist - speed * waveTime);
                x += wave * (x / (goalWidth * 0.5f + 0.001f));   // push outward in X
                z -= wave;                                         // push backward in Z
            }

            netVertices.push_back(x); netVertices.push_back(y); netVertices.push_back(z);
            // recompute a simple normal
            netVertices.push_back(0.0f); netVertices.push_back(0.0f); netVertices.push_back(1.0f);
        }
    }
    int totalCols = cols + 1;
    for (int r = 0; r < rows; r++) for (int c = 0; c < cols; c++) {
        unsigned int k1 = r * totalCols + c, k2 = k1 + totalCols;
        netIndices.push_back(k1); netIndices.push_back(k2);     netIndices.push_back(k1 + 1);
        netIndices.push_back(k1 + 1); netIndices.push_back(k2); netIndices.push_back(k2 + 1);
    }
    if (netVAO == 0) glGenVertexArrays(1, &netVAO);
    if (netVBO == 0) glGenBuffers(1, &netVBO);
    if (netEBO == 0) glGenBuffers(1, &netEBO);
    glBindVertexArray(netVAO);
    glBindBuffer(GL_ARRAY_BUFFER, netVBO);
    // Use GL_DYNAMIC_DRAW since we update every frame during wave
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(netVertices.size() * sizeof(float)),
        netVertices.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, netEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(netIndices.size() * sizeof(unsigned int)),
        netIndices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}
void drawFootballNet(Shader& lightingShader, glm::mat4 model, float r, float g, float b)
{
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.3f));
    lightingShader.setVec3("material.emissive", glm::vec3(0.0f));
    lightingShader.setFloat("material.shininess", 16.0f);
    lightingShader.setMat4("model", model);
    glBindVertexArray(netVAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(1.2f);
    glDrawElements(GL_TRIANGLES, (unsigned int)netIndices.size(), GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(0);
}

// ═══════════════════════════════════════════════════════════════════════════════
//  CATMULL-ROM SPLINE SURFACE — Stadium Roof
// ═══════════════════════════════════════════════════════════════════════════════
glm::vec2 catmullRom(SplinePoint p0, SplinePoint p1, SplinePoint p2, SplinePoint p3, float t)
{
    float t2 = t * t, t3 = t2 * t;
    float b0 = -0.5f * t3 + t2 - 0.5f * t;
    float b1 = 1.5f * t3 - 2.5f * t2 + 1.0f;
    float b2 = -1.5f * t3 + 2.0f * t2 + 0.5f * t;
    float b3 = 0.5f * t3 - 0.5f * t2;
    return glm::vec2(b0 * p0.x + b1 * p1.x + b2 * p2.x + b3 * p3.x,
        b0 * p0.y + b1 * p1.y + b2 * p2.y + b3 * p3.y);
}

void buildStadiumRoof(float rx, float rz, float centerX, float centerZ, int sweepSegs, int splineSegs)
{
    roofVertices.clear(); roofIndices.clear();
    vector<glm::vec2> profile;
    for (int seg = 1; seg <= ROOF_CTRL_COUNT - 2; seg++) {
        SplinePoint p0 = roofSplineCtrl[seg - 1];
        SplinePoint p1 = roofSplineCtrl[seg];
        SplinePoint p2 = roofSplineCtrl[seg + 1];
        SplinePoint p3 = (seg + 2 < ROOF_CTRL_COUNT) ? roofSplineCtrl[seg + 2] : roofSplineCtrl[seg + 1];
        for (int s = 0; s < splineSegs; s++)
            profile.push_back(catmullRom(p0, p1, p2, p3, float(s) / float(splineSegs)));
    }
    profile.push_back({ roofSplineCtrl[ROOF_CTRL_COUNT - 1].x,
                        roofSplineCtrl[ROOF_CTRL_COUNT - 1].y });
    int pc = (int)profile.size();
    const float kx = rx / 10.0f;
    const float kz = rz / 18.0f;
    vector<glm::vec3> pos((size_t)(sweepSegs + 1) * (size_t)pc);
    auto idx = [pc](int a, int p) { return a * pc + p; };
    for (int a = 0; a <= sweepSegs; a++) {
        float angle = 2.0f * (float)M_PI * float(a) / float(sweepSegs);
        float cosA = cos(angle), sinA = sin(angle);
        for (int p = 0; p < pc; p++) {
            float rd = profile[p].x, ht = profile[p].y;
            float wx = cosA * rd * kx + centerX;
            float wy = ht;
            float wz = sinA * rd * kz + centerZ;
            pos[idx(a, p)] = glm::vec3(wx, wy, wz);
        }
    }
    for (int a = 0; a <= sweepSegs; a++) {
        int ap, an;
        if (a == 0 || a == sweepSegs) { ap = sweepSegs - 1; an = 1; }
        else { ap = a - 1; an = a + 1; }
        float angle = 2.0f * (float)M_PI * float(a) / float(sweepSegs);
        float cosA = cos(angle), sinA = sin(angle);
        for (int p = 0; p < pc; p++) {
            glm::vec3 u = (pos[idx(an, p)] - pos[idx(ap, p)]) * 0.5f;
            glm::vec3 v;
            if (p == 0)
                v = pos[idx(a, 1)] - pos[idx(a, 0)];
            else if (p == pc - 1)
                v = pos[idx(a, pc - 1)] - pos[idx(a, pc - 2)];
            else
                v = (pos[idx(a, p + 1)] - pos[idx(a, p - 1)]) * 0.5f;
            glm::vec3 n = glm::cross(u, v);
            float len = glm::length(n);
            if (len < 1e-6f) {
                n = glm::vec3(cosA * kx, 0.35f, sinA * kz);
                len = glm::length(n);
            }
            n /= len;
            glm::vec3 P = pos[idx(a, p)];
            roofVertices.push_back(P.x); roofVertices.push_back(P.y); roofVertices.push_back(P.z);
            roofVertices.push_back(n.x); roofVertices.push_back(n.y); roofVertices.push_back(n.z);
        }
    }
    for (int a = 0; a < sweepSegs; a++)
        for (int p = 0; p < pc - 1; p++) {
            unsigned int k1 = a * pc + p, k2 = (a + 1) * pc + p;
            roofIndices.push_back(k1);     roofIndices.push_back(k2);     roofIndices.push_back(k1 + 1);
            roofIndices.push_back(k1 + 1); roofIndices.push_back(k2);     roofIndices.push_back(k2 + 1);
        }
    if (roofVAO == 0) glGenVertexArrays(1, &roofVAO);
    if (roofVBO == 0) glGenBuffers(1, &roofVBO);
    if (roofEBO == 0) glGenBuffers(1, &roofEBO);
    glBindVertexArray(roofVAO);
    glBindBuffer(GL_ARRAY_BUFFER, roofVBO);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(roofVertices.size() * sizeof(float)),
        roofVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, roofEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(roofIndices.size() * sizeof(unsigned int)),
        roofIndices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    cout << "[Roof] Spline surface built: " << roofVertices.size() / 6 << " verts\n";
}

void drawStadiumRoof(Shader& sh, glm::mat4 model, float r, float g, float b)
{
    sh.use();
    const float shadeR = 108.0f / 255.0f, shadeG = 117.0f / 255.0f, shadeB = 125.0f / 255.0f;
    sh.setVec3("material.ambient", glm::vec3(shadeR * 0.72f, shadeG * 0.72f, shadeB * 0.72f));
    sh.setVec3("material.diffuse", glm::vec3(r, g, b));
    sh.setVec3("material.specular", glm::vec3(0.14f, 0.145f, 0.15f));
    sh.setVec3("material.emissive", glm::vec3(0));
    sh.setFloat("material.shininess", 20.0f);
    sh.setVec3("rimColor", glm::vec3(79.0f / 255.0f, 195.0f / 255.0f, 247.0f / 255.0f));
    sh.setFloat("rimStrength", 0.4f);
    sh.setMat4("model", model);
    glBindVertexArray(roofVAO);
    glDrawElements(GL_TRIANGLES, (unsigned int)roofIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    sh.setFloat("rimStrength", 0.0f);
}

void drawSkyGradient(Shader& skyShader, unsigned int skyVAO, unsigned int skyTexture)
{
    skyShader.use();
    skyShader.setBool("useTexture", false);
    // Realistic daytime sky colors
    skyShader.setVec3("skyTop",    glm::vec3(0.15f, 0.35f, 0.72f));   // deep cerulean zenith
    skyShader.setVec3("skyMiddle", glm::vec3(0.45f, 0.65f, 0.90f));   // soft sky blue
    skyShader.setVec3("skyBottom", glm::vec3(0.75f, 0.85f, 0.95f));   // warm pale horizon
    glBindVertexArray(skyVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}
// ─────────────────────────────────────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────────────────────────────────────
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
        "Football Stadium - Interactive Game", NULL, NULL);
    if (!window) { cout << "GLFW window failed\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { cout << "GLAD failed\n"; return -1; }
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    glEnable(GL_DEPTH_TEST);

    // ── Shaders ──────────────────────────────────────────────────────────────
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");
    Shader skyShader("skyGradient.vs", "skyGradient.fs");

    float skyTri[] = {
        -1.0f, -1.0f,  0.0f, 0.0f,
         3.0f, -1.0f,  2.0f, 0.0f,
        -1.0f,  3.0f,  0.0f, 2.0f
    };
    unsigned int skyVAO, skyVBO;
    glGenVertexArrays(1, &skyVAO); glGenBuffers(1, &skyVBO);
    glBindVertexArray(skyVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyTri), skyTri, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // ── Textures ─────────────────────────────────────────────────────────────
    cout << "\n=== Loading Textures ===\n";
    unsigned int fieldTexture = loadTexture("field.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int grassTexture = loadTexture("grass.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int footballTexture = loadTexture("football.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int scoreTexture = loadTexture("score.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int gameOverTexture = loadTexture("GameOver.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int playerHeadTex = loadTexture("player_head.png", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int playerShirtTex = loadTexture("player_shirt.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int playerShortsTex = loadTexture("player_shorts.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int playerLegsTex = loadTexture("player_legs.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int playerBootsTex = loadTexture("player_boots.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int playerHairTex = loadTexture("player_hair.png", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int skyTexture = loadTexture("sky.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    backgroundTexture = loadTexture("background.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    // ── Cube VAO ─────────────────────────────────────────────────────────────
    float cube_vertices[] = {
        0,0,0,0,0,-1,0,0,   1,0,0,0,0,-1,1,0,   1,1,0,0,0,-1,1,1,   0,1,0,0,0,-1,0,1,
        1,0,0,1,0,0,0,0,    1,1,0,1,0,0,1,0,    1,0,1,1,0,0,1,1,    1,1,1,1,0,0,0,1,
        0,0,1,0,0,1,0,0,    1,0,1,0,0,1,1,0,    1,1,1,0,0,1,1,1,    0,1,1,0,0,1,0,1,
        0,0,1,-1,0,0,0,0,   0,1,1,-1,0,0,1,0,   0,1,0,-1,0,0,1,1,   0,0,0,-1,0,0,0,1,
        1,1,1,0,1,0,0,0,    1,1,0,0,1,0,1,0,    0,1,0,0,1,0,1,1,    0,1,1,0,1,0,0,1,
        0,0,0,0,-1,0,0,0,   1,0,0,0,-1,0,1,0,   1,0,1,0,-1,0,1,1,   0,0,1,0,-1,0,0,1
    };
    unsigned int cube_indices[] = {
        0,3,2,2,1,0, 4,5,7,7,6,4, 8,9,10,10,11,8,
        12,13,14,14,15,12, 16,17,18,18,19,16, 20,21,22,22,23,20
    };
    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO); glGenBuffers(1, &cubeVBO); glGenBuffers(1, &cubeEBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);            glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);

    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); glEnableVertexAttribArray(0);

    // ── Cone VAO ─────────────────────────────────────────────────────────────
    const int   CONE_SEGS = 36;
    const float CONE_R = 0.5f, CONE_H = 1.0f;
    float slopeLen = sqrtf(CONE_R * CONE_R + CONE_H * CONE_H);
    float nSlope = CONE_R / slopeLen, nUp = CONE_H / slopeLen;
    vector<float> coneVerts;
    int coneVertCount = 0;
    for (int i = 0; i < CONE_SEGS; ++i) {
        float a1 = 2 * (float)M_PI * i / CONE_SEGS, a2 = 2 * (float)M_PI * (i + 1) / CONE_SEGS, amid = (a1 + a2) * 0.5f;
        glm::vec3 nA = glm::normalize(glm::vec3(nSlope * cos(amid), nUp, nSlope * sin(amid)));
        glm::vec3 n1 = glm::normalize(glm::vec3(nSlope * cos(a1), nUp, nSlope * sin(a1)));
        glm::vec3 n2 = glm::normalize(glm::vec3(nSlope * cos(a2), nUp, nSlope * sin(a2)));
        coneVerts.insert(coneVerts.end(), { 0,CONE_H,0,nA.x,nA.y,nA.z,0.5f,1.0f });
        coneVerts.insert(coneVerts.end(), { CONE_R * cos(a1),0,CONE_R * sin(a1),n1.x,n1.y,n1.z,(float)i / CONE_SEGS,0 });
        coneVerts.insert(coneVerts.end(), { CONE_R * cos(a2),0,CONE_R * sin(a2),n2.x,n2.y,n2.z,(float)(i + 1) / CONE_SEGS,0 });
        coneVertCount += 3;
    }
    for (int i = 0; i < CONE_SEGS; ++i) {
        float a1 = 2 * (float)M_PI * i / CONE_SEGS, a2 = 2 * (float)M_PI * (i + 1) / CONE_SEGS;
        coneVerts.insert(coneVerts.end(), { 0,0,0,0,-1,0,0.5f,0.5f });
        coneVerts.insert(coneVerts.end(), { CONE_R * cos(a2),0,CONE_R * sin(a2),0,-1,0,0.5f + 0.5f * cos(a2),0.5f + 0.5f * sin(a2) });
        coneVerts.insert(coneVerts.end(), { CONE_R * cos(a1),0,CONE_R * sin(a1),0,-1,0,0.5f + 0.5f * cos(a1),0.5f + 0.5f * sin(a1) });
        coneVertCount += 3;
    }
    unsigned int coneVAO, coneVBO;
    glGenVertexArrays(1, &coneVAO); glGenBuffers(1, &coneVBO);
    glBindVertexArray(coneVAO);
    glBindBuffer(GL_ARRAY_BUFFER, coneVBO);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(coneVerts.size() * sizeof(float)), coneVerts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);            glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); glEnableVertexAttribArray(2);

    Sphere sphere = Sphere();

    buildFootballNet(2.0f, 1.0f, 0.8f, 20, 12, 0.0f);
    buildStadiumRoof(9.55f, 29.47f, 0.0f, 8.5f, 104, 16);

    cout << "\n=============== FOOTBALL STADIUM — INTERACTIVE GAME ===============\n";
    cout << "CAMERA : W/S/A/D  SPACE/CTRL  ArrowKeys  Q/E\n";
    cout << "LIGHTS : 1=Dir  2=Point  3=Spot  4=Emissive  5=Ambient  6=Diffuse  7=Specular\n";
    cout << "TEXTURE: M=Material  N=Texture  O=Blended\n";
    cout << "GAME   : J/L=Aim  G=Shoot  B=Restart  I/K/P=Difficulty 1/2/3\n";
    cout << "======================s===============================================\n\n";

    // ── Render loop ───────────────────────────────────────────────────────────
    while (!glfwWindowShouldClose(window))
    {
        float now = (float)glfwGetTime();
        deltaTime = now - lastFrame; lastFrame = now;
        processInput(window);

        // ── Update moving target ──────────────────────────────────────────────
        // Speed: slow=0.01/frame, medium=0.02/frame, fast=0.03/frame (same as demo)
        if (!gameOver) {
            if (difficult == 1) bar_x += 0.5f * deltaTime * bar_inc;
            else if (difficult == 2) bar_x += 1.0f * deltaTime * bar_inc;
            else if (difficult == 3) bar_x += 1.5f * deltaTime * bar_inc;
        }
        // Bounce at field edges (field half-width 2.5, target half-width 0.4)
        if (bar_x - 0.4f <= -2.1f) bar_inc = 1.0f;
        if (bar_x + 0.4f >= 2.1f) bar_inc = -1.0f;

        //// ── Update ball in flight ─────────────────────────────────────────────
        //if (ball_shoot) {
        //    bz += 0.03f;
        //    ball_x = slope_ball * bz;
        //} 

        // ── Animation update ─────────────────────────────────────────────────────
        switch (kickState)
        {
        case IDLE:
            runCycle = 0.0f;
            runLegAngle = 0.0f;
            runArmAngle = 0.0f;
            personRunZ = 0.0f;
            legAngle = 0.0f;
            bodyLean = 0.0f;
            leftLegAngle = 0.0f;
            break;

        case RUN_TO_BALL:
        {
            // Person runs forward toward ball (0.50 sec)
            // Start pos = BALL_START_Z - 1.0; must reach BALL_START_Z - 0.15
            // Total run distance = 0.85 units
            kickTimer += deltaTime;
            float runDuration = 0.10f;
            float runProgress = glm::clamp(kickTimer / runDuration, 0.0f, 1.0f);
            personRunZ = runProgress * 0.85f;

            runCycle += deltaTime * 950.0f;
            if (runCycle > 360.0f) runCycle -= 360.0f;
            runLegAngle = 30.0f * sinf(glm::radians(runCycle));
            runArmAngle = 20.0f * sinf(glm::radians(runCycle + 180.0f));
            bodyLean = 8.0f;   // slight forward lean while running

            if (kickTimer >= runDuration) {
                kickState = BACKSWING;
                kickTimer = 0.0f;
                runLegAngle = 0.0f;
                runArmAngle = 0.0f;
            }
            break;
        }

        case BACKSWING:
        {
            // Kick leg swings backward, body leans back slightly (0.18 sec)
            kickTimer += deltaTime;
            float t = glm::clamp(kickTimer / 0.18f, 0.0f, 1.0f);
            legAngle = -55.0f * t;              // hip swings back to -55 deg
            bodyLean = 8.0f - 12.0f * t;        // lean shifts backward
            leftLegAngle = 8.0f * t;             // standing leg braces slightly
            if (kickTimer >= 0.18f) {
                kickState = KICK_FORWARD;
                kickTimer = 0.0f;
            }
            break;
        }

        case KICK_FORWARD:
        {
            // Kick leg whips forward FAST (0.12 sec) — ball launches at t≈0.85
            kickTimer += deltaTime;
            float t = glm::clamp(kickTimer / 0.12f, 0.0f, 1.0f);
            // Smooth acceleration: slow start, fast through contact
            float eased = t * t * (3.0f - 2.0f * t);  // smoothstep
            legAngle = -55.0f + 130.0f * eased;        // -55 → +75 deg
            bodyLean = -4.0f + 18.0f * eased;          // lean forward into kick
            leftLegAngle = 8.0f - 5.0f * t;            // standing leg settles

            // Launch ball when foot reaches contact point (~85% through swing)
            if (t >= 0.85f && !ball_shoot) {
                ball_count++;
                serial_num = ball_count;
                ball_shoot = true;
                slope_ball = tanf(3.14159f * angle_ball / 180.0f);
            }
            if (kickTimer >= 0.12f) {
                kickState = FOLLOW_THROUGH;
                kickTimer = 0.0f;
            }
            break;
        }

        case FOLLOW_THROUGH:
        {
            // Leg decelerates, body returns upright (0.35 sec)
            kickTimer += deltaTime;
            float t = glm::clamp(kickTimer / 0.35f, 0.0f, 1.0f);
            legAngle = 75.0f - 75.0f * t;              // 75 → 0
            bodyLean = 14.0f - 14.0f * t;               // lean → upright
            leftLegAngle = 3.0f * (1.0f - t);
            if (kickTimer >= 0.35f) {
                kickState = DONE;
                kickTimer = 0.0f;
                legAngle = 0.0f;
                bodyLean = 0.0f;
                leftLegAngle = 0.0f;
            }
            break;
        }

        case DONE:
            legAngle = 0.0f;
            bodyLean = 0.0f;
            leftLegAngle = 0.0f;
            runCycle = 0.0f;
            personRunZ = 0.0f;
            kickState = IDLE;
            break;
        }

        // ── Celebration animation ─────────────────────────────────────────────────
        if (celebrating) {
            celebTimer += deltaTime;
            armAngle = 70.0f * fabsf(sinf(celebTimer * 6.0f));
            personJumpY = 0.20f * fabsf(sinf(celebTimer * 6.0f));
            if (celebTimer > 2.5f) {
                celebrating = false;
                celebTimer = 0.0f;
                armAngle = 0.0f;
                personJumpY = 0.0f;
            }
        }

        // ── Ball in flight update (Bezier trajectory) ─────────────────────────────
        if (ball_shoot) {
            bz += 0.42f;
            ball_x = slope_ball * bz;
        }

        // ── Return arc: ball arcs back to start after goal ────────────────────────
        // ── Net settle phase: ball rolls into net after goal ──────────────────────
        // ── Net settle phase: ball enters net, waits for celebration to end ────────
        if (ball_in_net) {
            // Step 1: quick settle into net (~0.4s)
            if (ball_net_t < 1.0f) {
                ball_net_t += deltaTime * 2.5f;
                if (ball_net_t > 1.0f) ball_net_t = 1.0f;
            }
            // Step 2: ball stays in net until celebration is fully done
            if (ball_net_t >= 1.0f && !celebrating) {
                ball_in_net = false;
                ball_returning = true;
                ball_ret_t = 1.0f;
            }
        }

        // ── Ground roll return: ball bounces back from net to center ──────────────
        if (ball_returning) {
            ball_ret_t -= 0.050f;
            if (ball_ret_t <= 0.0f) {
                ball_ret_t = 0.0f;
                ball_returning = false;
                ball_x = 0.0f;
                bz = 0.0f;
            }
        }

        // ── Collision detection: ball reaches far goal (z ≈ 17.0) ────────────
        if (bz >= HIT_DIST && !ball_returning)
        {
            start = false;
            if (serial_num == ball_count)
            {
                if (ball_x > bar_x - 1.0f && ball_x < bar_x + 1.0f) {
                    if (score < 9) score++;
                    cout << "[GOAL!] Score: " << score << "\n";
                    celebrating = true;
                    celebTimer = 0.0f;
                    netWaveActive = true;
                    netWaveTime = 0.0f;
                    // ── Trigger net settle first, then ground roll return ────
                    ball_in_net = true;
                    ball_net_t = 0.0f;
                    ball_ret_x0 = ball_x;  // capture final X position
                }
                else {
                    cout << "[MISS]  Game Over!\n";
                    gameOver = true;
                    ball_x = 0.0f;
                    bz = 0.0f;
                }
                serial_num--;
            }
            ball_shoot = false;
            // Note: bz/ball_x reset হবে return arc শেষ হলে (CHANGE 4)
            if (!ball_returning && !ball_in_net) {
                ball_x = 0.0f;
                bz = 0.0f;
            }
        }
        // if (!start && score == 0) gameOver = true;
        // ── Net wave animation update ─────────────────────────────────────
        if (netWaveActive) {
            netWaveTime += deltaTime;
            buildFootballNet(2.0f, 1.0f, 0.8f, 20, 12, netWaveTime);
            if (netWaveTime > 3.5f) {          // wave fully decayed, stop rebuilding
                netWaveActive = false;
                netWaveTime = 0.0f;
                buildFootballNet(2.0f, 1.0f, 0.8f, 20, 12, 0.0f);  // restore rest shape
            }
        }

         // ── Render setup ─────────────────────────────────────────────────────
        glViewport(0, 0, framebufferWidth, framebufferHeight);
        glClearColor(170.0f / 255.0f, 210.0f / 255.0f, 240.0f / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cloudDriftTime += deltaTime * 0.15f;   // slow cloud drift
        glDepthMask(GL_FALSE);
        glDisable(GL_DEPTH_TEST);
        drawSkyGradient(skyShader, skyVAO, skyTexture);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);

        // Build camera matrices once (used by single-view AND game overlay)
        glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom),
            (float)framebufferWidth / (float)framebufferHeight, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        auto setupShaders = [&](glm::vec3 vp) {
            lightingShader.use();
            lightingShader.setVec3("viewPos", vp);
            lightingShader.setFloat("rimStrength", 0.0f);
            lightingShader.setVec3("rimColor", glm::vec3(0.0f));
            pointlight1.setUpPointLight(lightingShader);
            pointlight2.setUpPointLight(lightingShader);
            pointlight3.setUpPointLight(lightingShader);
            pointlight4.setUpPointLight(lightingShader);
            directlight.setUpDirectionLight(lightingShader);
            spotlight.setUpSpotLight(lightingShader);

            lightingShaderWithTexture.use();
            lightingShaderWithTexture.setVec3("viewPos", vp);
            pointlight1.setUpPointLight(lightingShaderWithTexture);
            pointlight2.setUpPointLight(lightingShaderWithTexture);
            pointlight3.setUpPointLight(lightingShaderWithTexture);
            pointlight4.setUpPointLight(lightingShaderWithTexture);
            directlight.setUpDirectionLight(lightingShaderWithTexture);
            spotlight.setUpSpotLight(lightingShaderWithTexture);
            };

        if (activeViewport == 0)
        {
            setupShaders(camera.Position);
            lightingShader.setMat4("projection", proj);
            lightingShader.setMat4("view", view);
            lightingShaderWithTexture.setMat4("projection", proj);
            lightingShaderWithTexture.setMat4("view", view);



            renderScene(cubeVAO, coneVAO, coneVertCount,
                lightingShader, lightingShaderWithTexture, sphere,
                proj, view, emissiveOn, textureMode,
                fieldTexture, grassTexture, footballTexture, scoreTexture,
                bar_x);

            // Light-cube markers
            ourShader.use();
            ourShader.setMat4("projection", proj);
            ourShader.setMat4("view", view);
            glBindVertexArray(lightCubeVAO);
            for (int i = 0; i < 4; i++) {
                glm::mat4 m = glm::scale(glm::translate(glm::mat4(1), pointLightPositions[i]), glm::vec3(0.4f));
                ourShader.setMat4("model", m);
                ourShader.setVec3("color", pointLightOn ? glm::vec3(0.9f, 0.9f, 0.7f) : glm::vec3(0.1f));
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }

            // ════════════════════════════════════════════════════════════════
            //  GAME OVERLAY — drawn on top of the stadium scene
            //  Uses lightingShader (already configured with proj/view above)
            // ════════════════════════════════════════════════════════════════
            lightingShader.use();
            lightingShader.setMat4("projection", proj);
            lightingShader.setMat4("view", view);

            glm::mat4 I = glm::mat4(1.0f);

            // ── 1. Moving target goal (YELLOW) at z ≈ 16.9 ──────────────────
            //       Slides left/right with bar_x; player must shoot through it


            // ── 2. Aim arrow (WHITE/YELLOW flat slab near ball) ──────────────
            //       Rotates around Y with angle_ball; shows shoot direction
            if (!gameOver)
            {
                glm::mat4 rotArrow = glm::rotate(I, glm::radians(angle_ball), glm::vec3(0, 1, 0));
                glm::mat4 m = glm::translate(I, glm::vec3(-0.02f, 0.37f, BALL_START_Z + 0.1f))
                    * rotArrow
                    * glm::scale(I, glm::vec3(0.04f, 0.008f, 0.55f));
                drawCube(cubeVAO, lightingShader, m, 1.0f, 1.0f, 0.0f);
            }
            // ── Person standing at ball, facing shoot direction ──────────────────────
            if (!gameOver)
            {
                lightingShader.use();
                lightingShader.setMat4("projection", proj);
                lightingShader.setMat4("view", view);
                // Person stands just behind the ball, faces angle_ball direction
                // Choose leg/arm angles based on state
                float displayLegAngle = (kickState == RUN_TO_BALL) ? runLegAngle : legAngle;
                float displayArmAngle = (kickState == RUN_TO_BALL) ? runArmAngle :
                    (celebrating ? armAngle : 0.0f);
                float personZ = BALL_START_Z - 1.0f + personRunZ;  // starts behind, runs toward ball

                drawPerson(cubeVAO, lightingShader, lightingShaderWithTexture,
                    sphere,
                    0.0f, 0.0f, personZ,
                    angle_ball, displayLegAngle, displayArmAngle, personJumpY,
                    playerHeadTex, playerShirtTex,
                    playerShortsTex, playerLegsTex,
                    playerBootsTex, playerHairTex);
            }

            // ── 3. Game ball (football texture, dynamic position) ────────────
            //       Moves forward when ball_shoot=true; otherwise rests at start
            {
                lightingShaderWithTexture.use();
                lightingShaderWithTexture.setMat4("projection", proj);
                lightingShaderWithTexture.setMat4("view", view);
                lightingShaderWithTexture.setBool("useBlend", false);
                lightingShaderWithTexture.setVec3("surfaceColor", glm::vec3(1, 1, 1));
                lightingShaderWithTexture.setVec3("material.specular", glm::vec3(0.8f));
                lightingShaderWithTexture.setVec3("material.emissive", glm::vec3(0));
                lightingShaderWithTexture.setFloat("material.shininess", 32.0f);

                // Ball trajectory is controlled using curve interpolation (cubic Bezier)
                glm::vec3 ballPos;

                if (ball_in_net) {
                    // ── Phase 1: Ball rolls into net, drops to ground ─────────
                    float nt = ball_net_t;                        // 0 → 1
                    float netZ = (BALL_START_Z + HIT_DIST) + 1.5f * nt; // 17.5 → 19.0
                    float netY = 0.7f * (1.0f - nt) + 0.45f * nt;      // drops to ground
                    float netX = ball_ret_x0;
                    ballPos = glm::vec3(netX, netY, netZ);
                }
                else if (ball_returning) {
                    // ── Phase 2: Ball bounces back after celebration ───────────
                    float rt = ball_ret_t;  // 1.0 → 0.0

                    // Z: from behind net (19.0) → center field (8.5)
                    float retZ = BALL_START_Z + 10.5f * rt;

                    // X: smoothly back to center
                    float retX = ball_ret_x0 * rt;

                    // Y: 3 clean bounces, height fades as ball slows
                    // rt*rt makes it ease-out (ball decelerates naturally)
                    float bounce = 0.45f * (rt * rt) * fabsf(sinf(rt * 3.14159f * 3.5f));
                    float retY = 0.45f + bounce;

                    ballPos = glm::vec3(retX, retY, retZ);
                }
                else {
                    // ── Normal flight: Bezier arc forward ─────────────────────
                    float tn = (HIT_DIST > 0.0f) ? glm::clamp(bz / HIT_DIST, 0.0f, 1.0f) : 0.0f;
                    float mt = 1.0f - tn;
                    float bezY = mt * mt * mt * 0.45f + 3 * mt * mt * tn * 2.5f
                        + 3 * mt * tn * tn * 1.8f + tn * tn * tn * 0.7f;
                    ballPos = glm::vec3(ball_x, bezY, BALL_START_Z + bz);
                }

                glm::mat4 ballModel = glm::translate(I, ballPos)
                    * glm::scale(I, glm::vec3(0.15f));
                
                lightingShaderWithTexture.setMat4("model", ballModel);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, footballTexture);
                lightingShaderWithTexture.setInt("material.diffuse", 0);
                glBindVertexArray(sphere.sphereVAO);
                glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, (void*)0);

                // Switch back to lightingShader for remaining overlay elements
                lightingShader.use();
                lightingShader.setMat4("projection", proj);
                lightingShader.setMat4("view", view);
            }
            if (gameOver && !start)
            {
                lightingShaderWithTexture.use();
                lightingShaderWithTexture.setMat4("projection", proj);
                lightingShaderWithTexture.setMat4("view", view);
                lightingShaderWithTexture.setBool("useBlend", false);
                lightingShaderWithTexture.setVec3("surfaceColor", glm::vec3(1, 1, 1));
                lightingShaderWithTexture.setVec3("material.specular", glm::vec3(0.3f));
                lightingShaderWithTexture.setVec3("material.emissive", glm::vec3(0.3f, 0, 0));
                lightingShaderWithTexture.setFloat("material.shininess", 16.0f);
                glm::mat4 goModel = glm::translate(I, glm::vec3(2.9f, 5.9f, 24.72f))
                    * glm::scale(I, glm::vec3(-5.8f, 2.8f, 0.05f));
                lightingShaderWithTexture.setMat4("model", goModel);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, gameOverTexture);
                lightingShaderWithTexture.setInt("material.diffuse", 0);
                glBindVertexArray(cubeVAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }
            // GOAL text now draws in stadium() so it is visible from all camera modes.
            // ── 4. Score indicators — red spheres inside white bar ──
            {
                lightingShader.use();
                lightingShader.setMat4("projection", proj);
                lightingShader.setMat4("view", view);

                for (int i = 0; i < score; i++) {
                    float sbx = -1.6f + i * 0.38f;
                    glm::mat4 sm = glm::translate(I, glm::vec3(sbx, 6.99f, 24.68f))
                        * glm::scale(I, glm::vec3(0.15f));
                    lightingShader.setVec3("material.ambient", glm::vec3(1.0f, 0.0f, 0.0f));
                    lightingShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.0f, 0.0f));
                    lightingShader.setVec3("material.specular", glm::vec3(0.5f));
                    lightingShader.setVec3("material.emissive", glm::vec3(0.5f, 0.0f, 0.0f));
                    lightingShader.setFloat("material.shininess", 32.0f);
                    lightingShader.setMat4("model", sm);
                    glBindVertexArray(sphere.sphereVAO);
                    glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, (void*)0);
                }

                lightingShader.use();
                lightingShader.setMat4("projection", proj);
                lightingShader.setMat4("view", view);
            }

            // ── 5. Game-Over overlay on the scoreboard (red plane) ───────────
            //       Appears when score reaches 0 after the first shot
            if (gameOver && !start)
            {
                lightingShaderWithTexture.use();
                lightingShaderWithTexture.setMat4("projection", proj);
                lightingShaderWithTexture.setMat4("view", view);
                lightingShaderWithTexture.setBool("useBlend", false);
                lightingShaderWithTexture.setVec3("surfaceColor", glm::vec3(1, 1, 1));
                lightingShaderWithTexture.setVec3("material.specular", glm::vec3(0.3f));
                lightingShaderWithTexture.setVec3("material.emissive", glm::vec3(0.3f, 0, 0));
                lightingShaderWithTexture.setFloat("material.shininess", 16.0f);
                glm::mat4 goModel = glm::translate(I, glm::vec3(-2.9f, 5.9f, 24.72f))
                    * glm::scale(I, glm::vec3(5.8f, 2.8f, 0.05f));
                lightingShaderWithTexture.setMat4("model", goModel);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, gameOverTexture);
                lightingShaderWithTexture.setInt("material.diffuse", 0);
                glBindVertexArray(cubeVAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                lightingShader.use();
                lightingShader.setMat4("projection", proj);
                lightingShader.setMat4("view", view);
            }
        }
        else
        {
            // ── Multi-viewport (9 key) — no game overlay in split view ────────
            int hw = framebufferWidth / 2, hh = framebufferHeight / 2;
            glm::vec3 vps[4] = { {15,12,15},{0,20,8.5f},{0,3,-15},{0,2,5} };
            float pit[4] = { -35.264f,-90,0,0 };
            float yaw[4] = { -135,-90,90,90 };

            for (int i = 0; i < 4; i++) {
                int x = (i % 2) * hw, y = (i / 2 == 0) ? hh : 0;
                glViewport(x, y, hw, hh);
                glEnable(GL_SCISSOR_TEST);
                glScissor(x, y, hw, hh);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glDisable(GL_SCISSOR_TEST);

                glm::vec3 fwd;
                fwd.x = cos(glm::radians(yaw[i])) * cos(glm::radians(pit[i]));
                fwd.y = sin(glm::radians(pit[i]));
                fwd.z = sin(glm::radians(yaw[i])) * cos(glm::radians(pit[i]));
                fwd = glm::normalize(fwd);
                glm::mat4 vv = myLookAt(vps[i], vps[i] + fwd, { 0,1,0 });
                glm::mat4 vp;
                float asp = (float)hw / (float)hh;
                if (i == 0) vp = myOrthographic(-12 * asp, 12 * asp, -12, 12, 0.1f, 100);
                else if (i == 1) vp = myProjection(glm::radians(110.0f), asp, 0.1f, 100);
                else          vp = myProjection(glm::radians(75.0f), asp, 0.1f, 100);
                setupShaders(vps[i]);

                renderScene(cubeVAO, coneVAO, coneVertCount,
                    lightingShader, lightingShaderWithTexture, sphere,
                    vp, vv, emissiveOn, textureMode,
                    fieldTexture, grassTexture, footballTexture, scoreTexture,
                    bar_x);

            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO); glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteVertexArrays(1, &coneVAO);
    glDeleteVertexArrays(1, &skyVAO);
    glDeleteBuffers(1, &cubeVBO); glDeleteBuffers(1, &cubeEBO); glDeleteBuffers(1, &coneVBO);
    glDeleteBuffers(1, &skyVBO);
    if (netVAO) glDeleteVertexArrays(1, &netVAO);
    if (netVBO) glDeleteBuffers(1, &netVBO);
    if (netEBO) glDeleteBuffers(1, &netEBO);
    if (roofVAO) glDeleteVertexArrays(1, &roofVAO);
    if (roofVBO) glDeleteBuffers(1, &roofVBO);
    if (roofEBO) glDeleteBuffers(1, &roofEBO);
    glfwTerminate();
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
//  drawPerson — hierarchical skeleton with proper joint chains
//  Hip → Upper leg → Knee → Lower leg → Ankle → Boot
//  Body lean applied at root for natural kick posture
// ─────────────────────────────────────────────────────────────────────────────
void drawPerson(unsigned int& cubeVAO, Shader& lightingShader,
    Shader& lightingShaderWithTexture,
    Sphere& sphere,
    float posX, float posY, float posZ,
    float facingAngle,
    float legSwingAngle,
    float armRaiseAngle,
    float jumpY,
    unsigned int headTex, unsigned int shirtTex,
    unsigned int shortsTex, unsigned int legsTex,
    unsigned int bootsTex, unsigned int hairTex)
{
    glm::mat4 I = glm::mat4(1.0f);
    float py = posY + jumpY;

    // Root transform: position + facing + body lean (forward/backward tilt)
    glm::mat4 root = glm::translate(I, glm::vec3(posX, py, posZ))
        * glm::rotate(I, glm::radians(facingAngle + 180.0f), glm::vec3(0, 1, 0))
        * glm::rotate(I, glm::radians(bodyLean), glm::vec3(1, 0, 0));

    // ── textured cube helper ─────────────────────────────────────────────
    auto tCube = [&](glm::mat4 m, unsigned int tex) {
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setBool("useBlend", false);
        lightingShaderWithTexture.setVec3("surfaceColor", glm::vec3(1, 1, 1));
        lightingShaderWithTexture.setVec3("material.specular", glm::vec3(0.15f));
        lightingShaderWithTexture.setVec3("material.emissive", glm::vec3(0));
        lightingShaderWithTexture.setFloat("material.shininess", 8.0f);
        lightingShaderWithTexture.setMat4("model", m);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
        lightingShaderWithTexture.setInt("material.diffuse", 0);
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        };

    // ── textured sphere helper ───────────────────────────────────────────
    auto tSphere = [&](glm::mat4 m, unsigned int tex) {
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setBool("useBlend", false);
        lightingShaderWithTexture.setVec3("surfaceColor", glm::vec3(1, 1, 1));
        lightingShaderWithTexture.setVec3("material.specular", glm::vec3(0.2f));
        lightingShaderWithTexture.setVec3("material.emissive", glm::vec3(0));
        lightingShaderWithTexture.setFloat("material.shininess", 12.0f);
        lightingShaderWithTexture.setMat4("model", m);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
        lightingShaderWithTexture.setInt("material.diffuse", 0);
        glBindVertexArray(sphere.sphereVAO);
        glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, (void*)0);
        };

    // ════════════════════════════════════════════════════════════════════════
    //  HEAD
    // ════════════════════════════════════════════════════════════════════════
    {
        glm::mat4 m = root
            * glm::translate(I, glm::vec3(0.0f, 1.76f, 0.02f))
            * glm::scale(I, glm::vec3(0.220f));
        tSphere(m, hairTex);
    }
    {
        glm::mat4 m = root
            * glm::translate(I, glm::vec3(0.0f, 1.76f, -0.03f))
            * glm::scale(I, glm::vec3(0.205f, 0.225f, 0.175f));
        tSphere(m, headTex);
    }

    // ════════════════════════════════════════════════════════════════════════
    //  NECK
    // ════════════════════════════════════════════════════════════════════════
    tCube(root
        * glm::translate(I, glm::vec3(-0.020f, 1.52f, -0.010f))
        * glm::scale(I, glm::vec3(0.080f, 0.12f, 0.080f)), legsTex);

    // ════════════════════════════════════════════════════════════════════════
    //  TORSO
    // ════════════════════════════════════════════════════════════════════════
    tCube(root
        * glm::translate(I, glm::vec3(-0.115f, 1.06f, -0.055f))
        * glm::scale(I, glm::vec3(0.23f, 0.40f, 0.13f)), shirtTex);

    // ════════════════════════════════════════════════════════════════════════
    //  SHORTS / HIPS
    // ════════════════════════════════════════════════════════════════════════
    tCube(root
        * glm::translate(I, glm::vec3(-0.115f, 0.80f, -0.055f))
        * glm::scale(I, glm::vec3(0.23f, 0.20f, 0.13f)), shortsTex);

    // ════════════════════════════════════════════════════════════════════════
    //  LEFT ARM (swings opposite to kick arm)
    // ════════════════════════════════════════════════════════════════════════
    {
        float ang = -24.0f - armRaiseAngle;
        glm::mat4 shoulder = root
            * glm::translate(I, glm::vec3(-0.185f, 1.40f, 0.0f))
            * glm::rotate(I, glm::radians(ang), glm::vec3(1, 0, 0));

        tCube(shoulder
            * glm::translate(I, glm::vec3(0.0f, -0.13f, 0.0f))
            * glm::scale(I, glm::vec3(0.072f, 0.28f, 0.072f)), shirtTex);

        // Elbow child of shoulder
        glm::mat4 elbow = shoulder
            * glm::translate(I, glm::vec3(0.0f, -0.38f, 0.0f))
            * glm::rotate(I, glm::radians(glm::clamp(-ang * 0.3f, 0.0f, 30.0f)), glm::vec3(1, 0, 0));

        tCube(elbow
            * glm::translate(I, glm::vec3(0.0f, -0.05f, 0.0f))
            * glm::scale(I, glm::vec3(0.065f, 0.24f, 0.065f)), legsTex);
    }

    // ════════════════════════════════════════════════════════════════════════
    //  RIGHT ARM (swings forward during kick for balance)
    // ════════════════════════════════════════════════════════════════════════
    {
        float ang = 24.0f + armRaiseAngle;
        glm::mat4 shoulder = root
            * glm::translate(I, glm::vec3(0.135f, 1.40f, 0.0f))
            * glm::rotate(I, glm::radians(ang), glm::vec3(1, 0, 0));

        tCube(shoulder
            * glm::translate(I, glm::vec3(0.0f, -0.13f, 0.0f))
            * glm::scale(I, glm::vec3(0.072f, 0.28f, 0.072f)), shirtTex);

        glm::mat4 elbow = shoulder
            * glm::translate(I, glm::vec3(0.0f, -0.38f, 0.0f))
            * glm::rotate(I, glm::radians(glm::clamp(ang * 0.3f, 0.0f, 30.0f)), glm::vec3(1, 0, 0));

        tCube(elbow
            * glm::translate(I, glm::vec3(0.0f, -0.05f, 0.0f))
            * glm::scale(I, glm::vec3(0.065f, 0.24f, 0.065f)), legsTex);
    }

    // ════════════════════════════════════════════════════════════════════════
    //  LEFT LEG — standing/support leg (full hierarchical chain)
    //  Hip → Upper thigh → Knee → Shin → Ankle → Boot
    // ════════════════════════════════════════════════════════════════════════
    {
        // Hip joint: slight counter-swing
        glm::mat4 hip = root
            * glm::translate(I, glm::vec3(-0.062f, 0.80f, 0.0f))
            * glm::rotate(I, glm::radians(leftLegAngle), glm::vec3(1, 0, 0));

        // Upper thigh
        tCube(hip
            * glm::translate(I, glm::vec3(-0.015f, -0.16f, -0.015f))
            * glm::scale(I, glm::vec3(0.095f, 0.29f, 0.095f)), shortsTex);

        // Knee joint — slight bend when bracing
        float lKnee = glm::clamp(leftLegAngle * 0.4f, 0.0f, 15.0f);
        glm::mat4 knee = hip
            * glm::translate(I, glm::vec3(0.0f, -0.36f, 0.0f))
            * glm::rotate(I, glm::radians(lKnee), glm::vec3(1, 0, 0));

        // Shin
        tCube(knee
            * glm::translate(I, glm::vec3(-0.010f, -0.10f, 0.005f))
            * glm::scale(I, glm::vec3(0.080f, 0.27f, 0.080f)), legsTex);

        // Boot — child of knee, properly attached at ankle
        tCube(knee
            * glm::translate(I, glm::vec3(-0.030f, -0.35f, -0.050f))
            * glm::scale(I, glm::vec3(0.108f, 0.080f, 0.205f)), bootsTex);
    }

    // ════════════════════════════════════════════════════════════════════════
    //  RIGHT LEG — kick leg (full hierarchical chain)
    //  Hip → Upper thigh → Knee → Shin → Ankle → Boot
    // ════════════════════════════════════════════════════════════════════════
    {
        // Hip joint: main kick rotation
        glm::mat4 hip = root
            * glm::translate(I, glm::vec3(0.062f, 0.80f, 0.0f))
            * glm::rotate(I, glm::radians(-legSwingAngle), glm::vec3(1, 0, 0));

        // Upper thigh
        tCube(hip
            * glm::translate(I, glm::vec3(-0.015f, -0.16f, -0.015f))
            * glm::scale(I, glm::vec3(0.095f, 0.29f, 0.095f)), shortsTex);

        // Knee bend: bends during both backswing AND forward swing
        float kneeAngle;
        if (legSwingAngle < 0) {
            // Backswing: knee bends to pull foot back (like winding up)
            kneeAngle = fabsf(legSwingAngle) * 0.7f;
        } else if (legSwingAngle > 20.0f) {
            // Forward follow-through: knee straightens out
            kneeAngle = glm::clamp((75.0f - legSwingAngle) * 0.3f, 0.0f, 25.0f);
        } else {
            // Contact zone: leg is fully extended for maximum power
            kneeAngle = glm::clamp(legSwingAngle * 0.2f, 0.0f, 10.0f);
        }

        glm::mat4 knee = hip
            * glm::translate(I, glm::vec3(0.0f, -0.36f, 0.0f))
            * glm::rotate(I, glm::radians(kneeAngle), glm::vec3(1, 0, 0));

        // Shin — child of knee
        tCube(knee
            * glm::translate(I, glm::vec3(-0.010f, -0.10f, 0.005f))
            * glm::scale(I, glm::vec3(0.080f, 0.27f, 0.080f)), legsTex);

        // Boot — child of knee, properly follows shin
        tCube(knee
            * glm::translate(I, glm::vec3(-0.030f, -0.35f, -0.050f))
            * glm::scale(I, glm::vec3(0.108f, 0.080f, 0.205f)), bootsTex);
    }

    // ── Restore lightingShader ─────────────────────────────────────────────
    lightingShader.use();
}

// ─────────────────────────────────────────────────────────────────────────────
//  renderScene  (static stadium + static ball placeholder at centre)
//  NOTE: The game ball is drawn separately in the main loop above so that
//        it responds to game-state globals (ball_x, bz).
// ─────────────────────────────────────────────────────────────────────────────
void renderScene(unsigned int& cubeVAO, unsigned int coneVAO, int coneVertCount,
    Shader& lightingShader, Shader& lightingShaderWithTexture,
    Sphere& sphere, glm::mat4 projection, glm::mat4 view,
    bool emissiveOn, int textureMode,
    unsigned int fieldTexture, unsigned int grassTexture,
    unsigned int footballTexture, unsigned int scoreTexture, float bar_x)
{
    lightingShader.use();
    lightingShader.setMat4("projection", projection);
    lightingShader.setMat4("view", view);
    lightingShaderWithTexture.use();
    lightingShaderWithTexture.setMat4("projection", projection);
    lightingShaderWithTexture.setMat4("view", view);

    glm::mat4 I = glm::mat4(1.0f);

    stadium(cubeVAO, coneVAO, coneVertCount,
        lightingShader, lightingShaderWithTexture, I,
        fieldTexture, grassTexture, footballTexture, scoreTexture,
        textureMode);

    // ── Outdoor environment: green terrain, landscape trees, clouds ───────
    drawEnvironment(cubeVAO, lightingShader, lightingShaderWithTexture,
        sphere, grassTexture, textureMode, cloudDriftTime);

    // Goal 1 — near side (z = -0.5), STATIC
    drawGoalPost(cubeVAO, lightingShader, I, { 0,0.2f,-0.5f });
    {
        glm::mat4 nm = glm::translate(I, glm::vec3(0.0f, 0.2f, -0.5f));
        // Soft beige net (#E8DCCB) — warmer than plain white, blends with dark environment
        drawFootballNet(lightingShader, nm, 1.0f, 1.0f, 1.0f);
    }



    // Goal 2 — far side (z = 17.5), moves with bar_x
    drawGoalPost(cubeVAO, lightingShader, I, { bar_x, 0.2f, 17.5f });
    {
        glm::mat4 nm = glm::translate(I, glm::vec3(bar_x, 0.2f, 17.5f))
            * glm::rotate(I, glm::radians(180.0f), glm::vec3(0, 1, 0));
        // Soft beige net (#E8DCCB) — warmer than plain white, blends with dark environment
        drawFootballNet(lightingShader, nm, 1.0f, 1.0f, 1.0f);
    }

    for (int i = 0; i < 4; i++)
        drawFloodlight(cubeVAO, lightingShader, pointLightPositions[i], emissiveOn);
}

// ─────────────────────────────────────────────────────────────────────────────
//  drawCube
// ─────────────────────────────────────────────────────────────────────────────
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model,
    float r, float g, float b, glm::vec3 emissive)
{
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f));
    lightingShader.setVec3("material.emissive", emissive);
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setMat4("model", model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void drawCubeWithTexture(unsigned int& cubeVAO, Shader& lightingShaderWithTexture,
    glm::mat4 model, unsigned int texture)
{
    lightingShaderWithTexture.use();
    lightingShaderWithTexture.setBool("useBlend", false);
    lightingShaderWithTexture.setVec3("surfaceColor", glm::vec3(1, 1, 1));
    lightingShaderWithTexture.setVec3("material.specular", glm::vec3(0.5f));
    lightingShaderWithTexture.setVec3("material.emissive", glm::vec3(0));
    lightingShaderWithTexture.setFloat("material.shininess", 32.0f);
    lightingShaderWithTexture.setMat4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    lightingShaderWithTexture.setInt("material.diffuse", 0);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void drawCubeBlended(unsigned int& cubeVAO, Shader& lightingShaderWithTexture,
    glm::mat4 model, unsigned int texture, float r, float g, float b)
{
    lightingShaderWithTexture.use();
    lightingShaderWithTexture.setBool("useBlend", true);
    lightingShaderWithTexture.setVec3("surfaceColor", glm::vec3(r, g, b));
    lightingShaderWithTexture.setVec3("material.specular", glm::vec3(0.4f));
    lightingShaderWithTexture.setVec3("material.emissive", glm::vec3(0));
    lightingShaderWithTexture.setFloat("material.shininess", 32.0f);
    lightingShaderWithTexture.setMat4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    lightingShaderWithTexture.setInt("material.diffuse", 0);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Cone helpers
// ─────────────────────────────────────────────────────────────────────────────
void drawCone(unsigned int coneVAO, int vertCount, Shader& lightingShader, glm::mat4 model, float r, float g, float b)
{
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.4f));
    lightingShader.setVec3("material.emissive", glm::vec3(0));
    lightingShader.setFloat("material.shininess", 16.0f);
    lightingShader.setMat4("model", model);
    glBindVertexArray(coneVAO);
    glDrawArrays(GL_TRIANGLES, 0, vertCount);
}
void drawConeWithTexture(unsigned int coneVAO, int vertCount, Shader& s, glm::mat4 model, unsigned int texture)
{
    s.use(); s.setBool("useBlend", false); s.setVec3("surfaceColor", glm::vec3(1, 1, 1));
    s.setVec3("material.specular", glm::vec3(0.4f)); s.setVec3("material.emissive", glm::vec3(0));
    s.setFloat("material.shininess", 16.0f); s.setMat4("model", model);
    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, texture); s.setInt("material.diffuse", 0);
    glBindVertexArray(coneVAO); glDrawArrays(GL_TRIANGLES, 0, vertCount);
}
void drawConeBlended(unsigned int coneVAO, int vertCount, Shader& s, glm::mat4 model, unsigned int texture, float r, float g, float b)
{
    s.use(); s.setBool("useBlend", true); s.setVec3("surfaceColor", glm::vec3(r, g, b));
    s.setVec3("material.specular", glm::vec3(0.4f)); s.setVec3("material.emissive", glm::vec3(0));
    s.setFloat("material.shininess", 16.0f); s.setMat4("model", model);
    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, texture); s.setInt("material.diffuse", 0);
    glBindVertexArray(coneVAO); glDrawArrays(GL_TRIANGLES, 0, vertCount);
}

// ─────────────────────────────────────────────────────────────────────────────
//  stadium
// ─────────────────────────────────────────────────────────────────────────────
void stadium(unsigned int& cubeVAO, unsigned int coneVAO, int coneVertCount,
    Shader& lightingShader, Shader& lightingShaderWithTexture,
    glm::mat4 alTogether,
    unsigned int fieldTexture, unsigned int grassTexture,
    unsigned int footballTexture, unsigned int scoreTexture,
    int textureMode)
{
    int   numSegments = 200;
    float angleStep = 360.0f / numSegments;
    glm::mat4 I = glm::mat4(1.0f);
    glm::mat4 translateMatrix, rotateYMatrix, scaleMatrix, model;

    translateMatrix = glm::translate(I, glm::vec3(-2.5f, 0.1f, 0.0f));
    scaleMatrix = glm::scale(I, glm::vec3(5.0f, 0.2f, 17.0f));
    model = translateMatrix * scaleMatrix;
    drawCubeWithTexture(cubeVAO, lightingShaderWithTexture, model, fieldTexture);

    // White field markings
    auto whiteLine = [&](glm::vec3 pos, glm::vec3 sz) {
        model = glm::translate(I, pos) * glm::scale(I, sz);
        drawCube(cubeVAO, lightingShader, model, 1, 1, 1);
        };
    whiteLine({ -2.5f,0.31f,8.45f }, { 5.0f,0.01f,0.10f });
    whiteLine({ -2.55f,0.31f,0.0f }, { 0.1f,0.01f,17.0f });
    whiteLine({ 2.45f,0.31f,0.0f }, { 0.1f,0.01f,17.0f });
    whiteLine({ -2.5f,0.31f,-0.05f }, { 5.0f,0.01f,0.10f });
    whiteLine({ -2.5f,0.31f,16.95f }, { 5.0f,0.01f,0.10f });

    // Centre circle
    for (int c = 0; c < 36; c++) {
        float a1 = glm::radians(c * 10.0f), a2 = glm::radians((c + 1) * 10.0f);
        float mx = (1.5f * cos(a1) + 1.5f * cos(a2)) * 0.5f;
        float mz = (1.5f * sin(a1) + 1.5f * sin(a2)) * 0.5f + 8.5f;
        model = glm::translate(I, glm::vec3(mx - 0.05f, 0.315f, mz - 0.05f)) * glm::scale(I, glm::vec3(0.1f, 0.005f, 0.1f));
        drawCube(cubeVAO, lightingShader, model, 1, 1, 1);
    }

    // Ground base — stadium concrete platform (same footprint, slightly elevated)
    model = glm::translate(I, glm::vec3(-17, 0.09f, -7)) * glm::scale(I, glm::vec3(34, 0.2f, 30));
    drawCube(cubeVAO, lightingShader, model, 0.45f, 0.44f, 0.40f);   // warm concrete

    // Tiered seating
         for (int i = 0; i < numSegments; i++) {
        float angle = glm::radians(i * angleStep);
        float angleDeg = i * angleStep;
        bool isAisle = (fmod(angleDeg, 45.0f) < (angleStep * 2)) || (fmod(angleDeg, 45.0f) > (45.0f - angleStep * 2));
        bool isVIP = (angleDeg > 60 && angleDeg < 120) || (angleDeg > 240 && angleDeg < 300);
        if (!isAisle) {
            rotateYMatrix = glm::rotate(I, angle, glm::vec3(0, 1, 0));
            float x4 = 10.5f * cos(angle), z4 = 18.0f * sin(angle) + 8.5f;
            model = glm::translate(I, { x4,3.5f,z4 }) * rotateYMatrix * glm::scale(I, { 0.8f,0.35f,1.5f });
            if (textureMode == 0) drawCube(cubeVAO, lightingShader, model, 0.3f, 0.3f, 0.0f);
            else if (textureMode == 1) drawCubeWithTexture(cubeVAO, lightingShaderWithTexture, model, grassTexture);
            else drawCubeBlended(cubeVAO, lightingShaderWithTexture, model, grassTexture, 0.3f, 0.3f, 1.5f);
            float x3 = 9.0f * cos(angle), z3 = 16.0f * sin(angle) + 8.5f;
            model = glm::translate(I, { x3,2.8f,z3 }) * rotateYMatrix * glm::scale(I, { 0.75f,0.35f,1.4f });
            if (textureMode == 0) drawCube(cubeVAO, lightingShader, model, isVIP ? 0.5f : 0.7f, 0.1f, 0.3f);
            else if (textureMode == 1) drawCubeWithTexture(cubeVAO, lightingShaderWithTexture, model, grassTexture);
            else drawCubeBlended(cubeVAO, lightingShaderWithTexture, model, grassTexture, isVIP ? 1.2f : 1.8f, 0.1f, 0.3f);
            float x2 = 7.5f * cos(angle), z2 = 14.0f * sin(angle) + 8.5f;
            model = glm::translate(I, { x2,2.1f,z2 }) * rotateYMatrix * glm::scale(I, { 0.7f,0.35f,1.3f });
            if (textureMode == 0) drawCube(cubeVAO, lightingShader, model, 0.3f, 0.6f, 0.3f);
            else if (textureMode == 1) drawCubeWithTexture(cubeVAO, lightingShaderWithTexture, model, grassTexture);
            else drawCubeBlended(cubeVAO, lightingShaderWithTexture, model, grassTexture, 0.4f, 1.5f, 0.4f);
            float x1 = 6.0f * cos(angle), z1 = 12.0f * sin(angle) + 8.5f;
            model = glm::translate(I, { x1,1.4f,z1 }) * rotateYMatrix * glm::scale(I, { 0.65f,0.35f,1.2f });
            if (textureMode == 0) drawCube(cubeVAO, lightingShader, model, 0.9f, 0.9f, 0.3f);
            else if (textureMode == 1) drawCubeWithTexture(cubeVAO, lightingShaderWithTexture, model, grassTexture);
            else drawCubeBlended(cubeVAO, lightingShaderWithTexture, model, grassTexture, 2.0f, 2.0f, 0.3f);
        }
        else {
            float xa = 8.5f * cos(angle), za = 15.5f * sin(angle) + 8.5f;
            rotateYMatrix = glm::rotate(I, angle, glm::vec3(0, 1, 0));
            model = glm::translate(I, { xa,2.0f,za }) * rotateYMatrix * glm::scale(I, { 1.2f,0.15f,3.5f });
            drawCube(cubeVAO, lightingShader, model, 0.4f, 0.4f, 0.4f);
        }
    }

    // Entrance tunnels
    float entrAngles[] = { 0,90,180,270 };
    for (int e = 0; e < 4; e++) {
        float ea = glm::radians(entrAngles[e]);
        float ex = 11.5f * cos(ea), ez = 19.0f * sin(ea) + 8.5f;
        model = glm::translate(I, { ex,1.5f,ez }) * glm::rotate(I, ea, glm::vec3(0, 1, 0)) * glm::scale(I, { 2,3,1.5f });
        drawCube(cubeVAO, lightingShader, model, 0.2f, 0.2f, 0.25f);
    }

    // Support pillars
    for (int p = 0; p < 4; p++) {
        float pa = glm::radians(p * 90.0f);
        model = glm::translate(I, { 11 * cos(pa),0,18.5f * sin(pa) + 8.5f }) * glm::scale(I, { 0.8f,5,0.8f });
        drawCube(cubeVAO, lightingShader, model, 0.35f, 0.35f, 0.35f);
    }

    // Corner flags
    struct CF { float x, z, r, g, b; };
    CF corners[] = { {-2.5f,0,1.2f,0.2f,0.2f},{2.5f,0,0.2f,0.2f,1.5f},{-2.5f,17,1.5f,1.5f,0.2f},{2.5f,17,0.2f,1.5f,0.2f} };
    for (auto& cf : corners) {
        model = glm::translate(I, glm::vec3(cf.x - 0.025f, 0.3f, cf.z - 0.025f)) * glm::scale(I, glm::vec3(0.05f, 0.8f, 0.05f));
        // Soft beige flag poles (#E8DCCB) — warmer than plain off-white
        drawCube(cubeVAO, lightingShader, model, 0.910f, 0.863f, 0.796f);
        model = glm::translate(I, glm::vec3(cf.x, 1.1f, cf.z)) * glm::scale(I, glm::vec3(0.25f, 0.35f, 0.25f));
        if (textureMode == 0) drawCone(coneVAO, coneVertCount, lightingShader, model, cf.r, cf.g, cf.b);
        else if (textureMode == 1) drawConeWithTexture(coneVAO, coneVertCount, lightingShaderWithTexture, model, grassTexture);
        else drawConeBlended(coneVAO, coneVertCount, lightingShaderWithTexture, model, grassTexture, cf.r, cf.g, cf.b);
    }
    // ── Fractal Trees with Pots — visible beside near goal, outside sidelines ──
    //struct FT { float x, z; };
    //FT ftCorners[] = {
    //    { -3.2f,  3.5f },   // left side, clearly visible from camera
    //    {  3.2f,  3.5f },   // right side
    //    { -3.2f, 13.5f },   // left side far
    //    {  3.2f, 13.5f }    // right side far
    //};
    //for (auto& ft : ftCorners) {
    //    // ── Pot: terracotta orange-brown, taller pot ──────────────────────
    //    glm::mat4 potModel = glm::translate(I, glm::vec3(ft.x - 0.10f, 0.3f, ft.z - 0.10f))
    //        * glm::scale(I, glm::vec3(0.20f, 0.18f, 0.20f));
    //    drawCube(cubeVAO, lightingShader, potModel, 0.78f, 0.35f, 0.15f);

    //    // ── Pot rim: slightly wider dark band on top ──────────────────────
    //    glm::mat4 rimModel = glm::translate(I, glm::vec3(ft.x - 0.115f, 0.47f, ft.z - 0.115f))
    //        * glm::scale(I, glm::vec3(0.23f, 0.03f, 0.23f));
    //    drawCube(cubeVAO, lightingShader, rimModel, 0.55f, 0.22f, 0.08f);

    //    // ── Tree on top of pot — taller and fuller ────────────────────────
    //    glm::mat4 treeBase = glm::translate(I, glm::vec3(ft.x, 0.50f, ft.z));
    //    drawFractalTree(cubeVAO, lightingShader, treeBase, 8, 0.28f);
    //}
    // Scoreboard frame
    model = glm::translate(I, glm::vec3(-3.0f, 5.8f, 25.0f)) * glm::scale(I, glm::vec3(6.0f, 3.0f, 0.3f));
    drawCube(cubeVAO, lightingShader, model, 0.08f, 0.08f, 0.12f);

    // Scoreboard screen (texture — correct orientation)
    model = glm::translate(I, glm::vec3(2.9f, 5.9f, 24.75f)) * glm::scale(I, glm::vec3(-5.8f, 2.8f, 0.05f));
    drawCubeWithTexture(cubeVAO, lightingShaderWithTexture, model, scoreTexture);
    // ── Stadium Roof (Catmull-Rom Spline Surface) ─────────────────────────────
    drawStadiumRoof(lightingShader, glm::mat4(1.0f),
        168.0f / 255.0f, 176.0f / 255.0f, 184.0f / 255.0f);


}

// ─────────────────────────────────────────────────────────────────────────────
//  drawGoalPost
// ─────────────────────────────────────────────────────────────────────────────
void drawGoalPost(unsigned int& cubeVAO, Shader& lightingShader,
    glm::mat4 parentModel, glm::vec3 position)
{
    float rodThick = 0.05f, goalW = 2.0f, goalH = 1.0f;
    // Soft beige goal posts (#E8DCCB) — warmer and more natural than pure white
    const float beigeR = 0.910f, beigeG = 0.863f, beigeB = 0.796f;
    glm::mat4 m;
    m = glm::translate(parentModel, { -goalW / 2 + position.x,position.y,position.z }) * glm::scale(glm::mat4(1), { rodThick,goalH,rodThick });
    drawCube(cubeVAO, lightingShader, m, beigeR, beigeG, beigeB);
    m = glm::translate(parentModel, { goalW / 2 + position.x,position.y,position.z }) * glm::scale(glm::mat4(1), { rodThick,goalH,rodThick });
    drawCube(cubeVAO, lightingShader, m, beigeR, beigeG, beigeB);
    m = glm::translate(parentModel, { position.x - 0.95f,goalH + position.y,position.z }) * glm::scale(glm::mat4(1), { goalW,rodThick,rodThick });
    drawCube(cubeVAO, lightingShader, m, beigeR, beigeG, beigeB);
}

// ─────────────────────────────────────────────────────────────────────────────
//  drawFloodlight
// ─────────────────────────────────────────────────────────────────────────────
void drawFloodlight(unsigned int& cubeVAO, Shader& lightingShader, glm::vec3 pos, bool emissiveOn)
{
    glm::mat4 I = glm::mat4(1);
    glm::mat4 m = glm::translate(I, { pos.x - 0.2f,0,pos.z - 0.2f }) * glm::scale(I, { 0.4f,pos.y,0.4f });
    drawCube(cubeVAO, lightingShader, m, 0.3f, 0.3f, 0.3f);
    m = glm::translate(I, { pos.x - 1,pos.y - 1,pos.z - 1 }) * glm::scale(I, { 2,1.5f,2 });
    glm::vec3 em = emissiveOn ? glm::vec3(0.5f, 0.5f, 0.3f) : glm::vec3(0);
    drawCube(cubeVAO, lightingShader, m, 0.6f, 0.6f, 0.6f, em);
}
// ─────────────────────────────────────────────────────────────────────────────
//  drawFractalTree — Recursive 3D Fractal Tree (brown branches, green leaves)
//  Fractal self-similarity: each branch splits into 2 sub-branches that
//  twist around the Y-axis at each level, creating a full 3D canopy.
// ─────────────────────────────────────────────────────────────────────────────
void drawFractalTree(unsigned int& cubeVAO, Shader& lightingShader,
    glm::mat4 parentModel, int depth, float branchLen)
{
    if (depth < 0) return;

    glm::mat4 I = glm::mat4(1.0f);

    // ── Color: brown gradient for branches, varied greens for leaves ─────────
    float r, g, b;
    if (depth == 0) {
        // Leaf clusters — varied greens for natural look
        r = 0.05f; g = 0.70f; b = 0.10f;
    }
    else if (depth == 1) {
        // Twig tips with slight green tint
        r = 0.30f; g = 0.38f; b = 0.08f;
    }
    else if (depth <= 3) {
        // Young thin branches — medium brown
        r = 0.45f; g = 0.25f; b = 0.08f;
    }
    else if (depth <= 6) {
        // Mid branches — warm brown
        r = 0.38f; g = 0.20f; b = 0.06f;
    }
    else {
        // Trunk — deep dark brown
        r = 0.28f; g = 0.14f; b = 0.04f;
    }

    // ── Branch thickness: proportional to depth for realistic taper ──────────
    float thick;
    if (depth == 0) {
        // Leaves are drawn as wider flat cubes for clear visibility
        thick = 0.045f;
    } else {
        // Trunk thickest, tapers at each level
        thick = 0.008f + 0.022f * (depth / 9.0f);
    }

    // ── Draw this branch / leaf ──────────────────────────────────────────────
    if (depth == 0) {
        // Leaf: wider, flatter cube for visible foliage
        glm::mat4 leafModel = parentModel
            * glm::scale(I, glm::vec3(0.06f, branchLen * 0.6f, 0.05f));
        drawCube(cubeVAO, lightingShader, leafModel, r, g, b);
        return;   // leaves don't split
    }

    glm::mat4 branchModel = parentModel
        * glm::scale(I, glm::vec3(thick, branchLen, thick));
    drawCube(cubeVAO, lightingShader, branchModel, r, g, b);

    // ── Move to tip of this branch ──────────────────────────────────────────
    glm::mat4 tip = parentModel
        * glm::translate(I, glm::vec3(0.0f, branchLen, 0.0f));

    float childLen = branchLen * 0.72f;   // slightly longer reach
    float angleDeg = 35.0f;               // wider spread for fuller canopy

    // Y-axis twist: rotate branching plane by 90° at each depth level
    // so the tree spreads in all directions, not just one flat plane
    float twistDeg = 90.0f * (float)(depth % 4);
    glm::mat4 twist = glm::rotate(I, glm::radians(twistDeg), glm::vec3(0, 1, 0));

    // Right branch (spread in current twist plane)
    glm::mat4 rightBranch = tip * twist
        * glm::rotate(I, glm::radians(-angleDeg), glm::vec3(0, 0, 1));
    drawFractalTree(cubeVAO, lightingShader, rightBranch, depth - 1, childLen);

    // Left branch (spread in current twist plane)
    glm::mat4 leftBranch = tip * twist
        * glm::rotate(I, glm::radians(+angleDeg), glm::vec3(0, 0, 1));
    drawFractalTree(cubeVAO, lightingShader, leftBranch, depth - 1, childLen);
}
// ─────────────────────────────────────────────────────────────────────────────
//  drawCloud — fluffy cloud from overlapping white spheres
// ─────────────────────────────────────────────────────────────────────────────
void drawCloud(unsigned int& cubeVAO, Shader& lightingShader, Sphere& sphere,
    glm::vec3 center, float scale)
{
    lightingShader.use();
    // Soft white material with very slight warm tint
    lightingShader.setVec3("material.ambient",  glm::vec3(0.95f, 0.95f, 0.97f));
    lightingShader.setVec3("material.diffuse",  glm::vec3(0.98f, 0.98f, 1.00f));
    lightingShader.setVec3("material.specular", glm::vec3(0.1f));
    lightingShader.setVec3("material.emissive", glm::vec3(0.15f, 0.15f, 0.18f));
    lightingShader.setFloat("material.shininess", 4.0f);

    // Cloud puff positions (relative offsets) — organic lumpy shape
    struct Puff { float dx, dy, dz, r; };
    Puff puffs[] = {
        {  0.0f,  0.0f,  0.0f, 1.0f  },   // center
        { -1.1f,  0.15f, 0.1f, 0.85f },   // left
        {  1.2f,  0.1f, -0.1f, 0.90f },   // right
        {  0.5f,  0.35f, 0.15f, 0.75f },  // top-right
        { -0.5f,  0.30f,-0.1f, 0.70f },   // top-left
        { -1.8f, -0.05f, 0.0f, 0.60f },   // far left
        {  1.9f,  0.0f,  0.05f,0.55f },   // far right
        {  0.0f,  0.0f,  0.4f, 0.65f },   // front bulge
        {  0.0f,  0.0f, -0.35f,0.60f },   // back bulge
    };

    glm::mat4 I = glm::mat4(1.0f);
    for (auto& p : puffs) {
        glm::mat4 m = glm::translate(I, center + glm::vec3(p.dx * scale, p.dy * scale, p.dz * scale))
            * glm::scale(I, glm::vec3(p.r * scale * 0.55f, p.r * scale * 0.35f, p.r * scale * 0.45f));
        lightingShader.setMat4("model", m);
        glBindVertexArray(sphere.sphereVAO);
        glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, (void*)0);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  drawEnvironment — green terrain, landscape trees, floating clouds
// ─────────────────────────────────────────────────────────────────────────────
void drawEnvironment(unsigned int& cubeVAO, Shader& lightingShader,
    Shader& lightingShaderWithTexture, Sphere& sphere,
    unsigned int grassTexture, int textureMode, float cloudDrift)
{
    glm::mat4 I = glm::mat4(1.0f);
    glm::mat4 model;

    // ═══════════════════════════════════════════════════════════════════════
    //  LARGE GREEN TERRAIN — extends far beyond the stadium in all directions
    // ═══════════════════════════════════════════════════════════════════════
    // Main ground plane: huge green grass field
    model = glm::translate(I, glm::vec3(-50.0f, -0.05f, -40.0f))
        * glm::scale(I, glm::vec3(100.0f, 0.15f, 100.0f));
    drawCubeWithTexture(cubeVAO, lightingShaderWithTexture, model, backgroundTexture);

    // Slight earthy edge / dirt border (slightly below, slightly darker)
    model = glm::translate(I, glm::vec3(-55.0f, -0.20f, -45.0f))
        * glm::scale(I, glm::vec3(110.0f, 0.15f, 110.0f));
    drawCube(cubeVAO, lightingShader, model, 0.35f, 0.30f, 0.18f);

    // ═══════════════════════════════════════════════════════════════════════
    //  LANDSCAPE TREES — scattered around the exterior of the stadium
    //  Using the existing drawFractalTree with varying sizes
    // ═══════════════════════════════════════════════════════════════════════
    struct LTree { float x, z, height; int depth; };
    LTree landscapeTrees[] = {
     { -8.0f,  32.0f, 1.20f, 5 },
     {  5.0f,  35.0f, 1.00f, 5 },
     { 12.0f,  30.0f, 1.35f, 5 },
     { -6.0f, -12.0f, 1.10f, 5 },
     {  4.0f, -15.0f, 1.25f, 5 },
     { 10.0f, -10.0f, 0.90f, 5 },
     { -22.0f,  2.0f, 1.30f, 5 },
     { -25.0f, 10.0f, 1.00f, 5 },
     { -20.0f, 18.0f, 1.20f, 5 },
     { -28.0f, 14.0f, 0.85f, 4 },
     {  22.0f,  4.0f, 1.15f, 5 },
     {  25.0f, 12.0f, 1.30f, 5 },
     {  20.0f, 20.0f, 1.05f, 5 },
     {  28.0f,  8.0f, 0.80f, 4 },
     { -18.0f, 28.0f, 1.00f, 5 },
     {  18.0f,-12.0f, 1.10f, 5 },
    };

    for (auto& lt : landscapeTrees) {
        // Visible trunk base — taller and thicker, proportional to tree
        float trunkH = lt.height * 0.35f;
        float trunkW = 0.06f + lt.height * 0.04f;
        glm::mat4 trunkBase = glm::translate(I, glm::vec3(lt.x - trunkW * 0.5f, 0.05f, lt.z - trunkW * 0.5f))
            * glm::scale(I, glm::vec3(trunkW, trunkH, trunkW));
        drawCube(cubeVAO, lightingShader, trunkBase, 0.30f, 0.16f, 0.05f);

        // Fractal tree canopy — starts above trunk
        glm::mat4 treeBase = glm::translate(I, glm::vec3(lt.x, 0.05f + trunkH * 0.6f, lt.z));
        drawFractalTree(cubeVAO, lightingShader, treeBase, lt.depth, lt.height);
    }

    // ═══════════════════════════════════════════════════════════════════════
    //  FLOATING CLOUDS — 3 clouds drifting slowly across the sky
    // ═══════════════════════════════════════════════════════════════════════
    // Cloud positions cycle with cloudDrift (wraps around)
    float driftRange = 60.0f;   // total travel distance before wrap
    float wrap = fmodf(cloudDrift, 1.0f) * driftRange;   // 0 → driftRange

    struct CloudDef { float baseX, y, z, scale, speed; };
    CloudDef clouds[] = {
        { -12.0f, 16.0f, 15.0f, 2.2f, 1.0f },   // cloud 1 — large, left side
        {   6.0f, 18.0f,  5.0f, 1.8f, 0.7f },   // cloud 2 — medium, right front
        {  -2.0f, 14.5f, 28.0f, 2.5f, 1.3f },   // cloud 3 — large, far back
    };

    for (auto& cd : clouds) {
        float cx = cd.baseX + fmodf(wrap * cd.speed, driftRange) - driftRange * 0.5f;
        drawCloud(cubeVAO, lightingShader, sphere,
            glm::vec3(cx, cd.y, cd.z), cd.scale);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  processInput
// ─────────────────────────────────────────────────────────────────────────────
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    float camSpeed = 3.5f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime * camSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime * camSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime * camSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime * camSpeed);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.ProcessKeyboard(UP, deltaTime * camSpeed);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) camera.ProcessKeyboard(DOWN, deltaTime * camSpeed);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) camera.ProcessKeyboard(P_UP, deltaTime * camSpeed);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) camera.ProcessKeyboard(P_DOWN, deltaTime * camSpeed);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) camera.ProcessKeyboard(Y_LEFT, deltaTime * camSpeed);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) camera.ProcessKeyboard(Y_RIGHT, deltaTime * camSpeed);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camera.ProcessKeyboard(R_LEFT, deltaTime * camSpeed);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camera.ProcessKeyboard(R_RIGHT, deltaTime * camSpeed);
    // ── GAME CONTROLS ─────────────────────────────────────────────────────────
    // J / L  — rotate aim arrow left / right (only while ball is not in flight)
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && !ball_shoot)
        angle_ball += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && !ball_shoot)
        angle_ball -= 1.0f;

    // G — shoot (only when not already in flight and not game-over)
    static bool gPressed = false;
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && !gPressed) {
        if (!gameOver && kickState == IDLE && kickState != RUN_TO_BALL) {
            kickState = RUN_TO_BALL;
            kickTimer = 0.0f;
            personRunZ = 0.0f;
        }
        gPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE) gPressed = false;

    // B — restart after game-over
    static bool bPressed = false;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !bPressed) {
        start = true;
        gameOver = false;
        score = 0;
        ball_x = 0.0f;
        bz = 0.0f;
        ball_shoot = false;
        angle_ball = 0.0f;
        ball_count = 0;
        serial_num = 0;
        difficult = 0;
        bar_x = 0.0f;
        bar_inc = 1.0f;
        // ── Reset person animation ──
        kickState = IDLE;
        kickTimer = 0.0f;
        personRunZ = 0.0f;
        legAngle = 0.0f;
        runCycle = 0.0f;
        runLegAngle = 0.0f;
        runArmAngle = 0.0f;
        cout << "[RESTART]\n";
        bPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE) bPressed = false;

    // I / K / P — difficulty 1 / 2 / 3  (K instead of O to avoid O=texture-blend conflict)
    static bool iP = false, kP = false, pP = false;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && !iP) { difficult = 1; cout << "Difficulty 1 (slow)\n";   iP = true; }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE) iP = false;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && !kP) { difficult = 2; cout << "Difficulty 2 (medium)\n"; kP = true; }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE) kP = false;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !pP) { difficult = 3; cout << "Difficulty 3 (fast)\n";   pP = true; }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) pP = false;

    // ── LIGHT & TEXTURE TOGGLES (single-press guards) ────────────────────────
    static bool k1 = false, k2 = false, k3 = false, k4 = false;
    static bool k5 = false, k6 = false, k7 = false, k9 = false;
    static bool kM = false, kN = false, kO = false;

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !k1) {
        directionLighton = !directionLighton;
        if (directionLighton) directlight.turnOn(); else directlight.turnOff(); k1 = true;
    } if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE) k1 = false;

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !k2) {
        pointLightOn = !pointLightOn;
        if (pointLightOn) { pointlight1.turnOn(); pointlight2.turnOn(); pointlight3.turnOn(); pointlight4.turnOn(); }
        else { pointlight1.turnOff(); pointlight2.turnOff(); pointlight3.turnOff(); pointlight4.turnOff(); }
        k2 = true;
    } if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE) k2 = false;

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && !k3) {
        spotlighton = !spotlighton;
        if (spotlighton) spotlight.turnOn(); else spotlight.turnOff(); k3 = true;
    } if (glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE) k3 = false;

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && !k4) { emissiveOn = !emissiveOn; k4 = true; }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_RELEASE) k4 = false;

    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && !k5) {
        ambientToggle = !ambientToggle;
        if (ambientToggle) { pointlight1.turnAmbientOn(); pointlight2.turnAmbientOn(); pointlight3.turnAmbientOn(); pointlight4.turnAmbientOn(); directlight.turnAmbientOn(); spotlight.turnAmbientOn(); }
        else { pointlight1.turnAmbientOff(); pointlight2.turnAmbientOff(); pointlight3.turnAmbientOff(); pointlight4.turnAmbientOff(); directlight.turnAmbientOff(); spotlight.turnAmbientOff(); }
        k5 = true;
    } if (glfwGetKey(window, GLFW_KEY_5) == GLFW_RELEASE) k5 = false;

    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && !k6) {
        diffuseToggle = !diffuseToggle;
        if (diffuseToggle) { pointlight1.turnDiffuseOn(); pointlight2.turnDiffuseOn(); pointlight3.turnDiffuseOn(); pointlight4.turnDiffuseOn(); directlight.turnDiffuseOn(); spotlight.turnDiffuseOn(); }
        else { pointlight1.turnDiffuseOff(); pointlight2.turnDiffuseOff(); pointlight3.turnDiffuseOff(); pointlight4.turnDiffuseOff(); directlight.turnDiffuseOff(); spotlight.turnDiffuseOff(); }
        k6 = true;
    } if (glfwGetKey(window, GLFW_KEY_6) == GLFW_RELEASE) k6 = false;

    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS && !k7) {
        specularToggle = !specularToggle;
        if (specularToggle) { pointlight1.turnSpecularOn(); pointlight2.turnSpecularOn(); pointlight3.turnSpecularOn(); pointlight4.turnSpecularOn(); directlight.turnSpecularOn(); spotlight.turnSpecularOn(); }
        else { pointlight1.turnSpecularOff(); pointlight2.turnSpecularOff(); pointlight3.turnSpecularOff(); pointlight4.turnSpecularOff(); directlight.turnSpecularOff(); spotlight.turnSpecularOff(); }
        k7 = true;
    } if (glfwGetKey(window, GLFW_KEY_7) == GLFW_RELEASE) k7 = false;

    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS && !k9) { activeViewport = (activeViewport + 1) % 5; k9 = true; }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_RELEASE) k9 = false;

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && !kM) { textureMode = 0; cout << "Mode M: Material\n"; kM = true; }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE) kM = false;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && !kN) { textureMode = 1; cout << "Mode N: Texture\n";  kN = true; }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_RELEASE) kN = false;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && !kO) { textureMode = 2; cout << "Mode O: Blended\n"; kO = true; }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_RELEASE) kO = false;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    framebufferWidth = width; framebufferHeight = height;
    glViewport(0, 0, width, height);
}