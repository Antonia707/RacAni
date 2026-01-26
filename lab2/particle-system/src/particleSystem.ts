import * as THREE from "three";

interface ParticleSystemParams {
  scene: THREE.Scene;
  particleCount?: number;
  x?: number;
  y?: number;
  z?: number;
  xRange?: number;
  yRange?: number;
  zRange?: number;
  lifeRate?: number;
  velocity?: number;
  textureUrl?: string;
}

export class ParticleSystem {
  private scene: THREE.Scene;
  private particleCount: number;
  private particles: THREE.Sprite[] = [];
  private x: number = 0;
  private y: number = 0;
  private z: number = 0;
  private xRange: number = 100;
  private yRange: number = 100;
  private zRange: number = 100;
  private lifeRate: number = 0.997;
  private velocity: number = -0.1;
  private textureUrl: string;

  constructor(
    scene: THREE.Scene,
    textureUrl: string,
    particleCount: number = 500,
    x: number = 0,
    y: number = 0,
    z: number = 0,
    xRange: number = 100,
    yRange: number = 100,
    zRange: number = 100,
    lifeRate: number = 0.997,
    velocity: number = -0.1
  ) {
    this.scene = scene;
    this.textureUrl = textureUrl; // store texture for reset
    this.particleCount = particleCount;
    this.x = x;
    this.y = y;
    this.z = z;
    this.xRange = xRange;
    this.yRange = yRange;
    this.zRange = zRange;
    this.lifeRate = lifeRate;
    this.velocity = velocity;

    const textureLoader = new THREE.TextureLoader();
    const sprite = textureLoader.load(textureUrl);

    const material = new THREE.SpriteMaterial({
      map: sprite,
      transparent: true,
    });

    for (let i = 0; i < this.particleCount; i++) {
      const particle = new THREE.Sprite(material);
      particle.position.set(
        (Math.random() - 0.5) * xRange + x,
        (Math.random() - 0.5) * yRange + y,
        (Math.random() - 0.5) * zRange + z
      );
      const scale = Math.random() * 2;
      particle.scale.set(scale, scale, 1);
      this.scene.add(particle);
      this.particles.push(particle);
    }
  }

  public update(): void {
    this.particles.forEach((p) => {
      p.position.y += this.velocity;
      p.scale.multiplyScalar(this.lifeRate);
      if (p.scale.x < 0.1) {
        p.position.set(
          (Math.random() - 0.5) * this.xRange + this.x,
          this.y + this.yRange / 2,
          (Math.random() - 0.5) * this.zRange + this.z
        );
        const scale = Math.random() * 1.5 + 0.5;
        p.scale.set(scale, scale, 1);
      }
    });
  }

  public reset(params: Partial<ParticleSystemParams>): void {
    this.particles.forEach((p) => this.scene.remove(p));
    this.particles = [];
    Object.assign(this, params);
    const newSystem = new ParticleSystem(
      this.scene,
      this.textureUrl,
      this.particleCount,
      this.x,
      this.y,
      this.z,
      this.xRange,
      this.yRange,
      this.zRange,
      this.lifeRate,
      this.velocity
    );
    this.particles = newSystem.particles;
  }
}
