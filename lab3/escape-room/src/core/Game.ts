import * as THREE from "three";
import { Renderer } from "./Renderer";
import { Input } from "./Input";
import { Player } from "../player/Player";
import { Room } from "../world/Room";
import { GameState } from "./GameState";
import { InteractionSystem } from "../interaction/InteractionSystem";
import { UI } from "../UI/UI";
import { GameFinish } from "../UI/GameFinish";

export class Game {
  scene = new THREE.Scene();
  renderer = new Renderer();
  ui = new UI();
  gameState!: GameState;
  gameFinish = new GameFinish(
    new THREE.Vector3(0, 2, 0),
    new THREE.Vector3(60, 4, 40),
  );
  interaction!: InteractionSystem;
  input = new Input();
  player!: Player;
  room!: Room;

  prevTime = performance.now();

  start() {
    this.scene.background = new THREE.Color(0xa8f7ea);

    this.scene.add(new THREE.AmbientLight(0xffffff, 0.95));

    this.player = new Player(this.input);
    this.scene.add(this.player.object);

    this.gameState = new GameState(this.ui, this.player);
    this.room = new Room(this.scene, this.gameState);

    this.interaction = new InteractionSystem(
      this.player.camera,
      this.scene,
      this.input,
      this.ui,
    );

    this.renderer.setCamera(this.player.camera);

    this.animate();
  }

  animate = () => {
    requestAnimationFrame(this.animate);

    const now = performance.now();
    const delta = (now - this.prevTime) / 1000;
    this.prevTime = now;

    const collidables = [...this.room.colliders];

    this.player.update(delta, collidables, this.room.doors);
    this.room.update(delta);

    this.interaction.update();
    this.gameFinish.check(this.player.object.position);

    this.input.keysPressed = {};

    this.renderer.render(this.scene);
  };
}
