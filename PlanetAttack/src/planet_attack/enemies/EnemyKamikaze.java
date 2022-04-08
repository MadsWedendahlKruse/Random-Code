package planet_attack.enemies;

import planet_attack.core.World;
import planet_attack.math.Vector;

public class EnemyKamikaze extends Enemy {

	public EnemyKamikaze(Vector pos, World world) {
		super(ENEMY_MASS_KAMIKAZE, pos, ENEMY_DRAG_KAMIKAZE, ENEMY_SIZE_KAMIKAZE, ENEMY_MAX_HEALTH_KAMIKAZE, world);
	}
	
	@Override
	public void update(int currentTick) {
		// Calculate unit vector pointing towards the player target
		double dx = target.getX() - this.getX();
		double dy = target.getY() - this.getY();
		Vector force = new Vector(dx, dy).getUnitVector();
		force.scale(ENEMY_THRUST_KAMIKAZE);
		rotateTo(force.getRotation());
		applyForce(force);
		super.update(currentTick);
	}
}
