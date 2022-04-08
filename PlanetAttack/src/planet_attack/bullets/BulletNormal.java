package planet_attack.bullets;

import planet_attack.core.Entity;

public class BulletNormal extends Bullet {

	public BulletNormal(Entity entity) {
		super(BULLET_MASS_NORMAL, BULLET_SIZE_NORMAL, BULLET_SPEED_NORMAL, BULLET_DURATION_NORMAL, BULLET_SPREAD_NORMAL, BULLET_PLAYER_COOLDOWN_NORMAL, entity);
	}
}
