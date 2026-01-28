import * as THREE from "three";

export interface Interactable {
  object: THREE.Object3D;
  getPrompt(): string;
  interact(): void;
  highlight(on: boolean): void;
}
