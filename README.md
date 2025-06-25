# Physics_Sim – Minimal OpenGL Rotating-Cube Demo 

A bite-sized C++/OpenGL project that renders a **spinning, pixel-shaded cube**.  
Great as a template for learning modern OpenGL (3.3 Core) with **GLFW**, **GLAD**, and **GLM**.

## ✨ Features

| What | Why it matters |
|------|----------------|
| **Modern shader pipeline** | Vertex + fragment shaders compiled at runtime (330 core). |
| **Pixel-art fragment shader** | Demonstrates UV manipulation & color quantization (32-level vertical gradient). |
| **Depth testing + indexed VBO** | Correct 3-D rendering with minimal draw calls (single `glDrawElements`). |
| **Time-based rotation** | Uses GLFW’s clock to animate model matrix via GLM. |
| **GPU selection hint** | `NvOptimusEnablement` export nudges laptops to run on the integrated GPU. |

---

## 🛠️ Requirements

| Dependency | Tested version | Notes |
|------------|---------------|-------|
| **C++17** compiler | GCC 13 / clang 17 / MSVC 19 (v143) | |
| **GLFW** | 3.3+ | Window/context + input |
| **GLAD** | OpenGL 3.3 core loader | Generate → add sources |
| **GLM** | 0.9.9+ | Header-only math |
| (Windows) **OpenGL drivers** | Recent | Laptop dGPU/iGPU both work |

---
## Building

```bash
git clone https://github.com/<you>/Physics_Sim.git
cd Physics_Sim
mkdir build && cd build
cmake ..
cmake --build . --config Release
./Physics_Sim            # or Physics_Sim.exe on Windows
