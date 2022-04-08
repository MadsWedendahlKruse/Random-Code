package planet_attack.enemies;

import planet_attack.asteroids.Asteroid;
import planet_attack.bullets.Bullet;
import planet_attack.core.PhysicsObject;
import planet_attack.core.World;
import planet_attack.math.Polygon;
import planet_attack.math.Vector;

public class EnemySpinner extends Enemy {
	
	private int dashCooldown = 0;
	private int bladeCooldown = 0;
	private int bladeRotation = 0;
	private boolean bladesActive = false;
	
	public EnemySpinner(Vector pos, World world) {
		super(ENEMY_MASS_SHOOTER, pos, ENEMY_DRAG_SPINNER, ENEMY_SIZE_SHOOTER, ENEMY_MAX_HEALTH_SHOOTER, world);
		hitbox = new Polygon(pos, 8, ENEMY_SIZE_SHOOTER);
		hitbox.rotateBy(0.39269908); // Rotate polygon by 22.5 degrees
	}
	
	@Override
	public void update(int currentTick) {
		if (dashCooldown > 0) {
			dashCooldown--;
		}
		if (bladeCooldown > 0) {
			bladeCooldown--;
		}
		// Calculate unit vector pointing towards the player target
		double dx = target.getX() - this.getX();
		double dy = target.getY() - this.getY();
		Vector distVec = new Vector(dx, dy);
		if (distVec.getMagnitudeSquared() <= 16*ENEMY_SPINNER_BLADE_RANGE_SQUARED && bladeCooldown == 0) {
			if (!bladesActive) {
				bladesActive = true;
				hitbox = new Polygon(getPos(), 8, ENEMY_SPINNER_BLADE_RANGE);
			}
		} else {
			if (bladesActive) {
				bladesActive = false;
				hitbox = new Polygon(getPos(), 8, ENEMY_SIZE_SHOOTER);
				hitbox.rotateBy(0.39269908); // Rotate polygon by 22.5 degrees
			}
		}
		if (dashCooldown == 0) {
			distVec = distVec.getUnitVector();
			distVec.scale(ENEMY_THRUST_SPINNER);
			if (bladeCooldown > 0) {
				// Dash away if blades aren't active
				distVec.scale(-1); 
			}
			setVel(distVec);
			dashCooldown = ENEMY_SPINNER_DASH_COOLDOWN;
		}
		super.update(currentTick);
	}
	
	@Override
	public void draw() {
		if (bladesActive) {
			draw("enemy_spinner_blade.png", ENEMY_SPINNER_BLADE_RANGE, ENEMY_SPINNER_BLADE_RANGE, bladeRotation);
		}
		bladeRotation += ENEMY_SPINNER_BLADE_ROT_STEP;
		super.draw();
	}
	
	@Override
	public void doCollision(PhysicsObject other) {
		if (bladeCooldown == 0 && bladesActive) {
			if (other instanceof Bullet || other instanceof Asteroid) {
				bladeCooldown = 200;
				dashCooldown = ENEMY_SPINNER_DASH_COOLDOWN/10;
				other.doCollision();
				return;
			}
		}
		super.doCollision(other);
	}
}
