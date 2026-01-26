import { Game } from "./core/Game";

const menu = document.getElementById("menu")!;
const startBtn = document.getElementById("startBtn")!;

startBtn.addEventListener("click", () => {
  menu.style.display = "none";
  const game = new Game();
  game.start();
});
