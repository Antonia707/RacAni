import * as THREE from "three";
import { GLTFLoader } from "three/examples/jsm/loaders/GLTFLoader.js";
import { Door } from "./Door";
import { PickableItem } from "./PickableItem";
import type { GameState } from "../core/GameState";
import { CookingPot } from "./CookingPot";

export class Room {
  object!: THREE.Object3D;
  colliders: THREE.Object3D[] = [];
  doors: Door[] = [];
  specials: { update(delta: number): void }[] = [];

  constructor(scene: THREE.Scene, gameState: GameState) {
    const loader = new GLTFLoader();

    loader.load("../../public/room/room.glb", (gltf) => {
      this.object = gltf.scene;
      scene.add(this.object);

      this.object.traverse((child) => {
        // Meshes
        if ((child as THREE.Mesh).isMesh) {
          const mesh = child as THREE.Mesh;

          // Doors
          if (mesh.name.toLowerCase().includes("door")) {
            console.log("Found door:", mesh.name);
            const door = new Door(mesh);
            this.doors.push(door);
          }

          // Pickable items
          if (mesh.name.toLowerCase().includes("recipe")) {
            new PickableItem(mesh, "recipe", gameState, true);
          }

          if (mesh.name.toLowerCase().includes("pickable")) {
            const id = mesh.name.replace("_pickable", "");
            gameState.registerIngredient(id);
            new PickableItem(mesh, id, gameState, false);
          }

          if (mesh.name.toLowerCase().includes("pot")) {
            const pot = new CookingPot(mesh, gameState, scene);
            this.specials.push(pot);
          }

          // Colliders
          this.colliders.push(mesh);
        }

        // Lights
        if ((child as THREE.Light).isLight) {
          const light = child as THREE.Light;
          light.intensity *= 0.02;
        }
      });
    });
  }

  removeCollider(obj: THREE.Object3D) {
    this.colliders = this.colliders.filter((c) => c !== obj);
  }

  update(delta: number) {
    for (const door of this.doors) {
      door.update(delta);
    }
    for (const s of this.specials) s.update(delta);
  }
}
