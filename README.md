# 🧪 Sai Nanami — Block Descent

> _"Inspired Project which i made with passion for fun and dedicated to Sai Nanami, a talented programmer from the Sci-fi Manga & Anime Dr.Stone"_

A **Dr. Stone** themed falling block game featuring **Sai Nanami**, built with **C++17** and **SDL2**.

![C++](https://img.shields.io/badge/C%2B%2B-17-blue?style=flat-square&logo=cplusplus)
![SDL2](https://img.shields.io/badge/SDL2-Powered-green?style=flat-square)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey?style=flat-square&logo=windows)
![License](https://img.shields.io/badge/License-MIT-yellow?style=flat-square)

---

## 📸 Screenshots

![Gameplay](screenshots/gameplay.png)
![GameOver](screenshots/gameover.png)

---

## ✨ Features

| Feature          | Description                                           |
| ---------------- | ----------------------------------------------------- |
| 🎮 Gameplay      | Falling block mechanics with wall kicks & ghost piece |
| 🧑 Pixel Sprite  | Hand-crafted Sai Nanami pixel art on the side panel   |
| 💬 Quote Bubbles | Animated floating quotes pop from Sai's head          |
| 🌟 Starfield     | Falling star background animation                     |
| 🌈 Neon UI       | Full arcade-style glow on all panels                  |
| 💥 Particles     | Explosion effects on every line clear                 |
| 💾 High Score    | Persistent best score saved to disk                   |
| 🧩 Named Pieces  | Pieces named after CS concepts                        |

---

## 🎮 Controls

| Key     | Action                  |
| ------- | ----------------------- |
| `← →`   | Move left / right       |
| `↑`     | Rotate (with wall kick) |
| `↓`     | Soft drop               |
| `Space` | Hard drop               |
| `R`     | Restart                 |
| `Q`     | Quit                    |

---

## 🧩 Piece Reference

| Shape | Name      | Color  |
| ----- | --------- | ------ |
| I     | ALGORITHM | Cyan   |
| O     | BINARY    | Yellow |
| T     | FUNCTION  | Purple |
| S     | ARRAY     | Blue   |
| Z     | POINTER   | Red    |
| L     | LOOP      | Green  |
| J     | STACK     | Orange |

---

## 🛠️ Build & Run

### Requirements

- Windows 10/11
- [MSYS2](https://msys2.org) with MinGW-w64
- SDL2 + SDL2_ttf

### 1. Install dependencies

Open **MSYS2 MINGW64** and run:

```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_ttf make
```

### 2. Build

```bash
make
```

### 3. Run

```bash
./sai_blocks.exe
```

---

## 📁 Project Structure

sai-block-descent/
├── src/
│ ├── main.cpp # Entry point & game loop
│ ├── game.cpp/h # Game logic & collision
│ ├── renderer.cpp/h # SDL2 rendering & neon UI
│ ├── particles.cpp/h # Particles & starfield
│ ├── input.cpp/h # Keyboard input
│ └── sprite.h # Pixel art Sai Nanami
├── screenshots/
│ ├── gameplay.png
│ └── gameover.png
├── Makefile
└── README.md

---

## 👤 Author

Tahsin Akter Roza

---

_Built with C++17 · SDL2 · Inspired by Dr. Stone_

[def]: https://github.com/roza70
