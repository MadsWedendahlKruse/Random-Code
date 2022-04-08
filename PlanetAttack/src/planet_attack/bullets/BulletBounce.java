package planet_attack.bullets;

import planet_attack.core.Entity;

public class BulletBounce extends Bullet {

	public BulletBounce(Entity entity) {
		super(BULLET_MASS_NORMAL, BULLET_SIZE_NORMAL, BULLET_SPEED_NORMAL, BULLET_DURATION_BOUNCE, BULLET_SPREAD_NORMAL, BULLET_PLAYER_COOLDOWN_BOUNCE, entity);
	}
}
