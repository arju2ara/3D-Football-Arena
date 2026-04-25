# ⚽ 3D Football  Arena (OpenGL)

A fully interactive **3D Football Stadium Simulation** built using OpenGL 3.3.
This project combines real-time rendering, animation, and gameplay mechanics into a single immersive experience.

---

## 🎯 Project Overview

This project recreates a realistic football stadium environment with:

* 🏟️ Multi-tier stadium architecture
* 🌤️ Animated clouds and fractal trees
* 💡 Advanced lighting system (Phong Model)
* 🎮 Interactive football mini-game
* 🎥 Free camera & multi-viewport system

The user can aim, shoot, and score goals against a moving target inside the stadium.

> Developed as part of **CSE 4208: Computer Graphics Laboratory** at KUET.

---

## 🚀 Features

### 🧱 3D Scene & Objects

* Stadium bowl with multiple colored tiers
* Catmull-Rom spline based stadium roof
* Goal post & net (Bezier curve based)
* Fractal trees and animated clouds
* Scoreboard and floodlights

### 🎮 Interactive Gameplay

* 🧭 **Aim System**
  Rotate the shooting direction using keyboard controls

* ⚽ **Ball Shooting Mechanism**
  Press `G` to kick the ball
  → Ball follows a realistic **Bezier curve trajectory**

* 🎯 **Scoring System**

  * If the ball hits the moving target → ✅ Goal
  * Score increases and is displayed on scoreboard

* 🕺 **Player Animation System**

  * 👟 Running animation (idle state)
  * 🦵 Backswing before kick
  * ⚽ Kick animation (ball launch moment)
  * 🎉 **Celebration animation after scoring a goal**

* ❌ **Game Over Condition**

  * If the player misses → Game Over
  * Score resets and game restarts

* ⚡ **Difficulty Levels**

  * `I` → Slow
  * `K` → Medium
  * `P` → Fast

---


### 💡 Lighting System (Phong Model)

* Directional Light (Sunlight)
* Point Lights (Floodlights)
* Spot Light (Ball tracking)
* Emissive lighting
* Ambient, Diffuse, Specular toggle

### 🎨 Rendering Modes

* Material Mode (Phong only)
* Texture Mode
* Blended Mode (Texture + Color)

### 🎥 Camera System

* Free camera movement (W/A/S/D + Arrow keys)
* 4-Viewport split screen mode

---

## 🖼️ Screenshots

### 🏟️ Stadium View
![Stadium](four_view_port.jpeg)

### 🎮 Side View
![Side](side view.jpeg)

### 🔝 Top View
![Top](top view.jpeg)

### 👁️ View
![View](view.jpeg)

---

<h2>🎥 Demo Video</h2>
<p align="center">
  <a href="https://youtu.be/3h_pJdf7yw0">
    <img src="https://img.youtube.com/vi/3h_pJdf7yw0/0.jpg" width="600">
  </a>
</p>

---

## 🕹️ Controls

| Key        | Action                |
| ---------- | --------------------- |
| W/A/S/D    | Move camera           |
| Arrow Keys | Rotate camera         |
| V          | Toggle multi-viewport |
| M/N/O      | Rendering modes       |
| 1-7        | Lighting controls     |
| J / L      | Aim left/right        |
| G          | Shoot                 |
| I / K / P  | Difficulty levels     |
| ESC        | Exit                  |

---

## 🛠️ Technologies Used

* OpenGL 3.3 Core Profile
* GLFW
* GLAD
* GLM
* stb_image
* C++

---

## 🧠 Concepts Implemented

* OpenGL Rendering Pipeline
* Phong Lighting Model
* Bezier Curves (Ball trajectory & Net)
* Catmull-Rom Spline (Roof design)
* Fractal Generation (Trees)
* Texture Mapping
* Hierarchical Animation

---

## 📂 Project Structure

```
├── main.cpp
├── scene/
├── shaders/
├── textures/
├── utils/
├── assets/
└── README.md
```

---

## ⚡ Performance

* Runs at ~60 FPS (Full HD)
* Optimized using reusable geometry (VAO instancing)

---

## 📘 Report & Presentation

📄 Report: See attached PDF
📊 Slides: See presentation file

---

## 🔮 Future Improvements

* Shadow Mapping
* Physically Based Rendering (PBR)
* Collision Detection
* Multiplayer support
* Procedural crowd system

---

## 👩‍💻 Author

**Arju Ara Afrin**
CSE, Khulna University of Engineering & Technology

---

## ⭐ If you like this project

Give it a ⭐ on GitHub and share your feedback!
