
public class Vector {
	
	private Point pos;
	private Point dir;
	
	public Vector() {
		this.dir = new Point();
	}
	
	public Vector(double x, double y, double z) {
		this.dir = new Point(x, y, z);
		this.pos = new Point(0, 0, 0);
	}

	public Vector(Point dir) {
		this.pos = new Point(0, 0, 0);
		this.dir = dir;
	}
	
	public Vector(Point pos, Point dir) {
		this.pos = pos;
		this.dir = dir;
	}
	
	public Vector add(Vector other) {
		return new Vector(this.pos, this.dir.add(other.getDir()));
	}
	
	public Vector multiply(double scalar) {
		return new Vector(this.pos, this.dir.multiply(scalar));
	}
	
	public double getMagnitude() {
		return Math.sqrt(Math.pow(this.dir.getX(), 2) + Math.pow(this.dir.getY(), 2) + Math.pow(this.dir.getZ(), 2));
	}
	
	public Vector normalize() {
		return multiply(1/getMagnitude());
	}
	
	public double dot(Vector other) {
		return this.dir.getX() * other.getDir().getX() + 
				this.dir.getY() * other.getDir().getY() + 
				this.dir.getZ() * other.getDir().getZ();
	}
	
	public Point getDir() {
		return dir;
	}
	
	public Point getPos() {
		return pos;
	}
	
	public void setDir(Point dir) {
		this.dir = dir;
	}
	
	public void setPos(Point pos) {
		this.pos = pos;
	}

	public double getX() {
		return dir.getX();
	}
	
	public double getY() {
		return dir.getY();
	}
	
	public double getZ() {
		return dir.getZ();
	}
	
	public void setX(double x) {
		this.dir.setX(x);
	}
	
	public void setY(double x) {
		this.dir.setX(x);
	}
	
	public void setZ(double x) {
		this.dir.setX(x);
	}
	
	@Override
	public String toString() {
		return this.dir.toString();
	}
}
