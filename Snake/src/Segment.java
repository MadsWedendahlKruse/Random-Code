import java.awt.Color;

public class Segment {
	
	private int posX;
	private int posY;
	
	public Segment(int posX, int posY) {
		this.posX = posX;
		this.posY = posY;
	}
	
	public void draw(Color color) {
		StdDraw.setPenColor(color);
		StdDraw.filledSquare(posX + 0.5, posY + 0.5, 0.48);
	}
	
	public void moveTo(int newX, int newY) {
		this.posX = newX;
		this.posY = newY;
	}
	
	public int getPosX() {
		return posX;
	}
	
	public int getPosY() {
		return posY;
	}
	
	public boolean collide(int x, int y) {
		return posX == x && posY == y;
	}
}
