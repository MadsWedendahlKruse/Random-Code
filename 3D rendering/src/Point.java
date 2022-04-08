
public class Point {
	
	private double x;
	private double y;
	private double z;
	
	public Point() {
		this.x = 0;
		this.y = 0;
		this.z = 0;
	}
	
	public Point(double x, double y) {
		this.x = x;
		this.y = y;
		this.z = 0;
	}
	
	public Point(double x, double y, double z) {
		this.x = x;
		this.y = y;
		this.z = z;
	}
	
	public Point add(double x1, double y1, double z1) {
		return new Point(this.x + x1, this.y + y1, this.z + z1);
	}
	
	public Point add(Point other) {
		return new Point(this.x + other.getX(), this.y + other.getY(), this.z + other.getZ());
	}
	
	public void moveTo(double xNew, double yNew, double zNew) {
		this.x = xNew;
		this.y = yNew;
		this.z = zNew;
	}
	
	public void moveTo(Point destination) {
		this.x = destination.getX();
		this.y = destination.getY();
		this.z = destination.getZ();
	}
	
	// Mathematically this might make more sense to be attached
	// to the Vector object
	public Point multiply(double scalar) {
		return new Point(this.x * scalar, this.y * scalar, this.z * scalar);
	}
	
	public double getX() {
		return x;
	}
	
	public double getY() {
		return y;
	}
	
	public double getZ() {
		return z;
	}
	
	public void setX(double x) {
		this.x = x;
	}
	
	public void setY(double y) {
		this.y = y;
	}
	
	public void setZ(double z) {
		this.z = z;
	}
	
	@Override
	public String toString() {
		return "(" + this.getX() + ", " + this.getY() + ", " + this.getZ() + ")";
	}
}
