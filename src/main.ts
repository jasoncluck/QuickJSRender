class Fragment {
  constructor(
    public x: number,
    public y: number,
    public vx: number,
    public vy: number,
    public color: number[]
  ) { }

  update(dt: number, groundY: number) {
    this.vy += 40 * dt; // Gravity
    this.x += this.vx;
    this.y += this.vy;

    // Bounce Logic
    if (this.y >= groundY) {
      this.y = groundY;       // Snap to floor
      this.vy *= -0.6;        // Reverse and lose 40% energy
      this.vx *= 0.9;         // Friction on floor
    }
  }

  render() {
    nativeDrawRect(this.x, this.y, 6, 6, this.color[0], this.color[1], this.color[2]);
  }
}

class FallingSquare {
  y = -50;
  x = Math.random() * ((globalThis.windowWidth || 1024) - 40);
  speed = Math.random() * 200 + 150;
  color = [Math.random() * 255, Math.random() * 255, Math.random() * 255];
  isDead = false;

  update(dt: number) {
    this.y += this.speed * dt;
  }

  render() {
    nativeDrawRect(this.x, this.y, 40, 40, this.color[0], this.color[1], this.color[2]);
  }
}

let squares: FallingSquare[] = [];
let fragments: Fragment[] = [];
let spawnTimer = 0;
const OFFSET_FROM_BOTTOM = 200;

globalThis.render = (dt: number) => {
  const windowWidth = globalThis.windowWidth || 1024;
  const windowHeight = globalThis.windowHeight || 768;
  const groundY = windowHeight - OFFSET_FROM_BOTTOM;

  // 1. Draw the Ground
  nativeDrawRect(0, groundY + 5, windowWidth, 2, 150, 150, 150);

  // 2. Spawn Logic
  spawnTimer += dt;
  if (spawnTimer > 0.4) {
    squares.push(new FallingSquare());
    spawnTimer = 0;
  }

  // 3. Update Squares
  squares = squares.filter(s => {
    s.update(dt);
    const squareFloor = groundY - 40;

    if (s.y >= squareFloor) {
      // Explosion!
      for (let i = 0; i < 10; i++) {
        fragments.push(new Fragment(
          s.x + 20,
          squareFloor + 20,
          (Math.random() - 0.5) * 15,
          -Math.random() * 15,
          s.color
        ));
      }
      return false; // Remove square
    }
    s.render();
    return true;
  });

  // 4. Update Fragments
  fragments = fragments.filter(f => {
    f.update(dt, groundY);
    f.render();
    // Keep fragments until they eventually slide off or settle
    return f.x > -10 && f.x < windowWidth + 10 && f.y < windowHeight;
  });

  nativeDrawText(`Particles: ${fragments.length}`, 20, 20);
};

globalThis.onKeyPress = (k) => { }; // Placeholder for bridge
