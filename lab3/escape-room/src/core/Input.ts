export class Input {
  keys: Record<string, boolean> = {};
  keysPressed: Record<string, boolean> = {};

  constructor() {
    window.addEventListener("keydown", (e) => {
      const k = e.key.toLowerCase();
      if (!this.keys[k]) this.keysPressed[k] = true;
      this.keys[k] = true;
    });

    window.addEventListener("keyup", (e) => {
      this.keys[e.key.toLowerCase()] = false;
    });
  }
}
