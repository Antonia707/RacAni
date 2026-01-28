import { PointerLockControls } from "three/examples/jsm/controls/PointerLockControls.js";
import * as THREE from "three";

export class PointerLockManager {
  controls: PointerLockControls;
  overlay: HTMLDivElement;

  constructor(camera: THREE.Camera, domElement: HTMLElement) {
    this.controls = new PointerLockControls(camera, domElement);

    // Create overlay
    this.overlay = document.createElement("div");
    this.overlay.style.position = "absolute";
    this.overlay.style.top = "0";
    this.overlay.style.left = "0";
    this.overlay.style.width = "100%";
    this.overlay.style.height = "100%";
    this.overlay.style.background = "rgba(0,0,0,0.5)";
    this.overlay.style.color = "white";
    this.overlay.style.display = "flex";
    this.overlay.style.justifyContent = "center";
    this.overlay.style.alignItems = "center";
    this.overlay.style.fontSize = "24px";
    this.overlay.style.zIndex = "100";
    this.overlay.style.pointerEvents = "none";
    this.overlay.innerText = "Click to resume";
    document.body.appendChild(this.overlay);

    // Show overlay when pointer unlocked
    this.controls.addEventListener("unlock", () => {
      this.overlay.style.display = "flex";
    });

    // Hide overlay when pointer locked
    this.controls.addEventListener("lock", () => {
      this.overlay.style.display = "none";
    });

    // Lock pointer on click if not locked
    document.addEventListener("click", () => {
      if (!this.controls.isLocked) {
        this.controls.lock();
      }
    });

    // Initially lock
    this.controls.lock();
  }
}
