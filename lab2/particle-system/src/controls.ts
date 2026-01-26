import { PointerLockControls } from "three/examples/jsm/controls/PointerLockControls";
import { PerspectiveCamera, WebGLRenderer } from "three";

interface Keys {
  [key: string]: boolean;
}

export class FPSControls {
  public controls: PointerLockControls;
  private velocity = { x: 0, y: 0, z: 0 };
  private direction = { x: 0, z: 0 };
  private moveSpeed = 0.1;
  private keys: Keys = {};

  constructor(camera: PerspectiveCamera, renderer: WebGLRenderer) {
    this.controls = new PointerLockControls(camera, renderer.domElement);

    // Click to lock pointer
    renderer.domElement.addEventListener("click", () => {
      this.controls.lock();
    });

    // Key press events
    document.addEventListener("keydown", (e) => (this.keys[e.code] = true));
    document.addEventListener("keyup", (e) => (this.keys[e.code] = false));
  }

  public update(): void {
    this.velocity.x = 0;
    this.velocity.z = 0;

    if (this.keys["KeyS"]) this.velocity.z = -this.moveSpeed;
    if (this.keys["KeyW"]) this.velocity.z = this.moveSpeed;
    if (this.keys["KeyA"]) this.velocity.x = -this.moveSpeed;
    if (this.keys["KeyD"]) this.velocity.x = this.moveSpeed;
    if (this.keys["KeyQ"]) this.velocity.y = this.moveSpeed;
    if (this.keys["KeyE"]) this.velocity.y = -this.moveSpeed;

    // Move in the direction camera is facing
    const forward = this.velocity.z;
    const right = this.velocity.x;

    this.controls.moveRight(right);
    this.controls.moveForward(forward);
  }
}
