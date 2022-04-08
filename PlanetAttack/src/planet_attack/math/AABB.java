package planet_attack.math;

import java.awt.Color;

import planet_attack.GUI;
import planet_attack.StdDraw;

/**
 * A simple Axis-Aligned Bounding Box class.
 */
public class AABB {

	private Vector minPoint;
	private Vector maxPoint;
	
	public AABB(double x, double y, double ex, double ey) {
		this.minPoint = new Vector(x, y);
		this.maxPoint = new Vector(x + ex, y + ey);
	}
	
	public AABB(Vector minPoint, Vector maxPoint) {
		this(minPoint.getX(), minPoint.getY(), maxPoint.getX() - minPoint.getX(), maxPoint.getY() - minPoint.getY());
	}
	
	public AABB(AABB other) {
		this(other.getMinPoint(), other.getMaxPoint());
	}

	public void move(Vector offset) {
		minPoint.add(offset);
		maxPoint.add(offset);
	}

	public void moveTo(Vector position) {
		Vector extents = getExtents();
		minPoint.setX(position.getX());
		minPoint.setY(position.getY());
		maxPoint.setX(position.getX() + extents.getX());
		maxPoint.setY(position.getY() + extents.getY());
	}
	
	public void moveCentreTo(Vector position) {
		Vector extents = getExtents();
		minPoint.setX(position.getX() - extents.getX() / 2);
		minPoint.setY(position.getY() - extents.getY() / 2);
		maxPoint.setX(position.getX() + extents.getX() / 2);
		maxPoint.setY(position.getY() + extents.getY() / 2);
	}
	
	/**
	 * @param other Other AABB to check for collision
	 * @return Whether or not the two AABBs are colliding
	 */
	public boolean isColliding(AABB other) {
		return minPoint.getX() < other.getMaxPoint().getX() && maxPoint.getX() > other.getMinPoint().getX()
				&& minPoint.getY() < other.getMaxPoint().getY() && maxPoint.getY() > other.getMinPoint().getY();
	}

	/**
	 * @param point point to check for collision
	 * @return Whether or not the point is inside the AABB
	 */
	public boolean isPointWithin(Vector point) {
		return minPoint.getX() < point.getX() && maxPoint.getX() > point.getX()
				&& minPoint.getY() < point.getY() && maxPoint.getY() > point.getY();
	}
	
	public boolean isInside(AABB other) {
		return minPoint.getX() >= other.getMinPoint().getX() && maxPoint.getX() <= other.getMaxPoint().getX()
				&& minPoint.getY() >= other.getMinPoint().getY() && maxPoint.getY() <= other.getMaxPoint().getY();
	}
	
	public void setMaxPoint(Vector maxPoint) {
		this.maxPoint = maxPoint;
	}

	public void setMinPoint(Vector minPoint) {
		this.minPoint = minPoint;
	}

	public Vector getMaxPoint() {
		return maxPoint;
	}

	public Vector getMinPoint() {
		return minPoint;
	}
	
	public Vector getExtents() {
		return new Vector(maxPoint.getX() - minPoint.getX(), maxPoint.getY() - minPoint.getY());
	}
	
	public Vector getCenter() {
		Vector extents = getExtents();
		return new Vector(minPoint.getX() + extents.getX()/2, minPoint.getY() + extents.getY()/2);
	}
	
	public Vector[] getCorners() {
		Vector[] corners = new Vector[4];
		Vector extents = getExtents();
		for (int i = 0; i < corners.length; i++) {
			switch (i) {
			// Top left
			case 0:
				corners[i] = new Vector(maxPoint.getX() - extents.getX(), maxPoint.getY());
				break;
			// Top right
			case 1:
				corners[i] = new Vector(maxPoint);
				break;
			// Bottom left
			case 2:
				corners[i] = new Vector(minPoint);
				break;
			// Bottom right
			case 3:
				corners[i] = new Vector(minPoint.getX() + extents.getX(), minPoint.getY());
				break;
			default:
				break;
			}
		}
		return corners;
	}
	
	public void draw() {
		draw(StdDraw.LIGHT_GRAY);
	}
	
	public void draw(Color color) {
		Vector extents = getExtents();
		Vector center = getCenter();
		StdDraw.setPenColor(color);
		GUI.rectangle(center.getX(), center.getY(), extents.getX()/2, extents.getY()/2);
	}
	
	@Override
	public String toString() {
		return "minPoint = " + minPoint + ", maxPoint = " + maxPoint;
	}

}
