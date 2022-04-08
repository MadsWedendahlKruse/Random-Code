package planet_attack.asteroids;

import planet_attack.bullets.Bullet;
import planet_attack.bullets.BulletBounce;
import planet_attack.core.Entity;
import planet_attack.core.GameLoop;
import planet_attack.core.PhysicsObject;
import planet_attack.core.World;
import planet_attack.math.Polygon;
import planet_attack.math.Vector;

public class Asteroid extends Entity {
	
	public static final int ASTEROID_SIZE_SMALL = 4;
	public static final int ASTEROID_SIZE_MEDIUM = 6;
	public static final int ASTEROID_SIZE_LARGE = 8;
	public static final int ASTEROID_SIZE_HUGE = 16;
	public static final int ASTEROID_HITBOX_SIDES = 8;
	public static final double ASTEROID_DRAG_SMALL = 100;
	public static final double ASTEROID_DRAG_MEDIUM = 150;
	public static final double ASTEROID_DRAG_LARGE = 250;
	public static final double ASTEROID_DRAG_HUGE = 500;
	public static final double ASTEROID_MASS_SMALL = 10000;
	public static final double ASTEROID_MASS_MEDIUM = 40000;
	public static final double ASTEROID_MASS_LARGE = 120000;
	public static final double ASTEROID_MASS_HUGE = 300000;
	public static final int ASTEROID_HEALTH = 1000;
	public static final int ASTEROID_SPAWN_PROTECTION_FRAMES = 20;
	
	private AsteroidSize size;
	
	/**
	 * Prevents asteroid from being immediately destroyed after spawning
	 */
	private int spawnProtectionTicks; 
	
	public Asteroid(Vector pos, Vector vel, AsteroidSize size, World world, double rotationVelocity) {
		super(getMassFromSize(size), pos, vel, getDragFromSize(size), 0, rotationVelocity, getExtentsFromSize(size), ASTEROID_HEALTH, world);
		this.size = size;
		this.setExists(true);
		hitbox = new Polygon(pos, ASTEROID_HITBOX_SIDES, getExtentsFromSize(size));
	}
	
	public Asteroid(Vector pos, Vector vel, AsteroidSize size, World world, double rotationVelocity, int spawnProtectionFrames) {
		this(pos, vel, size, world, rotationVelocity);
		this.spawnProtectionTicks = spawnProtectionFrames;
	}
	
	public Asteroid(Vector pos, Vector vel, AsteroidSize size, World world) {
		this(pos, vel, size, world, 0);
	}
	
	public void explode(Vector impactVel) {
		if (spawnProtectionTicks > 0) {
			return;
		}
		// Asteroid doesn't "exist" after exploding
		this.setExists(false);
		// Only split asteroid if it is larger than SMALL
		if (this.size != AsteroidSize.SMALL) {
 			AsteroidSize newSize = AsteroidSize.values()[size.ordinal() - 1];
 			Vector velAsteroid1 = new Vector(impactVel.getX() - impactVel.getY(), impactVel.getY() + impactVel.getX());
 			velAsteroid1.scale(0.5);
 			Vector velAsteroid2 = new Vector(impactVel.getX() + impactVel.getY(), impactVel.getY() - impactVel.getX());
 			velAsteroid2.scale(0.5);
 			Vector impactVelUnit = impactVel.getUnitVector();
 			double distFromSpawn = getExtentsFromSize(newSize)/2;
 			impactVelUnit.scale(distFromSpawn);
 			Vector newPos1 = new Vector(getX() - impactVelUnit.getY(), getY() + impactVelUnit.getX());
 			Vector newPos2 = new Vector(getX() + impactVelUnit.getY(), getY() - impactVelUnit.getX());
 			double rotVel1 = GameLoop.RANDOM.nextDouble()/30;
 			double rotVel2 = -GameLoop.RANDOM.nextDouble()/30;
 			world.spawnObject(new Asteroid(newPos1, velAsteroid1, newSize, world, rotVel1, ASTEROID_SPAWN_PROTECTION_FRAMES));
 			world.spawnObject(new Asteroid(newPos2, velAsteroid2, newSize, world, rotVel2, ASTEROID_SPAWN_PROTECTION_FRAMES));
		}
	}
	
	@Override
	public void doCollision(PhysicsObject other) {
		if (other instanceof Asteroid && spawnProtectionTicks == 0) {
			// If we hit another asteroid, do elastic collsion
			doElasticCollision(other);
			this.setSpawnProtectionTicks(ASTEROID_SPAWN_PROTECTION_FRAMES);
			((Asteroid) other).setSpawnProtectionTicks(ASTEROID_SPAWN_PROTECTION_FRAMES);
		} else if (other instanceof Bullet) {
			if (other instanceof BulletBounce) {
				doElasticCollision(other);
			} else {
				other.doCollision();
			}
		} else {
			explode(other.getVel());
			other.doCollision();
		}
	}

	@Override
	public void update(int currentTick) {
		if (spawnProtectionTicks > 0) {
			spawnProtectionTicks--;
		}
		super.update(currentTick);
	}
	
	@Override
	public void draw() {
		super.draw("asteroid_" + size.toString().toLowerCase().charAt(0) + ".png", getExtentsFromSize(this.size), getExtentsFromSize(this.size));
		//StdDraw.picture(getX(), getY(), "asteroid_" + size.toString().toLowerCase().charAt(0) + ".png", getExtentsFromSize(this.size), getExtentsFromSize(this.size), getRotationDegrees());
		//getVelSI().draw(getPos(), StdDraw.RED);
		//getAccelSI().draw(getPos(), StdDraw.GREEN);
	}
	
	private static double getExtentsFromSize(AsteroidSize size) {
		switch (size) {
		case SMALL:
			return ASTEROID_SIZE_SMALL;
		case MEDIUM:
			return ASTEROID_SIZE_MEDIUM;
		case LARGE:
			return ASTEROID_SIZE_LARGE;
		case HUGE:
			return ASTEROID_SIZE_HUGE;
		default:
			return 0;
		}
	}
	
	private static double getDragFromSize(AsteroidSize size) {
		switch (size) {
		case SMALL:
			return ASTEROID_DRAG_SMALL;
		case MEDIUM:
			return ASTEROID_DRAG_MEDIUM;
		case LARGE:
			return ASTEROID_DRAG_LARGE;
		case HUGE:
			return ASTEROID_DRAG_HUGE;
		default:
			return 0;
		}
	}
	
	private static double getMassFromSize(AsteroidSize size) {
		switch (size) {
		case SMALL:
			return ASTEROID_MASS_SMALL;
		case MEDIUM:
			return ASTEROID_MASS_MEDIUM;
		case LARGE:
			return ASTEROID_MASS_LARGE;
		case HUGE:
			return ASTEROID_MASS_HUGE;
		default:
			return 0;
		}
	}
	
	@Override
	public String toString() {
		return super.toString() + ", size = " + size + ", spawnFrames = " + spawnProtectionTicks;
	}
	
	public int getSpawnProtectionTicks() {
		return spawnProtectionTicks;
	}
	
	public void setSpawnProtectionTicks(int spawnProtectionTicks) {
		this.spawnProtectionTicks = spawnProtectionTicks;
	}
}

