import { createScene } from "./scene";
import { createCamera } from "./camera";
import { createRenderer } from "./renderer";
import { ParticleSystem } from "./particleSystem";
import { FPSControls } from "./controls";
import * as THREE from "three";

const scene = createScene();
const camera = createCamera();
const renderer = createRenderer();
document.body.appendChild(renderer.domElement);

//const geometry = new THREE.BoxGeometry(5, 5, 5);
//const material = new THREE.MeshStandardMaterial({ color: 0xff0000 });
//const cube = new THREE.Mesh(geometry, material);
//scene.add(cube);

camera.position.set(0, 10, 30);
camera.lookAt(0, 0, 0);

const fpsControls = new FPSControls(camera, renderer);
const particleSystem = new ParticleSystem(scene, "./public/snow.bmp");

const form = document.getElementById("particleForm") as HTMLFormElement;
form.addEventListener("submit", (e) => {
  e.preventDefault();
  const formData = new FormData(form);
  particleSystem.reset({
    particleCount: Number(formData.get("particleCount")),
    x: Number(formData.get("x")),
    y: Number(formData.get("y")),
    z: Number(formData.get("z")),
    xRange: Number(formData.get("xRange")),
    yRange: Number(formData.get("yRange")),
    zRange: Number(formData.get("zRange")),
    lifeRate: Number(formData.get("lifeRate")),
    velocity: Number(formData.get("velocity")),
  });
});

function animate(): void {
  requestAnimationFrame(animate);
  fpsControls.update();
  particleSystem.update();
  renderer.render(scene, camera);
}

animate();
