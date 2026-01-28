import * as THREE from "three";
import { GLTFLoader } from "three/examples/jsm/loaders/GLTFLoader.js";

export class Hands {
  handsObject = new THREE.Object3D();

  constructor(camera: THREE.Camera) {
    const loader = new GLTFLoader();
    loader.load("/../../assets/hands/hands.glb", (gltf) => {
      const hands = gltf.scene;
      hands.scale.set(0.1, 0.1, 0.1);
      hands.position.set(0, 0, -0.2);
      camera.add(hands);
      this.handsObject = hands;
    });
  }

  updateBobbing(bob: number) {
    this.handsObject.position.y = -0.05 + bob;
  }
}
