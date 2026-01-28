import * as THREE from "three";
import type { Interactable } from "../interaction/Interactable";
import { GameState } from "../core/GameState";

export class Potion implements Interactable {
  object = new THREE.Group();

  liquid: THREE.Mesh;
  particles: THREE.Points;

  time = 0;

  private game: GameState;
  private scene: THREE.Scene;

  constructor(potPosition: THREE.Vector3, game: GameState, scene: THREE.Scene) {
    this.game = game;
    this.scene = scene;
    // Liquid surface
    const liquidGeo = new THREE.CircleGeometry(0.25, 32);
    const liquidMat = new THREE.MeshStandardMaterial({
      color: 0x00ff99,
      emissive: 0x00aa55,
      transparent: true,
      opacity: 0.85,
    });

    this.liquid = new THREE.Mesh(liquidGeo, liquidMat);
    this.liquid.rotation.x = -Math.PI / 2;

    // Position in the pot
    this.object.position.copy(potPosition);
    this.liquid.position.y = 0.15;

    this.object.add(this.liquid);

    // Particles
    const pCount = 60;
    const positions = new Float32Array(pCount * 3);

    for (let i = 0; i < pCount; i++) {
      positions[i * 3 + 0] = (Math.random() - 0.5) * 0.4;
      positions[i * 3 + 1] = Math.random() * 0.5;
      positions[i * 3 + 2] = (Math.random() - 0.5) * 0.4;
    }

    const pGeo = new THREE.BufferGeometry();
    pGeo.setAttribute("position", new THREE.BufferAttribute(positions, 3));

    const pMat = new THREE.PointsMaterial({
      color: 0x66ffcc,
      size: 0.03,
      transparent: true,
      opacity: 0.8,
      depthWrite: false,
    });

    this.particles = new THREE.Points(pGeo, pMat);
    this.object.add(this.particles);

    // Add to scene
    this.scene.add(this.object);

    // Make interactable
    (this.liquid as any).__interactable = this;
  }

  getPrompt() {
    return "Press E to drink potion";
  }

  interact() {
    this.game.drinkPotion();
    this.object.removeFromParent();
  }

  highlight(_: boolean) {}

  update(delta: number) {
    this.time += delta;

    // Liquid pulsing
    const scale = 1 + Math.sin(this.time * 3) * 0.05;
    this.liquid.scale.set(scale, scale, scale);

    // Particle bubbling
    const pos = this.particles.geometry.attributes
      .position as THREE.BufferAttribute;

    for (let i = 0; i < pos.count; i++) {
      let y = pos.getY(i);
      y += delta * (0.2 + Math.random() * 0.2);

      if (y > 0.6) y = 0;

      pos.setY(i, y);
    }

    pos.needsUpdate = true;
  }
}
