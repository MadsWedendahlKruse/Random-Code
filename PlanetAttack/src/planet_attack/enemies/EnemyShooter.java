package planet_attack.enemies;

import planet_attack.bullets.Bullet;
import planet_attack.bullets.BulletEnemy;
import planet_attack.core.World;
import planet_attack.math.Polygon;
import planet_attack.math.Vector;

public class EnemyShooter extends Enemy {
	
	private int cooldown = 0;
	
	public EnemyShooter(Vector pos, World world) {
		super(ENEMY_MASS_SHOOTER, pos, ENEMY_DRAG_SHOOTER, ENEMY_SIZE_SHOOTER, ENEMY_MAX_HEALTH_SHOOTER, world);
		hitbox = new Polygon(pos, 3, ENEMY_SIZE_SHOOTER);
	}
	
	@Override
	public void update(int currentTick) {
		if (cooldown > 0) {
			cooldown--;
		}
		// Calculate unit vector pointing towards the player target
		double dx = target.getX() - this.getX();
		double dy = target.getY() - this.getY();
		Vector targetDir = new Vector(dx, dy);
		rotateTo(targetDir.getRotation());
		if (cooldown == 0) {
			Bullet bullet = new BulletEnemy(this);
	    	world.spawnObject(bullet);
	    	cooldown = bullet.getCooldown();
		}
		super.update(currentTick);
	}
}
