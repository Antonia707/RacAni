import * as THREE from "three";

export class Door {
  object: THREE.Object3D;
  isOpen = false;
  isMoving = false;

  closedRotation: number;
  openRotation: number;

  constructor(object: THREE.Object3D) {
    this.object = object;

    this.closedRotation = object.rotation.y;
    this.openRotation = this.closedRotation + Math.PI / 2; // 90 degrees
  }

  toggle() {
    if (this.isMoving) return;
    this.isOpen = !this.isOpen;
    this.isMoving = true;
  }

  highlight(on: boolean) {
    this.object.traverse((child) => {
      if ((child as THREE.Mesh).isMesh) {
        const mesh = child as THREE.Mesh;
        const mat = mesh.material as THREE.MeshStandardMaterial;

        if (!mat.emissive) return;

        mat.emissive.set(on ? 0x333300 : 0x000000);
      }
    });
  }

  update(delta: number) {
    if (!this.isMoving) return;

    const target = this.isOpen ? this.openRotation : this.closedRotation;

    const speed = 2.5; // radians per second
    const current = this.object.rotation.y;

    const diff = target - current;

    if (Math.abs(diff) < 0.1) {
      this.object.rotation.y = target;
      this.isMoving = false;
      return;
    }

    this.object.rotation.y += Math.sign(diff) * speed * delta;
  }
}
