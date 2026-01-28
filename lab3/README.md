# 🧪 Escape Room (Three.js)

A first-person 3D escape room game built for the web using **Three.js** and **TypeScript**. The game runs directly in the browser and requires no additional installation.

The player is placed inside a closed room and must explore the environment, interact with objects, find a recipe, collect ingredients, brew a potion, and finally escape the room by exploiting a deliberate limitation in the collision system.

The project demonstrates a modular game architecture, real-time 3D rendering, object interaction, and a custom collision detection system.

---

## 🎮 Gameplay Overview

The game follows a simple but structured progression:

1. The player starts inside a locked room.
2. Initially, only exploration is possible.
3. After finding the recipe, a task system is activated.
4. The player must:

   * Collect all required ingredients
   * Use the cooking pot to create a potion
   * Drink the potion to gain extremely high movement speed
5. With the increased speed, the player can exploit a collision detection limitation and pass through a wall.
6. Leaving the room ends the game and displays the victory screen.

---

## 🛠️ Technologies Used

* **Three.js** — WebGL-based 3D rendering engine
* **TypeScript** — Main programming language
* **Blender** — Creation of the full 3D scene and models
* **HTML + CSS** — User interface (prompts, recipe UI, end screen)
* **GLB / GLTF** — 3D asset format
* **Node.js** — Development environment and build tooling

---

## 📁 Project Structure

```
src/
├── core/         (Game loop, GameState, Renderer, Input)
├── player/       (Player, Hands)
├── world/        (Room, Door, PickableItem, ...)
├── interaction/  (InteractionSystem, Interactable, ...)
├── utils/        (Collision)
└── ui/           (UI logic)
```

---

## 🧱 System Architecture

### Scene Loading

* The entire room is created in **Blender** and exported as a **GLB** file.
* After loading with `GLTFLoader`, the scene is traversed and objects are categorized based on their names:

  * Collision objects
  * Interactable objects
  * Pickable items (`*_pickable`)

This allows game logic to be defined directly inside the 3D editor.

---

### Player System

The player consists of:

* First-person camera
* Pointer Lock mouse controls
* Raycaster for interaction detection
* Movement system in the XZ plane
* Head-bobbing effect for visual realism

---

### Interaction System

* Every frame, a ray is cast from the camera forward.
* If it hits an interactable object within range:

  * The object is highlighted
  * A `"Press E"` prompt is shown
  * Interaction becomes available

This system is used for:

* Doors
* Pickable items
* The cooking pot
* The potion

---

### Collision System

* Uses **AABB (Axis-Aligned Bounding Boxes)**
* Each object and the player have invisible bounding boxes
* Before moving, the system checks whether the next position would cause an intersection

⚠️ **Important:**
Collision detection is **discrete**, meaning only the current and next position are tested.

At very high speeds, it is possible to **skip over thin walls** between frames (tunneling effect).

👉 This is **intentionally preserved** and used as the core escape mechanic.

---

### Doors

* Doors are modeled in Blender with correct pivots
* They rotate around their hinge axis
* Opening and closing is animated using smooth interpolation
* Doors are highlightable when interactable

---

### Recipe, Items, and Potion

* The recipe is generated automatically from the scene.
* Any object whose name contains `_pickable` is added to the recipe list.

When an item is picked up:

* It is removed from the scene
* Its status is updated in the UI

After all ingredients are collected:

* The pot becomes interactable
* Activating it spawns a potion with a particle effect
* Drinking the potion increases player speed using a speed multiplier

---

## 🕹️ Controls

| Key / Input | Action       |
| ----------- | ------------ |
| W A S D     | Move         |
| Shift       | Run          |
| Mouse       | Look around  |
| E           | Interact     |
| Mouse Click | Open doors   |
| ESC         | Unlock mouse |

---

## 🚀 How to Run

### Requirements

* **Node.js** (version 18 or newer recommended)
* A modern browser with **WebGL** support

---

### Installation & Start

In the project root directory, run:

```bash
npm install
npm run dev
```

After that, open the URL printed in the console (usually):

```
http://localhost:5173
```

---

## ⚙️ How It Runs

* The entire application runs in the browser
* No external plugins or installations are required
* All rendering is done using **WebGL via Three.js**


