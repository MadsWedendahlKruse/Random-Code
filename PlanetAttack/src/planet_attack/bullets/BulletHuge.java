package planet_attack.bullets;

import planet_attack.core.Entity;

public class BulletHuge extends Bullet {

	public BulletHuge(Entity entity) {
		super(BULLET_MASS_NORMAL, BULLET_SIZE_HUGE, BULLET_SPEED_HUGE, BULLET_DURATION_NORMAL, BULLET_SPREAD_NORMAL, BULLET_PLAYER_COOLDOWN_HUGE, entity);
	}
}
