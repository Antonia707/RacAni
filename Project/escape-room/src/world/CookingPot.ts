import * as THREE from "three";
import type { Interactable } from "../core/Interactable";
import { GameState } from "../core/GameState";
import { Potion } from "./Potion";

export class CookingPot implements Interactable {
  potion: Potion | null = null;
  public object: THREE.Object3D;
  private game: GameState;
  private scene: THREE.Scene;

  constructor(object: THREE.Object3D, game: GameState, scene: THREE.Scene) {
    this.object = object;
    this.game = game;
    this.scene = scene;
    (this.object as any).__interactable = this;
  }

  getPrompt() {
    if (!this.game.canBrew() || this.game.potionBrewed) return "";
    return "Press E to brew potion";
  }

  interact() {
    if (!this.game.canBrew() || this.game.potionBrewed) return;

    this.game.brew();

    // Spawn potion at pot position
    this.potion = new Potion(
      this.object.getWorldPosition(new THREE.Vector3()),
      this.game,
      this.scene,
    );
  }

  update(delta: number) {
    if (this.potion) {
      this.potion.update(delta);
    }
  }

  highlight(_: boolean) {}
}
