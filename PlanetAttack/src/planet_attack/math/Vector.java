package planet_attack.math;

import java.awt.Color;

import planet_attack.GUI;
import planet_attack.StdDraw;

/**
 * A simple vector class. This probably already exists though...
 */
public class Vector {

	private double x;
	private double y;

	public Vector() {
		this(0, 0);
	}

	public Vector(double x, double y) {
		this.x = x;
		this.y = y;
	}

	public Vector(Vector other) {
		this(other.getX(), other.getY());
	}

	public double getMagnitude() {
		return Math.sqrt(x * x + y * y);
	}
	
	/*
	 * Less computationally demanding
	 */
	public double getMagnitudeSquared() {
		return x * x + y * y;
	}

	public Vector getUnitVector() {
		Vector unitVector = new Vector(x, y);
		double magnitude = getMagnitude();
		// Avoid division by zero
		if (magnitude > 0) {
			unitVector.scale(1 / getMagnitude());
			return unitVector;
		}
		return new Vector();
	}

	public static Vector getVectorWithRotation(double rotation, double magnitude) {
		Vector vector = new Vector(Math.cos(rotation), Math.sin(rotation));
		vector.scale(magnitude);
		return vector;
	}

	public void add(double x, double y) {
		this.x += x;
		this.y += y;
	}

	public void add(Vector v) {
		this.x += v.getX();
		this.y += v.getY();
	}

	public void subtract(Vector v) {
		this.x -= v.getX();
		this.y -= v.getY();
	}

	public void scale(double scalar) {
		this.x *= scalar;
		this.y *= scalar;
	}

	public double dot(Vector v) {
		return this.x * v.x + this.y * v.y;
	}

	public double distance(Vector v) {
		return Math.sqrt(Math.pow(this.x - v.x, 2) + Math.pow(this.y - v.y, 2));
	}

	public double getRotation() {
		return Math.atan2(y, x);
	}
	
	/**
	 * Rotates the vector by the given angle
	 */
	public void rotateBy(double rads) {
		double tempX = x;
		this.x = x * Math.cos(rads) - y * Math.sin(rads);
		this.y = tempX * Math.sin(rads) + y * Math.cos(rads);
	}
	
	/**
	 * Rotates the vector to the target angle
	 */
	public void rotateTo(double targetRads) {
		rotateBy(targetRads - getRotation());
	}

	/**
	 * Rotates the vector towards the target angle by an angle at most equal to the
	 * step size
	 */
	public void rotateTo(double targetRads, double stepSize) {
		double rotDiff = targetRads - getRotation();
		rotDiff += (rotDiff > Math.PI) ? -2 * Math.PI : (rotDiff < -Math.PI) ? 2 * Math.PI : 0;
		if (rotDiff > 0) {
			rotateBy(Math.min(rotDiff, stepSize));
		} else {
			rotateBy(Math.max(rotDiff, -stepSize));
		}
	}

	@Override
	public String toString() {
		return String.format("(%.4f, %.4f)", x, y);
	}

	public void setX(double x) {
		this.x = x;
	}

	public void setY(double y) {
		this.y = y;
	}

	public double getX() {
		return x;
	}

	public double getY() {
		return y;
	}

	public void setZero() {
		x = y = 0;
	}

	public void set(Vector vector) {
		x = vector.getX();
		y = vector.getY();
	}

	public void draw(Vector pos, Color color) {
		StdDraw.setPenColor(color);
		GUI.line(pos.getX(), pos.getY(), pos.getX() + this.getX(), pos.getY() + this.getY());
	}
}
