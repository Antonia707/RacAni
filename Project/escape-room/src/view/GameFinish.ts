import * as THREE from "three";

export class GameFinish {
  area: THREE.Box3;
  finished = false;

  constructor(exitPosition: THREE.Vector3, size: THREE.Vector3) {
    // Create a box representing the exit area
    const min = exitPosition.clone().sub(size.clone().multiplyScalar(0.5));
    const max = exitPosition.clone().add(size.clone().multiplyScalar(0.5));
    this.area = new THREE.Box3(min, max);
  }

  check(playerPos: THREE.Vector3): boolean {
    if (!this.area.containsPoint(playerPos)) {
      this.finished = true;
      this.onFinish();
      return true;
    }
    return false;
  }

  onFinish() {
    const overlay = document.createElement("div");
    overlay.style.position = "absolute";
    overlay.style.top = "0";
    overlay.style.left = "0";
    overlay.style.width = "100%";
    overlay.style.height = "100%";
    overlay.style.background = "rgba(0, 0, 0, 0.1)";
    overlay.style.color = "white";
    overlay.style.display = "flex";
    overlay.style.justifyContent = "center";
    overlay.style.alignItems = "center";
    overlay.style.fontSize = "36px";
    overlay.style.zIndex = "100";
    overlay.innerText = "You Escaped! 🎉";
    document.body.appendChild(overlay);
  }
}
