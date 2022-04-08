import java.util.ArrayList;
import java.util.Random;

public class SnakeMain {
	
	public static final int WINDOW_SIZE = 900;

	public static int X_MIN = -15;
	public static int X_MAX = 15;
	public static int Y_MIN = -15;
	public static int Y_MAX = 15;

	public static double ASPECT_RATIO = (double) (X_MAX - X_MIN)/(Y_MAX - Y_MIN);
	
	public static final int SPEED = 1; //Less is more

	public static ArrayList<Segment> food = new ArrayList<Segment>();
	public static final int FOOD_AMOUNT = 1;
	public static final int NUTRITION = 5;
	
	public static Snake snake = new Snake(0, 0);
	
	// Key input samples
	public static char key1 = ' ';
	public static char key2 = ' ';
	
	public static void main(String[] args) {

		initCanvas();
		
		while (true) {
			
			long startTime = System.currentTimeMillis();
			
			sampleKeys();
			if (key1 != key2) {
				snake.update(key2);
			}
			key1 = key2;
			
			if (food.size() < FOOD_AMOUNT) {
				for (int i = food.size(); i < FOOD_AMOUNT; i++) {
					addFood();
				}
			}
			
			snake.move();
			
			if (snake.collideSelf() || 
					snake.getFront().getPosX() > X_MAX || snake.getFront().getPosX() < X_MIN || 
					snake.getFront().getPosY() > Y_MAX || snake.getFront().getPosY() < Y_MIN ) {
				System.out.println("GAME OVER");
				break;
			}
			
			for (int i = 0; i < food.size(); i++) {
				if (snake.headCollide(food.get(i).getPosX(), food.get(i).getPosY())) {
					snake.addSegment(NUTRITION);
					food.remove(i);
				}
			}
			draw();
			
			long endTime = System.currentTimeMillis();
			
			StdDraw.show(Math.max(SPEED - (int) (endTime - startTime), 0));
			
			//System.out.println(System.currentTimeMillis() - startTime);
		}
	}

	private static void sampleKeys() {
		if (StdDraw.hasNextKeyTyped()) {
			key2 = StdDraw.nextKeyTyped();
		} else {
			key2 = ' ';
			key1 = key2;
		}
	}
	
	private static void initCanvas() {
		StdDraw.show(0);
		StdDraw.setCanvasSize((int) (WINDOW_SIZE*ASPECT_RATIO), (int) (WINDOW_SIZE));
		StdDraw.setXscale(X_MIN, X_MAX);
		StdDraw.setYscale(Y_MIN, Y_MAX);
	}
	
	private static void drawGrid() {
		StdDraw.setPenColor(StdDraw.LIGHT_GRAY);
		for (int i = X_MIN; i <= X_MAX; i++) {
			for (int j = Y_MIN; j < Y_MAX; j++) {		
				StdDraw.line(i, Y_MIN, i, Y_MAX);
				StdDraw.line(X_MIN, j, X_MAX, j);
			}
		}
	}
	
	private static void draw() {
		StdDraw.clear();
		snake.draw(StdDraw.BLUE);
		drawGrid();
		for (Segment food : food) {
			food.draw(StdDraw.YELLOW);
		}
	}
	
	private static void addFood() {
		Random rand = new Random();
		while (true) {
			int x = rand.nextInt(X_MAX - X_MIN);
			x += X_MIN;
			int y = rand.nextInt(Y_MAX - Y_MIN);
			y += Y_MIN;
			if (!snake.collide(x, y)) {
				food.add(new Segment(x, y));
				break;
			}
		}
	}
}
