package planet_attack;
import java.awt.event.KeyEvent;

import planet_attack.bullets.Bullet;
import planet_attack.core.Entity;
import planet_attack.core.GameLoop;
import planet_attack.core.PhysicsObject;
import planet_attack.core.World;
import planet_attack.math.Polygon;
import planet_attack.math.Vector;
import planet_attack.power_ups.PowerUp;
import planet_attack.power_ups.PowerUpType;

public class Player extends Entity {
	
	public static final double PLAYER_MASS = 2000;
	public static final double PLAYER_SIZE = 8;
	public static final int PLAYER_MAX_HEALTH = 20;
	public static final double PLAYER_THRUST = GameLoop.perSecondSquaredToPerTickSquared(250000);
	public static final double PLAYER_DRAG = 40;
	public static final double ROTATION_ACCELERATION = GameLoop.perSecondToPerTick(3);
	
	public static final int PLAYER_1_FORWARD = KeyEvent.VK_W;
	public static final int PLAYER_1_LEFT = KeyEvent.VK_A;
	public static final int PLAYER_1_RIGHT = KeyEvent.VK_D;
	public static final int PLAYER_1_SHOOT = KeyEvent.VK_SPACE;
	
	private int cooldown;
	private int left;
	private int forward;
	private int right;
	private int shoot;
	private int[] keybinds = new int[4];
	private int score;
	private int playerNum;
	
	public Player(Vector pos, int playerNum, World world) {
		super(PLAYER_MASS, pos, PLAYER_DRAG, 0, PLAYER_SIZE, PLAYER_MAX_HEALTH, world);
		keybinds[0] = forward = PLAYER_1_FORWARD;
		keybinds[1] = left = PLAYER_1_LEFT;
		keybinds[2] = right = PLAYER_1_RIGHT;
		keybinds[3] = shoot = PLAYER_1_SHOOT;
		cooldown = 0;
		this.playerNum = playerNum;
		this.world = world;
		applyPowerUp(PowerUpType.NONE);
		hitbox = new Polygon(pos, 8, PLAYER_SIZE);
	}
	
	@Override
	public void update(int currentTick) {
		super.update(currentTick);
		handleInput();
		if (cooldown > 0) {
			cooldown--;
		}
	}
	
	/**
	 * Handles keyboard inputs and modifies the player accordingly.
	 * Returns whether or not a key was pressed when it was called.
	 */
	public boolean handleInput() {
		boolean keyPressed = false;
		for (int keybind : keybinds) {
			if (StdDraw.isKeyPressed(keybind)) {
				if (keybind == forward) {
			    	Vector thrust = new Vector(PLAYER_THRUST*Math.cos(getRotation()), PLAYER_THRUST*Math.sin(getRotation()));
			    	applyForce(thrust);
				} 
			    if (keybind == left) {
			        rotate(ROTATION_ACCELERATION);
			    }
			    if (keybind == right) {
			    	rotate(-ROTATION_ACCELERATION);
			    }
			    if (keybind == shoot && cooldown == 0 && exists()) {
			    	Bullet bullet = Bullet.createBullet(this);
			    	// Vector bulletVel = bullet.getVel();
			    	// double dx = StdDraw.mouseX() - this.getX() + GUI.cameraX;
			    	// double dy = StdDraw.mouseY() - this.getY() + GUI.cameraY;
			    	// bulletVel.rotateTo(Math.atan2(dy, dx));
			    	world.spawnObject(bullet);
			    	cooldown = bullet.getCooldown();
			    }
			    keyPressed = true;
			}
		}
		return keyPressed;
	}
	
	@Override
	public void draw() {
		super.drawWithShield("spaceship", PLAYER_SIZE);
	}
	
	@Override
	public void doCollision(PhysicsObject other) {
		// Players can't shoot themselves or each other
		if (other instanceof Bullet && ((Bullet) other).getShooter() instanceof Player) {
			return;
		}
		if (other instanceof PowerUp) {
			applyPowerUp(((PowerUp) other).getType());
		}
		super.doCollision(other);
	}
	
	@Override
	public void heal() {
		heal(5);
	}
	
	public void addScore(int score) {
		this.score += score;
	}
	
	public int getScore() {
		return score;
	}
	
	public int getPlayerNum() {
		return playerNum;
	}
	
	public World getWorld() {
		return world;
	}
	
	@Override
	public String toString() {
		// TODO Auto-generated method stub
		return "Player " + super.toString();
	}
}
