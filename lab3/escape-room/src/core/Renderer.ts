import * as THREE from "three";

export class Renderer {
  renderer: THREE.WebGLRenderer;
  camera!: THREE.Camera;

  constructor() {
    this.renderer = new THREE.WebGLRenderer({ antialias: true });
    this.renderer.setSize(window.innerWidth, window.innerHeight);

    this.renderer.shadowMap.enabled = true;
    this.renderer.shadowMap.type = THREE.PCFSoftShadowMap;

    (this.renderer as any).physicallyCorrectLights = true;
    this.renderer.outputColorSpace = THREE.SRGBColorSpace;
    this.renderer.toneMapping = THREE.ACESFilmicToneMapping;
    this.renderer.toneMappingExposure = 1.0;

    document.body.appendChild(this.renderer.domElement);

    window.addEventListener("resize", () => {
      if (!this.camera) return;
      const cam = this.camera as THREE.PerspectiveCamera;
      cam.aspect = window.innerWidth / window.innerHeight;
      cam.updateProjectionMatrix();
      this.renderer.setSize(window.innerWidth, window.innerHeight);
    });
  }

  setCamera(camera: THREE.Camera) {
    this.camera = camera;
  }

  render(scene: THREE.Scene) {
    this.renderer.render(scene, this.camera);
  }
}
