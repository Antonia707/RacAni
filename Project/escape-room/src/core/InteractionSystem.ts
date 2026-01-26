import * as THREE from "three";
import { Input } from "./Input";
import type { Interactable } from "./Interactable";
import { UI } from "../view/UI";

export class InteractionSystem {
  raycaster = new THREE.Raycaster();
  current: Interactable | null = null;
  private camera: THREE.Camera;
  private scene: THREE.Scene;
  private input: Input;
  private ui: UI;

  constructor(camera: THREE.Camera, scene: THREE.Scene, input: Input, ui: UI) {
    this.camera = camera;
    this.scene = scene;
    this.input = input;
    this.ui = ui;
  }

  update() {
    this.current = null;
    this.ui.setPrompt("");

    const dir = new THREE.Vector3();
    this.camera.getWorldDirection(dir);

    this.raycaster.set(this.camera.getWorldPosition(new THREE.Vector3()), dir);

    const hits = this.raycaster.intersectObjects(this.scene.children, true);

    for (const hit of hits) {
      let obj: THREE.Object3D | null = hit.object;

      while (obj) {
        const i = (obj as any).__interactable as Interactable;
        if (i) {
          this.current = i;
          this.ui.setPrompt(i.getPrompt());
          i.highlight(true);
          break;
        }
        obj = obj.parent;
      }

      if (this.current) break;
    }

    if (this.input.keysPressed["e"] && this.current) {
      this.current.interact();
    }
  }
}
