package planet_attack.bullets;

import planet_attack.Player;
import planet_attack.asteroids.Asteroid;
import planet_attack.core.Entity;
import planet_attack.core.GameLoop;
import planet_attack.core.PhysicsObject;
import planet_attack.enemies.Enemy;
import planet_attack.math.Vector;
import planet_attack.power_ups.PowerUp;

public abstract class Bullet extends PhysicsObject {
	
	// MASS
	public static final double BULLET_MASS_NORMAL = 0.5;
	// SPEED
	public static final double BULLET_SPEED_NORMAL = GameLoop.perSecondToPerTick(135);
	public static final double BULLET_SPEED_HUGE = GameLoop.perSecondToPerTick(100);
	public static final double BULLET_SPEED_ENEMY = GameLoop.perSecondToPerTick(50);
	// DRAG
	public static final int BULLET_DRAG = 0;
	// SIZE
	public static final int BULLET_SIZE_NORMAL = 2;
	public static final int BULLET_SIZE_HUGE = 5;
	public static final int BULLET_SIZE_ENEMY = 3;
	// DURATION
	public static final int BULLET_DURATION_NORMAL = GameLoop.secondsToTicks(10);
	public static final int BULLET_DURATION_BOUNCE = GameLoop.secondsToTicks(20);
	// SPREAD
	public static final double BULLET_SPREAD_NORMAL = 0.1;
	public static final double BULLET_SPREAD_RAPID_FIRE = 0.3;
	// COOLDOWN
	public static final int BULLET_PLAYER_COOLDOWN_NORMAL = GameLoop.secondsToTicks(0.2);
	public static final int BULLET_PLAYER_COOLDOWN_BOUNCE = GameLoop.secondsToTicks(0.2);
	public static final int BULLET_PLAYER_COOLDOWN_RAPID_FIRE = GameLoop.secondsToTicks(0.07);
	public static final int BULLET_PLAYER_COOLDOWN_HUGE = GameLoop.secondsToTicks(0.4);
	public static final int BULLET_ENEMY_COOLDOWN_NORMAL = GameLoop.secondsToTicks(4);
	// MISC
	public static final double BULLET_HOMING_ROTATION_STEP_SIZE = 0.1;
	
	protected int size;
	protected int duration;
	protected int cooldown;
	protected Entity shooter;
	
	public Bullet(double mass, int size, double speed, int duration, double spread, int cooldown, Entity shooter) {
		super(mass, new Vector(shooter.getPos()), 0, 0, size);
		this.size = size;
		this.duration = duration;
		this.cooldown = cooldown;
		this.shooter = shooter;
		setVel(getBulletVector(shooter, speed, spread));
	}

	@Override
	public void doCollision(PhysicsObject other) {
		// The shooter can't shoot themselves or each other, bullets can't hit other bullets, and bullets can't hit power-ups
		if ((shooter.equals(other)) ||
				other instanceof Bullet ||
				other instanceof PowerUp) {
			return;
		}
		if (other instanceof Asteroid) {
			this.doCollision();
		}
		if (other instanceof Enemy && shooter instanceof Player) {
			((Player) shooter).addScore(Enemy.ENEMY_SCORE_KAMIKAZE);
		}
		super.doCollision(other);
	}
	
	@Override
	public void doCollision() {
		setExists(false);
	}
	
	@Override
	public void update(int currentTick) {
		if (duration > 0) {
			duration--;
		} else if (duration == 0) {
			setExists(false);
		}
		super.update(currentTick);
	}
	
	@Override
	public void draw() {
		if (shooter instanceof Player) {
			super.draw("bullet_normal.png", getSize());
		} else {
			super.draw("bullet_enemy.png", getSize());
		}
	}
	
	public static Vector getBulletVector(Entity entity, double speed, double spread) {
		Vector vector = Vector.getVectorWithRotation(entity.getRotation(), speed);
		vector.add(entity.getVel());
		vector.rotateBy((GameLoop.RANDOM.nextDouble() - 0.5) * spread);
		return vector;
	}
	
	public static Bullet createBullet(Entity entity) {
		switch (entity.getPowerUp()) {
		case BOUNCE:
			return new BulletBounce(entity);
		case RAPID_FIRE:
			return new BulletRapidFire(entity);
		case HUGE:
			return new BulletHuge(entity);
		case HOMING:
			return new BulletHoming(entity);
		default:
			return new BulletNormal(entity);
		}
	}
	
	public int getSize() {
		return size;
	}

	public Entity getShooter() {
		return shooter;
	}
	
	public int getCooldown() {
		return cooldown;
	}
	
	/*
	 * TODO: update this
	@Override
	public String toString() {
		return "Bullet " + super.toString() + ", type = " + type + ", duration = " + duration;
	}
	*/
}
