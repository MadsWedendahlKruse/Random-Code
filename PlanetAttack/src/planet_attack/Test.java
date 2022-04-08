package planet_attack;

import java.util.ArrayList;
import java.util.Iterator;

import planet_attack.core.PhysicsObject;
import planet_attack.math.Polygon;
import planet_attack.math.Vector;

public class Test {
	
	public static void main(String[] args) {
		
		Vector playerPos = new Vector(400.8763, 434.5371);
		Vector targetPos = new Vector(352, 416);
		System.out.println("playerPos = " + playerPos);
		System.out.println("targetPos = " + targetPos);
		double dx = targetPos.getX() - playerPos.getX();
		double dy = targetPos.getY() - playerPos.getY();
		Vector diff = new Vector(dx, dy);
		System.out.println("diff = " + diff);
		System.out.println("diff rotation = " + diff.getRotation());
		Vector playerFacing = Vector.getVectorWithRotation(Math.PI, 1);
		System.out.println("player rotation = " + playerFacing.getRotation());
		double rotDiff = diff.getRotation() - playerFacing.getRotation();
		rotDiff += (rotDiff > Math.PI) ? -2*Math.PI : (rotDiff < -Math.PI) ? 2*Math.PI : 0;
		System.out.println("rotDiff = " + rotDiff);
		System.out.println("rotDiff + 2*PI = " + (rotDiff + 2*Math.PI));
		playerFacing.rotateTo(diff.getRotation());
		System.out.println("player rotation after rotating = " + playerFacing.getRotation());
		
		System.out.println("\n------------------------------------------------------\n");
		
		ArrayList<Integer> arrayList1 = new ArrayList<Integer>();
		ArrayList<Integer> arrayList2 = new ArrayList<Integer>();
		System.out.println("arrayList1 = " + arrayList1);
		System.out.println("arrayList2 = " + arrayList2);
		System.out.println("Put some numbers in...");
		Integer test = new Integer(5);
		arrayList1.add(test);
		arrayList2.add(test);
		System.out.println("arrayList1 = " + arrayList1);
		System.out.println("arrayList2 = " + arrayList2);
		System.out.println("Remove the number...");
		for (Iterator<Integer> iterator = arrayList1.iterator(); iterator.hasNext();) {
			Integer integer = (Integer) iterator.next();
			if (integer.intValue() == 5) {
				iterator.remove();
			}
		}
		for (Iterator<Integer> iterator = arrayList2.iterator(); iterator.hasNext();) {
			Integer integer = (Integer) iterator.next();
			if (integer.intValue() == 5) {
				iterator.remove();
			}
		}
		System.out.println("arrayList1 = " + arrayList1);
		System.out.println("arrayList2 = " + arrayList2);
		
		System.out.println("\n------------------------------------------------------\n");
		
		Vector[] verticesA = {new Vector(13, 10), new Vector(13, 3), new Vector(6, 3), new Vector(6, 10)};
		Vector[] edgesA = {new Vector(0, -7), new Vector(-7, 0), new Vector(0, 7), new Vector(7, 0)};
		Vector[] verticesB = {new Vector(14, 18), new Vector(15, 11), new Vector(10, 13)};
		Vector[] edgesB = {new Vector(1, -7), new Vector(-5, 2), new Vector(4, 5)};
		Polygon polygonA = new Polygon(verticesA);
		Polygon polygonB = new Polygon(verticesB);
		System.out.println(polygonA.isColliding(polygonB));
		
		Vector[] verticesA2 = {new Vector(11, 10), new Vector(11, 3), new Vector(4, 3), new Vector(4, 10)};
		Vector[] edgesA2 = {new Vector(0, -7), new Vector(-7, 0), new Vector(0, 7), new Vector(7, 0)};
		Vector[] verticesB2 = {new Vector(13, 13), new Vector(8, 9), new Vector(7, 15)};
		Vector[] edgesB2 = {new Vector(-5, -4), new Vector(-1, 6), new Vector(6, -2)};
		Polygon polygonA2 = new Polygon(verticesA2);
		Polygon polygonB2 = new Polygon(verticesB2);
		System.out.println(polygonA2.isColliding(polygonB2));
	}
}
