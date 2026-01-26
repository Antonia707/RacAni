import * as THREE from "three";

const tempBox = new THREE.Box3();
const playerBox = new THREE.Box3();

export function checkCollision(
  playerPos: THREE.Vector3,
  colliders: THREE.Object3D[],
  playerHeight = 1.7,
  playerRadius = 0.3,
): boolean {
  playerBox.min.set(
    playerPos.x - playerRadius,
    playerPos.y - playerHeight,
    playerPos.z - 2 * playerRadius,
  );

  playerBox.max.set(
    playerPos.x + playerRadius,
    playerPos.y,
    playerPos.z + 2 * playerRadius,
  );

  for (const obj of colliders) {
    if (!obj.parent) continue;
    if (!(obj as any).geometry) continue;
    if ((obj as any).userData.disableCollision) continue;

    const mesh = obj as THREE.Mesh;

    mesh.geometry.computeBoundingBox();
    tempBox.copy(mesh.geometry.boundingBox!);
    tempBox.applyMatrix4(mesh.matrixWorld);

    if (tempBox.intersectsBox(playerBox)) {
      return true;
    }
  }

  return false;
}
