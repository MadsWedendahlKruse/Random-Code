package planet_attack.enemies;

import planet_attack.core.GameLoop;
import planet_attack.core.World;
import planet_attack.math.Polygon;
import planet_attack.math.Vector;
import planet_attack.power_ups.PowerUp;
import planet_attack.power_ups.PowerUpType;

public class EnemyCargo extends Enemy {
	
	public EnemyCargo(Vector pos, World world) {
		super(ENEMY_MASS_CARGO, pos, ENEMY_DRAG_CARGO, ENEMY_SIZE_CARGO, ENEMY_MAX_HEALTH_CARGO, world);
		hitbox = new Polygon(pos, (double) ENEMY_SIZE_CARGO, (double) ENEMY_SIZE_CARGO - 2);
	}
	
	@Override
	public void update(int currentTick) {
		// 1% chance of turning
		if (GameLoop.RANDOM.nextInt(100) == 0) {
			// fifty-fifty chance of turning left or right
			if (GameLoop.RANDOM.nextInt(2) > 0) {
				rotate(ENEMY_ROTATION_STEP_CARGO);
			} else {
				rotate(-ENEMY_ROTATION_STEP_CARGO);
			}
		}
		Vector force = Vector.getVectorWithRotation(getRotation(), ENEMY_THRUST_CARGO);
		applyForce(force);
		super.update(currentTick);
	}
	
	@Override
	protected void onDeath() {
		world.spawnObject(new PowerUp(getPos(), getVel(), PowerUpType.values()[GameLoop.RANDOM.nextInt(PowerUpType.values().length - 1) + 1]));
		//world.spawnObject(new PowerUp(getPos(), getVel(), PowerUpType.HOMING));
		super.onDeath();
	}
}
