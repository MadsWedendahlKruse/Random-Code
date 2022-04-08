package planet_attack.core;

import planet_attack.GUI;
import planet_attack.StdDraw;
import planet_attack.math.AABB;
import planet_attack.math.Polygon;
import planet_attack.math.Vector;

/**
 * Object to handle simple Newtonian mechanics. Controls position, velocity and
 * acceleration.
 */
public abstract class PhysicsObject {

	/**
	 * Mass of the object in kilogrammes.
	 */
	private double mass;

	/**
	 * Position (x,y) of the object in meters.
	 */
	private Vector pos;

	/**
	 * Velocity vector of the object in meters per
	 * {@link planet_attack.core.GameLoop#TICK_TIME tick}.
	 */
	private Vector vel;

	/**
	 * Acceleration vector of the object in meters per
	 * {@link planet_attack.core.GameLoop#TICK_TIME tick} squared.
	 */
	private Vector accel;

	/**
	 * The resulting force vector being applied to the object in Newtons
	 * [kg*m/({@link planet_attack.core.GameLoop#TICK_TIME tick}^2)].
	 */
	private Vector force;

	/**
	 * Rotation of the of the object in radians, [0;2pi].
	 */
	private double rot;
	private double rotVel;
	private Vector drag;
	private final double dragCoeff;

	/**
	 * AABB describing the size of the object. Used for collision detection.
	 */
	private AABB aabb;
	
	/**
	 * Polygon describing the size of the object. Used for collision detection.
	 */
	protected Polygon hitbox;

	private boolean exists;
	private boolean drawn;

	/**
	 * Value of the game loops {@link planet_attack.core.GameLoop#currentTick
	 * currentTick} last time this object was updated
	 */
	private int lastTick;

	/**
	 * Constructor for {@link PhysicsObject} that allows setting all parameters when
	 * the object is created.
	 * 
	 * @param mass      {@link PhysicsObject#mass}
	 * @param pos       {@link PhysicsObject#pos}
	 * @param vel       {@link PhysicsObject#vel}
	 * @param dragCoeff {@link PhysicsObject#dragCoeff}
	 * @param rot       {@link PhysicsObject#rot}
	 * @param rotVel    {@link PhysicsObject#rotVel}
	 * @param size   	Side length of the objects {@link planet_attack.math.AABB AABB}
	 */
	public PhysicsObject(double mass, Vector pos, Vector vel, double dragCoeff, double rot, double rotVel,
			double size) {
		this.mass = mass;
		this.pos = pos;
		this.vel = vel;
		this.force = new Vector();
		this.accel = new Vector();
		this.dragCoeff = dragCoeff;
		this.rot = rot;
		this.rotVel = rotVel;
		this.aabb = new AABB(pos.getX() - size / 2, pos.getY() - size / 2, size, size);
		this.hitbox = new Polygon(pos, size, size);
		this.exists = true;
		this.drawn = false;
		lastTick = 0;
	}

	/**
	 * Constructor for spawning a stationary object at a given position.
	 */
	public PhysicsObject(double mass, Vector pos, double dragCoeff, double rot, double size) {
		this(mass, pos, new Vector(), dragCoeff, rot, 0, size);
	}

	public void update(int currentTick) {
		if (exists) {
			// Create drag vector from velocity vector
			drag = new Vector(vel);
			/*
			 * Scale the drag vector by the negative drag coefficient, so that the force is
			 * opposite the direction we're currently moving, and then also scale it by its
			 * own magnitude (effectively velocity^2).
			 */
			drag.scale(-dragCoeff * drag.getMagnitude());
			// Apply drag
			applyForce(drag);
			// Create acceleration vector from force vector
			accel.set(force);
			accel.scale(1 / mass);
			vel.add(accel);
			pos.add(vel);
			aabb.move(vel);
			hitbox.offsetBy(vel);
			// TODO: do some similar stuff with torque and angular velocity
			rotate(rotVel);
			force.setZero();
			lastTick = currentTick;
			// TODO: Maybe check if it moved? Might also be fine as is, cause what if it suddenly goes off-camera and comes back in?
			drawn = false;
		}
	}

	public void applyForce(Vector force) {
		this.force.add(force);
	}

	public boolean isColliding(PhysicsObject other) {
		if (exists && other.exists) {
			//return aabb.isColliding(other.getAABB());
			return hitbox.isColliding(other.getHitbox());
		}
		return false;
	}

	public boolean isColliding(Polygon polygon) {
		return hitbox.isColliding(polygon);
	}
	
	public boolean isColliding(AABB aabb) {
		return hitbox.isColliding(aabb);
	}

	public void doCollision(PhysicsObject other) {
		this.doCollision();
		other.doCollision();
	}

	public abstract void doCollision();

	public void doElasticCollision(PhysicsObject other) {
		// TODO: this can probably be done in fewer lines...
		Vector v1 = this.getVel();
		Vector v2 = other.getVel();
		double m1 = this.getMass();
		double m2 = other.getMass();
		Vector x1 = this.getPos();
		Vector x2 = other.getPos();
		Vector dPos12 = new Vector(x1);
		dPos12.subtract(x2);
		Vector dPos21 = new Vector(x2);
		dPos21.subtract(x1);
		Vector dVel12 = new Vector(v1);
		dVel12.subtract(v2);
		Vector dVel21 = new Vector(v2);
		dVel21.subtract(v1);
		double scale1 = (2 * m2) / (m1 + m2) * dVel12.dot(dPos12) / Math.pow(x1.distance(x2), 2);
		double scale2 = (2 * m1) / (m1 + m2) * dVel21.dot(dPos21) / Math.pow(x2.distance(x1), 2);
		dPos12.scale(scale1);
		dPos21.scale(scale2);
		Vector v1New = new Vector(v1);
		Vector v2New = new Vector(v2);
		v1New.subtract(dPos12);
		v2New.subtract(dPos21);
		this.setVel(v1New);
		other.setVel(v2New);
	}

	public abstract void draw();
	
	public void draw(String texture, double width, double height, double rotation) {
		GUI.picture(getX(), getY(), texture, width, height, rotation);
		// getVelSI().draw(getPos(), StdDraw.RED);
		// getAccelSI().draw(getPos(), StdDraw.GREEN);
		if (GUI.DRAW_DEBUG) {
			aabb.draw(StdDraw.BLUE);
			GUI.text(getX(), getY() - 6, getPos().toString());
			hitbox.draw();
		}
	}
	
	public void draw(String texture, double width, double height) {
		draw(texture, width, height, getRotationDegrees());
	}

	public void draw(String texture, double size) {
		draw(texture, size, size);
	}

	public void rotate(double angle) {
		rot += angle;
		if (rot >= Math.PI * 2) {
			rot -= Math.PI * 2;
		} else if (rot < 0) {
			rot += Math.PI * 2;
		}
		hitbox.rotateBy(angle, pos);
	}

	public void rotateTo(double angle) {
		rotate(angle - rot);
		//rot = angle;
	}

	@Override
	public String toString() {
		return "pos [m] = " + pos + ", vel [m/tick] = " + vel + ", vel [m/s] = " + getVelSI()
				+ ", accel [m/(tick^2)] = " + accel + ", accel [m/(s^2)] = " + getAccelSI() + ", exists = " + exists;
	}

	public void setAABB(AABB aabb) {
		this.aabb = aabb;
	}

	public AABB getAABB() {
		return aabb;
	}
	
	public Polygon getHitbox() {
		return hitbox;
	}
	
	public void setPos(Vector pos) {
		this.pos = pos;
	}

	public Vector getPos() {
		return pos;
	}

	public double getX() {
		return pos.getX();
	}

	public double getY() {
		return pos.getY();
	}

	public void setExists(boolean exists) {
		this.exists = exists;
	}

	public boolean exists() {
		return exists;
	}

	public void setVel(Vector vel) {
		this.vel = vel;
	}

	public Vector getVel() {
		return vel;
	}

	public Vector getVelSI() {
		Vector velSI = new Vector(vel);
		velSI.scale(GameLoop.perTickToPerSecond(1));
		return velSI;
	}

	public double getVelX() {
		return vel.getX();
	}

	public double getVelY() {
		return vel.getY();
	}

	public void setVelX(double velX) {
		vel.setX(velX);
	}

	public void setVelY(double velY) {
		vel.setY(velY);
	}

	public void setAccel(Vector accel) {
		this.accel = accel;
	}

	public Vector getAccel() {
		return accel;
	}

	public Vector getAccelSI() {
		Vector accelSI = new Vector(accel);
		accelSI.scale(GameLoop.perTickSquaredToPerSecondSquared(1));
		return accelSI;
	}

	public double getRotation() {
		return rot;
	}

	public double getRotationDegrees() {
		return getRotation() * 180 / Math.PI;
	}

	public Vector getForce() {
		return force;
	}

	public double getMass() {
		return mass;
	}

	public int getLastTick() {
		return lastTick;
	}
	
	public boolean isDrawn() {
		return drawn;
	}
	
	public void setDrawn(boolean drawn) {
		this.drawn = drawn;
	}


}
