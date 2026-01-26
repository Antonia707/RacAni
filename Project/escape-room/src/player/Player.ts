import * as THREE from "three";
import { PointerLockControls } from "three/examples/jsm/controls/PointerLockControls.js";
import { Input } from "../core/Input";
import { checkCollision } from "../utils/Collision";
import { Hands } from "./Hands";
import { Door } from "../world/Door";
import { PointerLockManager } from "../view/PointerLockManager";

export class Player {
  object = new THREE.Object3D();
  camera = new THREE.PerspectiveCamera(
    75,
    window.innerWidth / window.innerHeight,
    0.1,
    1000,
  );

  controls: PointerLockControls;
  controlsManager: PointerLockManager;
  hands: Hands;
  input: Input;

  playerHeight = 1.7;
  playerRadius = 0.3;

  walkTime = 0;
  walkSpeed = 4;
  runningSpeed = 8;
  speedMultiplier = 1;

  raycaster = new THREE.Raycaster();
  currentDoor: Door | null = null;

  promptEl = document.getElementById("prompt")!;

  constructor(input: Input) {
    this.input = input;

    this.object.position.set(0, 2, 4);

    this.camera.position.set(0, 1.7, 0);
    this.object.add(this.camera);

    this.controlsManager = new PointerLockManager(this.camera, document.body);
    this.controls = this.controlsManager.controls;

    this.controls.lock();

    this.hands = new Hands(this.camera);

    // Mouse click interaction
    window.addEventListener("mousedown", () => {
      if (this.currentDoor) {
        this.currentDoor.toggle();
      }
    });
  }

  update(delta: number, collidables: THREE.Object3D[], doors: Door[]) {
    if (!this.controls.isLocked) return;

    // Interaction Scan
    this.scanForInteractable(doors);

    // Keyboard Interaction
    if (this.input.keysPressed["e"] && this.currentDoor) {
      this.currentDoor.toggle();
    }

    // Movement
    const speed = this.input.keys["shift"]
      ? this.runningSpeed * this.speedMultiplier
      : this.walkSpeed;

    const forward = new THREE.Vector3();
    this.camera.getWorldDirection(forward);
    forward.y = 0;
    forward.normalize();

    const right = new THREE.Vector3();
    right.crossVectors(forward, new THREE.Vector3(0, 1, 0)).normalize();

    const moveDir = new THREE.Vector3();
    if (this.input.keys["w"]) moveDir.add(forward);
    if (this.input.keys["s"]) moveDir.sub(forward);
    if (this.input.keys["a"]) moveDir.sub(right);
    if (this.input.keys["d"]) moveDir.add(right);

    if (moveDir.lengthSq() > 0) moveDir.normalize();

    const move = moveDir.multiplyScalar(speed * delta);
    const pos = this.object.position.clone();

    //  X axis collision
    const tryX = pos.clone();
    tryX.x += move.x;

    if (
      !checkCollision(tryX, collidables, this.playerHeight, this.playerRadius)
    ) {
      pos.x = tryX.x;
    }

    // Z axis collision
    const tryZ = pos.clone();
    tryZ.z += move.z;

    if (
      !checkCollision(tryZ, collidables, this.playerHeight, this.playerRadius)
    ) {
      pos.z = tryZ.z;
    }

    // Apply
    this.object.position.copy(pos);

    // Head Bobbing
    const moving = moveDir.lengthSq() > 0;
    if (moving) {
      this.walkTime +=
        delta * (this.input.keys["shift"] ? this.runningSpeed : this.walkSpeed);
    }

    const bob = Math.sin(this.walkTime) * 0.03;
    this.camera.position.y = 1.7 + bob;
    this.hands.updateBobbing(bob);
  }

  // Interaction
  private scanForInteractable(doors: Door[]) {
    this.raycaster.setFromCamera(new THREE.Vector2(0, 0), this.camera);

    const doorObjects = doors.map((d) => d.object);
    const hits = this.raycaster.intersectObjects(doorObjects, true);

    let found: Door | null = null;

    if (hits.length > 0 && hits[0].distance < 3) {
      found =
        doors.find(
          (d) =>
            d.object === hits[0].object ||
            d.object.children.includes(hits[0].object),
        ) ?? null;
    }

    // Clear previous highlight
    if (this.currentDoor && this.currentDoor !== found) {
      this.currentDoor.highlight(false);
    }

    this.currentDoor = found;

    if (this.currentDoor) {
      this.currentDoor.highlight(true);
      this.promptEl.style.opacity = "1";
    } else {
      this.promptEl.style.opacity = "0";
    }
  }
}
