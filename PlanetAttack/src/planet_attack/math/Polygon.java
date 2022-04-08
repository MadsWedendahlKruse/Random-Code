package planet_attack.math;

import java.awt.Color;
import java.util.ArrayList;
import java.util.Arrays;

import planet_attack.GUI;
import planet_attack.StdDraw;

/**
 * All collision code is based on
 * http://programmerart.weebly.com/separating-axis-theorem.html
 */
public class Polygon {

	Vector[] vertices;
	Vector[] edges;
	Vector center;

	public Polygon(Vector[] vertices) {
		this.vertices = vertices;
		edges = calculateEdges(vertices);
		center = calculateCenter(vertices);
	}

	// Rectangular polygon
	public Polygon(Vector center, double width, double height) {
		vertices = new Vector[4];
		vertices[0] = new Vector(center.getX() - width / 2, center.getY() + height / 2);
		vertices[1] = new Vector(center.getX() + width / 2, center.getY() + height / 2);
		vertices[2] = new Vector(center.getX() + width / 2, center.getY() - height / 2);
		vertices[3] = new Vector(center.getX() - width / 2, center.getY() - height / 2);
		edges = calculateEdges(vertices);
		this.center = new Vector(center);
	}
	
	public Polygon(Vector center, int sides, double diameter) {
		vertices = new Vector[sides];
		double vertexAngle = 2*Math.PI/sides;
		for (int i = 0; i < vertices.length; i++) {
			vertices[i] = new Vector(diameter/2 * Math.cos(vertexAngle*i) + center.getX(), diameter/2 * Math.sin(vertexAngle*i) + center.getY());
		}
		this.edges = calculateEdges(vertices);
		this.center = calculateCenter(vertices);
	}
	
	public Polygon(AABB aabb) {
		this(aabb.getCenter(), aabb.getExtents().getX(), aabb.getExtents().getY());
	}

	public static Vector[] calculateEdges(Vector[] vertices) {
		Vector[] edges = new Vector[vertices.length];
		for (int i = 0; i < vertices.length; i++) {
			int j = i + 1;
			if (i == vertices.length - 1) {
				j = 0;
			}
			edges[i] = new Vector(vertices[i].getX() - vertices[j].getX(), vertices[i].getY() - vertices[j].getY());
		}
		return edges;
	}
	
	public static Vector calculateCenter(Vector[] vertices) {
		double centerX = 0, centerY = 0;
		for (Vector vertex : vertices) {
			centerX += vertex.getX();
			centerY += vertex.getY();
		}
		return new Vector(centerX/vertices.length, centerY/vertices.length);
	}

	public boolean isColliding(Polygon other) {
		ArrayList<Vector> perpendicularLines = new ArrayList<Vector>();
		for (int i = 0; i < this.edges.length; i++) {
			perpendicularLines.add(new Vector(-this.edges[i].getY(), this.edges[i].getX()));
		}
		for (int i = 0; i < other.edges.length; i++) {
			perpendicularLines.add(new Vector(-other.edges[i].getY(), other.edges[i].getX()));
		}
		for (int i = 0; i < perpendicularLines.size(); i++) {
			double amin = Integer.MAX_VALUE;
			double amax = Integer.MIN_VALUE;
			double bmin = Integer.MAX_VALUE;
			double bmax = Integer.MIN_VALUE;
			double dot;
			for (int j = 0; j < this.vertices.length; j++) {
				dot = perpendicularLines.get(i).dot(this.vertices[j]);
				if (dot < amin) {
					amin = dot;
				}
				if (dot > amax) {
					amax = dot;
				}
			}
			for (int j = 0; j < other.vertices.length; j++) {
				dot = perpendicularLines.get(i).dot(other.vertices[j]);
				if (dot < bmin) {
					bmin = dot;
				}
				if (dot > bmax) {
					bmax = dot;
				}
			}
			if ((amin < bmax && amin > bmin) || (bmin < amax && bmin > amin)) {
				continue;
			} else {
				return false;
			}
		}
		return true;
	}
	
	public boolean isColliding(AABB aabb) {
		Polygon aabbPoly = new Polygon(aabb);
		//System.out.println("aabb = " + aabb + ", aabbPoly = " + aabbPoly);
		return this.isColliding(aabbPoly);
	}

	public void offsetBy(Vector offset) {
		for (int i = 0; i < vertices.length; i++) {
			vertices[i].add(offset);
		}
		center.add(offset);
	}

	/**
	 * Rotate by a given amount (rads) around a given point
	 */
	public void rotateBy(double rads, Vector point) {
		for (int i = 0; i < vertices.length; i++) {
			double tempVertX = vertices[i].getX() - point.getX();
			double tempVertY = vertices[i].getY() - point.getY();
			vertices[i].setX((tempVertX * Math.cos(rads) - tempVertY * Math.sin(rads)) + point.getX());
			vertices[i].setY((tempVertX * Math.sin(rads) + tempVertY * Math.cos(rads)) + point.getY());
		}
		edges = calculateEdges(vertices);
		double tempCentX = center.getX() - point.getX();
		double tempCentY = center.getY() - point.getY();
		center.setX((tempCentX * Math.cos(rads) - tempCentY * Math.sin(rads)) + point.getX());
		center.setY((tempCentX * Math.sin(rads) + tempCentY * Math.cos(rads)) + point.getY());
	}

	/**
	 * Rotate by a given amount around the polygons center (only work in the center
	 * is not null)
	 */
	public void rotateBy(double rads) {
		for (int i = 0; i < vertices.length; i++) {
			double tempVertX = vertices[i].getX() - center.getX();
			double tempVertY = vertices[i].getY() - center.getY();
			vertices[i].setX((tempVertX * Math.cos(rads) - tempVertY * Math.sin(rads)) + center.getX());
			vertices[i].setY((tempVertX * Math.sin(rads) + tempVertY * Math.cos(rads)) + center.getY());
		}
		edges = calculateEdges(vertices);
	}

	/**
	 * Moves the center to the given position
	 */
	public void moveTo(Vector pos) {
		Vector offset = new Vector(pos);
		offset.subtract(center);
		offsetBy(offset);
	}

	public void draw() {
		draw(StdDraw.ORANGE);
	}

	public void draw(Color color) {
		StdDraw.setPenColor(color);
		for (int i = 0; i < vertices.length; i++) {
			GUI.line(vertices[i].getX(), vertices[i].getY(), vertices[i].getX() - edges[i].getX(),
					vertices[i].getY() - edges[i].getY());
		}
	}
	
	public Vector getCenter() {
		return center;
	}
	
	@Override
	public String toString() {
		return Arrays.toString(vertices);
	}
}
