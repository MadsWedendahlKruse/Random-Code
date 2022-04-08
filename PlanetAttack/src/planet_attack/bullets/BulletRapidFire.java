package planet_attack.bullets;

import planet_attack.core.Entity;

public class BulletRapidFire extends Bullet {

	public BulletRapidFire(Entity entity) {
		super(BULLET_MASS_NORMAL, BULLET_SIZE_NORMAL, BULLET_SPEED_NORMAL, BULLET_DURATION_NORMAL, BULLET_SPREAD_RAPID_FIRE, BULLET_PLAYER_COOLDOWN_RAPID_FIRE, entity);
	}
}
