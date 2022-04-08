import java.awt.Color;

public class Render3D {

	public static final int WINDOW_SIZE = 900;

	public static double X_MIN = -5;
	public static double X_MAX = 5;
	public static double Y_MIN = -5;
	public static double Y_MAX = 5;
	public static double Z_MIN = -5;
	public static double Z_MAX = 5;

	public static int AXES_RESOLUTION = 5;

	public static double ASPECT_RATIO = (X_MAX - X_MIN)/(Y_MAX - Y_MIN);

	// WRONG IDEA
	// Default perspective is looking at the (x,y)-plane.
	// Make sure this is always normalized.
	//public static Vector PERSPECTIVE = new Vector(new Point(0, 0, 1));
	
	//https://en.wikipedia.org/wiki/Rotation_matrix
	
	public static double YAW = 0;
	public static double PITCH = 0;
	public static double ROLL = 0;
	
	public static void main(String[] args) {

		StdDraw.setCanvasSize((int) (WINDOW_SIZE*ASPECT_RATIO), (int) (WINDOW_SIZE/ASPECT_RATIO));
		StdDraw.setXscale(X_MIN, X_MAX);
		StdDraw.setYscale(Y_MIN, Y_MAX);

		StdDraw.show(0);

		drawAxes();
		//drawAxesLabels();

		Point test1 = new Point(1, 2, 0);
		Point test2 = new Point(-2, 1, 0);
		Point test3 = new Point(-1, -2, 0);
		Point test4 = new Point(2, -1, 0);

		int steps = 100;

		double pointSize = 0.05;
		
		drawPoint3D(test1, StdDraw.RED, pointSize);
		drawPoint3D(test2, StdDraw.YELLOW, pointSize);
		drawPoint3D(test3, StdDraw.GREEN, pointSize);
		drawPoint3D(test4, StdDraw.BLUE, pointSize);
		
		/*
		// WRONG IDEA
		for (int i = 0; i <= steps; i++) {
			//StdDraw.show();
			StdDraw.clear();
			double n = (double) i;
			PERSPECTIVE = new Vector(new Point(n/steps, 1/Math.sqrt(3), (double) (1-n/steps)));
			PERSPECTIVE = PERSPECTIVE.normalize();
			//System.out.println("Perspective = " + PERSPECTIVE);
			drawAxes();
			drawPoint3D(test1, StdDraw.RED, pointSize);
			drawPoint3D(test2, StdDraw.YELLOW, pointSize);
			drawPoint3D(test3, StdDraw.GREEN, pointSize);
			drawPoint3D(test4, StdDraw.BLUE, pointSize);
			StdDraw.show(50);
		}
		 */
		
		Matrix testMatrix = new Matrix(3, 3);
		
		testMatrix.setValue(0, 0, 1);
		testMatrix.setValue(0, 1, 4);
		testMatrix.setValue(0, 2, 4);
		
		testMatrix.setValue(1, 0, 1);
		testMatrix.setValue(1, 1, 2);
		testMatrix.setValue(1, 2, 3);
		
		testMatrix.setValue(2, 0, 4);
		testMatrix.setValue(2, 1, 3);
		testMatrix.setValue(2, 2, 1);
		
		Vector testVector = new Vector(1, 2, 3);
		Vector dotTest = testMatrix.dot(testVector);
		
		System.out.println("testVector = " + testVector);
		System.out.println("dotTest = " + dotTest);
		
		int frameLength = 5;
		
		for (int i = 0; i <= steps; i++) {
			StdDraw.clear();
			double n = (double) i;
			YAW = n/steps*Math.PI/4;
			drawAxes();
			drawPoint3D(test1, StdDraw.RED, pointSize);
			drawPoint3D(test2, StdDraw.YELLOW, pointSize);
			drawPoint3D(test3, StdDraw.GREEN, pointSize);
			drawPoint3D(test4, StdDraw.BLUE, pointSize);
			StdDraw.show(frameLength);
		}
		
		for (int i = 0; i <= steps; i++) {
			StdDraw.clear();
			double n = (double) i;
			ROLL = n/steps*Math.PI/4;
			drawAxes();
			drawPoint3D(test1, StdDraw.RED, pointSize);
			drawPoint3D(test2, StdDraw.YELLOW, pointSize);
			drawPoint3D(test3, StdDraw.GREEN, pointSize);
			drawPoint3D(test4, StdDraw.BLUE, pointSize);
			StdDraw.show(frameLength);
		}
		
		for (int i = 0; i <= steps; i++) {
			StdDraw.clear();
			double n = (double) i;
			PITCH = n/steps*2*Math.PI/6;
			drawAxes();
			drawPoint3D(test1, StdDraw.RED, pointSize);
			drawPoint3D(test2, StdDraw.YELLOW, pointSize);
			drawPoint3D(test3, StdDraw.GREEN, pointSize);
			drawPoint3D(test4, StdDraw.BLUE, pointSize);
			StdDraw.show(frameLength);
		}
		
		StdDraw.show(0);

	}

	/*
	// WRONG IDEA
	public static Point projectPoint3D(Point point) {
		Vector v = new Vector(new Point(point.getX(), point.getY(), point.getZ()));

		Point point3D = v.add(PERSPECTIVE.multiply(-PERSPECTIVE.dot(v))).getDir();

		//Point point3D = point.add(PERSPECTIVE.normalize().multiply(-v.dot(PERSPECTIVE.normalize())).getDir());
		return point3D;
	}
	 */

	public static Point getPoint3D(Point point) {
		
		Matrix rotationMatrix = new Matrix(3, 3);
		
		rotationMatrix.setValue(0, 0, Math.cos(YAW) * Math.cos(PITCH));
		rotationMatrix.setValue(0, 1, Math.cos(YAW) * Math.sin(PITCH) * Math.sin(ROLL) - Math.sin(YAW) * Math.cos(ROLL));
		rotationMatrix.setValue(0, 2, Math.cos(YAW) * Math.sin(PITCH) * Math.cos(ROLL) + Math.sin(YAW) * Math.sin(ROLL));
		
		rotationMatrix.setValue(1, 0, Math.sin(YAW) * Math.cos(PITCH));
		rotationMatrix.setValue(1, 1, Math.sin(YAW) * Math.sin(PITCH) * Math.sin(ROLL) + Math.cos(YAW) * Math.cos(ROLL));
		rotationMatrix.setValue(1, 2, Math.sin(YAW) * Math.sin(PITCH) * Math.cos(ROLL) - Math.cos(YAW) * Math.sin(ROLL));
		
		rotationMatrix.setValue(2, 0, -Math.sin(PITCH));
		rotationMatrix.setValue(2, 1, Math.cos(PITCH) * Math.sin(ROLL));
		rotationMatrix.setValue(2, 2, Math.cos(PITCH) * Math.cos(ROLL));
		
		return rotationMatrix.dot(new Vector(point)).getDir();
	}

	public static void drawPoint3D(Point point) {
		Point point3D = getPoint3D(point);
		StdDraw.setPenColor(StdDraw.RED);
		StdDraw.setPenRadius(0.01);
		StdDraw.point(point3D.getX(), point3D.getY());
	}

	public static void drawPoint3D(Point point, Color color, double size) {
		Point point3D = getPoint3D(point);
		StdDraw.setPenColor(color);
		StdDraw.setPenRadius(size);
		StdDraw.point(point3D.getX(), point3D.getY());
	}

	public static void drawLine3D(Point start, Point end) {
		Point start3D = getPoint3D(start);
		Point end3D = getPoint3D(end);
		StdDraw.setPenColor(StdDraw.BLACK);
		StdDraw.setPenRadius();
		StdDraw.line(start3D.getX(), start3D.getY(), end3D.getX(), end3D.getY());
	}

	public static void drawLine3D(Point start, Point end, Color color, double size) {
		Point start3D = getPoint3D(start);
		Point end3D = getPoint3D(end);
		StdDraw.setPenColor(color);
		StdDraw.setPenRadius(size);
		StdDraw.line(start3D.getX(), start3D.getY(), end3D.getX(), end3D.getY());
	}

	public static void drawAxes() {

		double axisThickness = 0.004;

		StdDraw.setPenColor(StdDraw.BLACK);
		StdDraw.setPenRadius();

		// x-axis
		Point xStart = new Point(X_MIN, 0, 0);
		Point xEnd = new Point(X_MAX, 0, 0);
		drawLine3D(xStart, xEnd, StdDraw.RED, axisThickness);

		// y-axis
		Point yStart = new Point(0, Y_MIN, 0);
		Point yEnd = new Point(0, Y_MAX, 0);
		drawLine3D(yStart, yEnd, StdDraw.GREEN, axisThickness);

		// z-axis
		Point zStart = new Point(0, 0, Z_MIN);
		Point zEnd = new Point(0, 0, Z_MAX);
		drawLine3D(zStart, zEnd, StdDraw.BLUE, axisThickness);
	}

	public static void drawAxesLabels() {

		StdDraw.setPenColor(StdDraw.BLACK);
		StdDraw.setPenRadius(0.01);

		for (int i = 1; i <= AXES_RESOLUTION; i++) {
			StdDraw.point(X_MAX/AXES_RESOLUTION*i, 0);
			StdDraw.text(X_MAX/AXES_RESOLUTION*i, -Y_MAX/20, X_MAX/AXES_RESOLUTION*i + "");
			StdDraw.point(0, Y_MAX/AXES_RESOLUTION*i);
			StdDraw.text(-X_MAX/20, Y_MAX/AXES_RESOLUTION*i, Y_MAX/AXES_RESOLUTION*i + "");
		}
	}

}
