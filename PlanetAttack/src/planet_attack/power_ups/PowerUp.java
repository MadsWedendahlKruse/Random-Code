package planet_attack.power_ups;

import planet_attack.Player;
import planet_attack.core.GameLoop;
import planet_attack.core.PhysicsObject;
import planet_attack.math.Vector;

/**
 * This class represents the actual power up that you can pick up in the world.
 */
public class PowerUp extends PhysicsObject {
	
	public static final int POWER_UP_MASS = 1;
	public static final int POWER_UP_SIZE = 8;
	public static final int POWER_UP_DRAG = 1;
	
	public static final int POWER_UP_DURATION_NONE = 0;
	public static final int POWER_UP_DURATION_BOUNCE = GameLoop.secondsToTicks(120);
	public static final int POWER_UP_DURATION_RAPID_FIRE = GameLoop.secondsToTicks(120);
	
	private PowerUpType type;
	
	public PowerUp(Vector pos, Vector vel, PowerUpType type) {
		super(POWER_UP_MASS, pos, vel, POWER_UP_DRAG, 0, 0, POWER_UP_SIZE);
		this.type = type;
	}

	@Override
	public void draw() {
		draw("power_up_" + type.toString().toLowerCase() + ".png", POWER_UP_SIZE);
	}
	
	@Override
	public void doCollision(PhysicsObject other) {
		if (other instanceof Player) {
			((Player) other).applyPowerUp(type);
			doCollision();
		}
	}

	@Override
	public void doCollision() {
		setExists(false);
	}
	
	public PowerUpType getType() {
		return type;
	}
}


