package planet_attack.core;

import planet_attack.math.Vector;
import planet_attack.power_ups.PowerUpType;

public abstract class Entity extends PhysicsObject {

	private int health;
	private int maxHealth;
	private int invulnFrames;
	private PowerUpType powerUp;
	protected World world;

	public static final int COLLISION_INVULN_FRAMES = GameLoop.secondsToTicks(1);

	public Entity(double mass, Vector pos, Vector vel, double dragCoeff, double rot, double rotVel, double extents,
			int maxHealth, World world) {
		super(mass, pos, vel, dragCoeff, rot, rotVel, extents);
		this.powerUp = PowerUpType.NONE;
		this.maxHealth = maxHealth;
		health = maxHealth;
		invulnFrames = 0;
		this.world = world;
	}

	public Entity(double mass, Vector pos, double dragCoeff, double rot, double extents, int maxHealth, World world) {
		this(mass, pos, new Vector(), dragCoeff, rot, 0, extents, maxHealth, world);
	}

	/*
	 * public Entity(double extents, Vector pos, Vector vel, double rotation, double
	 * rotationVelocity, double drag, int maxHealth) { super(extents, pos, vel,
	 * rotation, rotationVelocity, drag); this.powerUp = PowerUpType.NONE;
	 * this.maxHealth = maxHealth; health = maxHealth; invulnFrames = 0; }
	 */
	@Override
	public void update(int currentTick) {
		super.update(currentTick);
		if (invulnFrames > 0) {
			invulnFrames--;
		}
		if (powerUp == PowerUpType.HEAL) {
			heal();
			powerUp = PowerUpType.NONE;
		}
	}

	public boolean isVulnerable() {
		return getInvulnFrames() == 0;
	}

	public void doCollision() {
		if (!isVulnerable()) {
			return;
		}
		if (health > 0) {
			health--;
			invulnFrames = COLLISION_INVULN_FRAMES;
		}
		if (health == 0) {
			onDeath();
		}
	}

	/**
	 * Called when the entity's health reaches 0.
	 */
	protected void onDeath() {
		setExists(false);
	}

	public void heal(int healAmount) {
		health = Math.min(health + healAmount, maxHealth);
	}

	public void heal() {
		heal(1);
	}

	public void drawWithShield(String texture, double size) {
		super.draw(texture + ".png", size);
		if ((getInvulnFrames() / 5) % 2 > 0) {
			super.draw(texture + "_shield.png", size);
			// super.draw("shield.png", size + 4);
		}
	}

	public void setInvulnFrames(int invulnFrames) {
		this.invulnFrames = invulnFrames;
	}

	public int getInvulnFrames() {
		return invulnFrames;
	}

	public void setHealth(int health) {
		this.health = health;
	}

	public int getHealth() {
		return health;
	}

	public void setMaxHealth(int maxHealth) {
		this.maxHealth = maxHealth;
	}

	public int getMaxHealth() {
		return maxHealth;
	}

	public void applyPowerUp(PowerUpType powerUp) {
		if (powerUp == PowerUpType.HEAL) {
			heal();
		} else {
			this.powerUp = powerUp;
		}
	}

	public PowerUpType getPowerUp() {
		return powerUp;
	}

	public World getWorld() {
		return world;
	}
}
