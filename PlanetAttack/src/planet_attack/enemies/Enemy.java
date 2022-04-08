package planet_attack.enemies;

import planet_attack.Player;
import planet_attack.bullets.Bullet;
import planet_attack.core.Entity;
import planet_attack.core.GameLoop;
import planet_attack.core.PhysicsObject;
import planet_attack.core.World;
import planet_attack.math.Vector;

public abstract class Enemy extends Entity {
	
	// MASS
	public static final double ENEMY_MASS_KAMIKAZE = 1500;
	public static final double ENEMY_MASS_CARGO = 150000;
	public static final double ENEMY_MASS_SHOOTER = 1500;
	// DRAG
	public static final double ENEMY_DRAG_KAMIKAZE = 40;
	public static final double ENEMY_DRAG_CARGO = 60;
	public static final double ENEMY_DRAG_SHOOTER = 40;
	public static final double ENEMY_DRAG_SPINNER = 80;
	// THRUST
	public static final double ENEMY_THRUST_KAMIKAZE = GameLoop.perSecondSquaredToPerTickSquared(150000);
	public static final double ENEMY_THRUST_CARGO = GameLoop.perSecondSquaredToPerTickSquared(150000);
	public static final double ENEMY_THRUST_SHOOTER = GameLoop.perSecondSquaredToPerTickSquared(150000);
	public static final double ENEMY_THRUST_SPINNER = 14; // Actually in m/tick 
	// SIZE
	public static final int ENEMY_SIZE_KAMIKAZE = 8;
	public static final int ENEMY_SIZE_CARGO = 8;
	public static final int ENEMY_SIZE_SHOOTER = 8;
	public static final int ENEMY_SIZE_SPINNER = 8;
	// MAX HEALTH
	public static final int ENEMY_MAX_HEALTH_KAMIKAZE = 1;
	public static final int ENEMY_MAX_HEALTH_CARGO = 5;
	public static final int ENEMY_MAX_HEALTH_SHOOTER = 1;
	// SCORE
	public static final int ENEMY_SCORE_KAMIKAZE = 5;
	public static final int ENEMY_SCORE_CARGO = 5;
	public static final int ENEMY_SCORE_SHOOTER = 5;
	// MISC
	public static final double ENEMY_ROTATION_STEP_CARGO = Math.PI/10;
	public static final int ENEMY_SPINNER_BLADE_ROT_STEP = 20; // Degrees
	public static final int ENEMY_SPINNER_BLADE_RANGE = 30;
	public static final int ENEMY_SPINNER_BLADE_RANGE_SQUARED = ENEMY_SPINNER_BLADE_RANGE*ENEMY_SPINNER_BLADE_RANGE;
	public static final int ENEMY_SPINNER_DASH_COOLDOWN = GameLoop.secondsToTicks(3);
	
	protected Player target;
	protected int size;
	
	public Enemy(double mass, Vector pos, double drag, int size, int maxHealth, World world) {
		super(mass, pos, drag, 0, size, maxHealth, world);
		target = world.getRandomPlayer();
		this.size = size;
	}

	@Override
	public void draw() {
		String fileName = this.getClass().getSimpleName().replace("Enemy", "enemy_").toLowerCase();
		if (getMaxHealth() > 1) {
			super.drawWithShield(fileName, size);
		} else {
			super.draw(fileName + ".png", size);
		}
	}
	
	@Override
	public void doCollision(PhysicsObject other) {
		if (other instanceof Enemy) {
			doElasticCollision(other);
			return;
		}
		if (other instanceof Bullet) {
			if (((Bullet) other).getShooter() instanceof Player) {
				if (getHealth() == 1 && isVulnerable()) {
					((Player) ((Bullet) other).getShooter()).addScore(Enemy.ENEMY_SCORE_KAMIKAZE);
				}
			} else {
				return;
			}
		}
		super.doCollision(other);
	}
	
	@Override
	protected void onDeath() {
		world.reduceEnemyCount(this);
		super.onDeath();
	}
	
	public static Enemy createEnemy(Vector pos, World world) {
		// Do some more advanced stuff here :^)
		if (GameLoop.RANDOM.nextDouble() > 0.7) {
			if (GameLoop.RANDOM.nextDouble() > 0.6 && world.getWave() > 2) {
				return new EnemyShooter(pos, world);
			}
			return new EnemySpinner(pos, world);
		}
		return new EnemyKamikaze(pos, world);
	}
}
