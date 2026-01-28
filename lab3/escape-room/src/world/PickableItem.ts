import * as THREE from "three";
import type { Interactable } from "../interaction/Interactable";
import { GameState } from "../core/GameState";

export class PickableItem implements Interactable {
  private isRecipe: boolean;
  private id: string;
  private game: GameState;
  public object: THREE.Object3D;

  constructor(
    object: THREE.Object3D,
    id: string,
    game: GameState,
    isRecipe: boolean,
  ) {
    this.id = id;
    this.game = game;
    this.isRecipe = isRecipe;
    this.object = object;
    (this.object as any).__interactable = this;
  }

  getPrompt() {
    if (this.isRecipe) return "Press E to read recipe";

    if (!this.game.canPickItems()) return "";

    return "Press E to pick up";
  }

  interact() {
    if (this.isRecipe) {
      this.game.onRecipePicked();
      this.object.removeFromParent();
      return;
    }

    if (!this.game.canPickItems()) return;

    this.game.pickItem(this.id);
    this.object.removeFromParent();
  }

  highlight(_: boolean) {}
}
