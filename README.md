#  BreakOUT Clone (C++ & olcPixelGameEngine)

A clone of the classic **Breakout arcade game**, built in **C++** with [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine).  
This project was made to learn the basics of 2D game development such as **rendering, collision detection, game states, and particle effects**.  

---

##  Features Explained

### . Multiple Levels
The game isn’t just a single screen — it comes with **4 unique levels**, each with different block layouts and increasing difficulty.  
- Level 1: Wider paddle and slower ball (easier).  
- Level 2–4: Faster ball and smaller paddle, with more challenging block patterns.  
- Beat Level 4 to unlock the **Win Screen**.  

---

### . Score System
Every time you destroy a block, you earn **10 points**.  
Your **total score** is always displayed at the top of the screen and is shown again on **Game Over** or **Win** screens.  

---

### . Lives System
You start with **3 tries (lives)**.  
Each time the ball falls below the paddle, you lose one.  
Remaining lives are displayed as **heart icons** at the top left of the screen.  

---

### . Paddle Mechanics
The paddle doesn’t just bounce the ball straight up.  
- Where the ball hits the paddle **changes the reflection angle**.  
- This lets you control the direction of the ball, adding a **skill-based element**.  

---

### . Particle Effects
When a block is destroyed, it **explodes into colored fragments** that fade over time.  
- Red fragments for red blocks  
- Green fragments for green blocks  
- Yellow fragments for yellow blocks  

This adds some visual polish and makes breaking blocks more satisfying.  

---

### . Game States
The game is structured with clear **states**:
- **Menu** → Start screen (press SPACE to begin).  
- **Playing** → Main gameplay loop.  
- **Game Over** → Shows score + option to restart.  
- **Win** → When all levels are completed, with restart option.  

---

## . Tech Stack
- **Language**: C++17  
- **Engine**: [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine)  
- **Graphics**: Simple PNG assets (tiles, heart icons, particle sprite)  

---

## 🚀 Running the Game
1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/breakout-clone.git
   cd breakout-clone
