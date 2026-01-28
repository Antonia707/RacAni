export class UI {
  prompt = document.createElement("div");
  crosshair = document.createElement("div");
  recipe = document.createElement("div");

  constructor() {
    // Crosshair
    this.crosshair.innerText = "+";
    Object.assign(this.crosshair.style, {
      position: "fixed",
      left: "50%",
      top: "50%",
      transform: "translate(-50%, -50%)",
      color: "white",
      fontSize: "24px",
      pointerEvents: "none",
    });
    document.body.appendChild(this.crosshair);

    // Prompt
    Object.assign(this.prompt.style, {
      position: "fixed",
      bottom: "20%",
      width: "100%",
      textAlign: "center",
      color: "white",
      fontSize: "20px",
      pointerEvents: "none",
    });
    document.body.appendChild(this.prompt);

    // Recipe UI
    Object.assign(this.recipe.style, {
      position: "fixed",
      right: "20px",
      top: "20px",
      width: "250px",
      background: "#e8d8a8",
      padding: "12px",
      fontFamily: "monospace",
      display: "none",
    });
    document.body.appendChild(this.recipe);
  }

  setPrompt(text: string) {
    this.prompt.innerText = text;
  }

  showRecipe(text: string) {
    this.recipe.style.display = "block";
    this.recipe.innerText = text;
  }

  hideRecipe() {
    this.recipe.style.display = "none";
  }
}
