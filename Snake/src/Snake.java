import java.awt.Color;
import java.awt.event.KeyEvent;
import java.util.ArrayList;

public class Snake {
	
	private int velX = 0;
	private int velY = 0;
	
	private ArrayList<Segment> segments = new ArrayList<Segment>();
	
	public Snake(int startX, int startY) {
		segments.add(new Segment(startX, startY));
	}
	
	public void move() {
		Segment front = segments.get(0);
		int backIndex = segments.size()-1;
		Segment back = segments.get(backIndex);
		back.moveTo(front.getPosX() + velX, front.getPosY() + velY);
		segments.remove(backIndex);
		segments.add(0, back);
	}
	
	public void update(int key) {
		
		switch (key) {
		case 'w':
			if (velY != -1) {
				velY =1;
			}
			velX = 0;
			break;
		case 's':
			if (velY != 1) {
				velY = -1;
			}
			velX = 0;
			break;
		case 'a':
			velY = 0;
			if (velX != 1) {
				velX = -1;
			}
			break;
		case 'd':
			velY = 0;
			if (velX != -1) {
				velX = 1;
			}
			break;
		default:
			break;
		}
	}
	
	public void draw(Color color) {
		for (int i = 0; i < segments.size(); i++) {
			segments.get(i).draw(color);
		}
	}
	
	public void addSegment(int amount) {
		Segment back = segments.get(segments.size()-1);
		for (int i = 0; i < amount; i++) {
			segments.add(new Segment(back.getPosX(), back.getPosY()));
		}
	}
	
	public boolean headCollide(int x, int y) {
		return segments.get(0).collide(x, y);
	}
	
	public boolean collide(int x, int y) {
		for (int i = 0; i < segments.size(); i++) {
			if (segments.get(i).collide(x, y)) {
				return true;
			}
		}
		return false;
	}
	
	public boolean collideSelf() {
		Segment front = segments.get(0);
		for (int i = 1; i < segments.size(); i++) {
			if (segments.get(i).collide(front.getPosX(), front.getPosY())) {
				return true;
			}
		}
		return false;
	}
	
	public ArrayList<Segment> getSegments() {
		return segments;
	}
	
	public Segment getFront() {
		return segments.get(0);
	}
}
