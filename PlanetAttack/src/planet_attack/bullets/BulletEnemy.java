package planet_attack.bullets;

import planet_attack.core.Entity;

public class BulletEnemy extends Bullet {

	public BulletEnemy(Entity entity) {
		super(BULLET_MASS_NORMAL, BULLET_SIZE_ENEMY, BULLET_SPEED_ENEMY, BULLET_DURATION_NORMAL, BULLET_SPREAD_NORMAL, BULLET_ENEMY_COOLDOWN_NORMAL, entity);
	}
}
