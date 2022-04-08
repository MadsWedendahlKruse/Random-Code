package planet_attack.bullets;

import java.util.ArrayList;

import planet_attack.core.Chunk;
import planet_attack.core.Entity;
import planet_attack.core.PhysicsObject;
import planet_attack.core.World;
import planet_attack.enemies.Enemy;
import planet_attack.math.Vector;

public class BulletHoming extends Bullet {

	private Entity target = null;

	public BulletHoming(Entity shooter) {
		super(BULLET_MASS_NORMAL, BULLET_SIZE_NORMAL, BULLET_SPEED_NORMAL, BULLET_DURATION_NORMAL, BULLET_SPREAD_NORMAL, BULLET_PLAYER_COOLDOWN_NORMAL, shooter);
	}

	@Override
	public void update(int currentTick) {
		if (target == null) {
			World world = shooter.getWorld();
			ArrayList<Chunk> chunks = world.getNeighbourChunks(getPos());
			Enemy closestEnemy = null;
			double distClosestEnemy = Integer.MAX_VALUE;
			for (Chunk chunk : chunks) {
				for (PhysicsObject object : chunk.getObjects()) {
					if (object instanceof Enemy) {
						double distEnemy = object.getPos().distance(this.getPos());
						if (distEnemy < distClosestEnemy) {
							distClosestEnemy = distEnemy;
							closestEnemy = (Enemy) object;
						}
					}
				}
			}
			if (closestEnemy != null) {
				target = closestEnemy;
			}
		} else if (!target.exists()) {
			target = null;
		}
		if (target != null) {
			// Calculate unit vector pointing towards the homing target
			double dx = target.getX() - this.getX();
			double dy = target.getY() - this.getY();
			Vector force = new Vector(dx, dy).getUnitVector();
			getVel().rotateTo(force.getRotation(), BULLET_HOMING_ROTATION_STEP_SIZE);
		}
		super.update(currentTick);
	}
}
