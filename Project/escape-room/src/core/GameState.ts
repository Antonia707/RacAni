import { UI } from "../view/UI";
import { Player } from "../player/Player";

export class GameState {
  private ui: UI;
  private player: Player;
  hasRecipe = false;
  potionBrewed = false;
  potionDrunk = false;

  required = new Set<string>();
  collected = new Set<string>();

  constructor(ui: UI, player: Player) {
    this.ui = ui;
    this.player = player;
  }

  registerIngredient(id: string) {
    this.required.add(id);
  }

  onRecipePicked() {
    this.hasRecipe = true;
    this.updateUI();
  }

  canPickItems() {
    return this.hasRecipe;
  }

  pickItem(id: string) {
    this.collected.add(id);
    this.updateUI();
  }

  canBrew() {
    return this.hasRecipe && this.collected.size === this.required.size;
  }

  brew() {
    this.potionBrewed = true;
    this.updateUI();
  }

  drinkPotion() {
    this.potionDrunk = true;
    this.player.speedMultiplier = 12;
    this.updateUI();
  }

  updateUI() {
    if (!this.hasRecipe) return;

    let text = "📜 Recipe:\n\n";

    for (const id of this.required) {
      text += this.collected.has(id) ? `✅ ${id}\n` : `❌ ${id}\n`;
    }

    if (this.canBrew() && !this.potionBrewed) {
      text += "\n➡ Brew the potion!";
    }

    if (this.potionDrunk) {
      text += "\n🎉 You feel empowered!";
    }

    this.ui.showRecipe(text);
  }
}
