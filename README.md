# Rose2-Engine

![C++](https://img.shields.io/badge/C++20-blue.svg)
![License](https://img.shields.io/badge/license-MIT-lightgrey.svg)

**Rose2-Engine** is a custom, lightweight 2D game engine built from scratch in C++ and OpenGL. Designed as a hobby project to explore modern game engine architecture, low-level graphics programming, and tooling design, it provides a robust foundation for building 2D games.

It pairs seamlessly with the **[Rose2-Editor](https://github.com/your-username/Rose2-Editor)**, a companion desktop GUI application built for visual scene composition, asset management, and project configuration.

---

## Features

* **:Custom Sprite Renderer:** High-performance 2D rendering pipeline powered by OpenGL, supporting batch rendering, textures, and transformations.
* **:Robust Asset System:** Efficient asset management, loading, and caching for textures, shaders, and audio.
* **:Entity Component System (ECS):** Data-oriented architecture for scalable game logic and performance.
* **:Scene & Project Serialization:** Save and load complete project states and game scenes cleanly using structured formats (JSON).
* **:Lua Scripting Engine:** Embedded Lua scripting support enabling flexible, hot-reloadable gameplay logic without recompiling the core engine.
* **:Editor Integration:** Fully compatible with `Rose2-Editor` for real-time scene editing, property inspection, and hierarchy management.

---


## Getting Started

### Prerequisites
Make sure you have the following installed on your system:
* A modern C++ compiler supporting C++20 (MSVC, GCC, or Clang)
* CMake (version 3.20 or higher)
* Git

### Cloning the Repository
Clone the engine along with its submodules:
```bash
git clone --recursive [https://github.com/your-username/Rose2-Engine.git](https://github.com/your-username/Rose2-Engine.git)
cd Rose2-Engine
