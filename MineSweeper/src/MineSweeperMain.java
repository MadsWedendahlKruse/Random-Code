import java.awt.Point;
import java.awt.event.KeyEvent;
import java.util.ArrayList;

public class MineSweeperMain {
	
	// Mouse input samples
	public static boolean mouse1 = false;
	public static boolean mouse2 = false;
	
	// Key input samples
	public static char key1 = ' ';
	public static char key2 = ' ';
	
	// Used to automatically solve
	public static boolean solve = false;
	
	public static final int ELIMINATION_ATTEMPTS_MAX = 3;
	public static int eliminationAttempts = 0;
	
	public static ArrayList<MineSweeperState> states = new ArrayList<MineSweeperState>();
	
	public static final int WIDTH = 80;
	public static final int HEIGHT = 40;
	public static final int BOMB_COUNT = 400;
	public static final int SEED = 5;
	
	public static void main(String[] args) throws InterruptedException {
		
		MineSweeper mineSweeper = new MineSweeper(WIDTH, HEIGHT, BOMB_COUNT, SEED);
		
		states.add(mineSweeper.getCurrentState());
		
		long solveStartTime = -1;
		
		while (!mineSweeper.isGameOver() && !mineSweeper.isVictory()) {
			if (solve) {
				if (solveStartTime == -1) {
					System.out.println("Started solving...");
					solveStartTime = System.currentTimeMillis();
					mineSweeper.setHintsEnabled(true);
				}
				long getFlagsStartTime = System.currentTimeMillis();
				ArrayList<Point> bestFlag = mineSweeper.getBestFlags();
				System.out.println("Got best flags in " + (System.currentTimeMillis() - getFlagsStartTime) + " ms");
				if (mineSweeper.getMaxMineLikelihood() <= 10 && eliminationAttempts < ELIMINATION_ATTEMPTS_MAX) {
					long eliminateAllStartTime = System.currentTimeMillis();
					System.out.println("Eliminating all tiles...");
					mineSweeper.eliminateTiles(true);
					System.out.println("Eliminated all tiles in " + (System.currentTimeMillis() - eliminateAllStartTime) + " ms");
					eliminationAttempts++;
					continue;
				}
				if (eliminationAttempts == ELIMINATION_ATTEMPTS_MAX) {
					System.out.println("Elimination failed after " + eliminationAttempts + " attempts");
					eliminationAttempts = 0;
				}
				long clickFlagsStartTime = System.currentTimeMillis();
				for (Point point : bestFlag) {
					mineSweeper.mouseClicked(point.x, point.y, true);
					states.add(mineSweeper.getCurrentState());
				}
				System.out.println("Clicked flags in " + (System.currentTimeMillis() - clickFlagsStartTime) + " ms");
				long eliminateTilesTile = System.currentTimeMillis();
				mineSweeper.eliminateTiles();
				System.out.println("Eliminated tiles in " + (System.currentTimeMillis() - eliminateTilesTile) + " ms");
				states.add(mineSweeper.getCurrentState());
			} else {
				handleUserInput(mineSweeper);
			}
			mineSweeper.draw();
		}
		
		System.out.println("Finished solving in " + (System.currentTimeMillis() - solveStartTime) + " ms");
		
		int currentStateIdx = states.size()-1;
		while (!mineSweeper.isVictory()) {
			
			if (StdDraw.hasNextKeyTyped()) {
				key2 = StdDraw.nextKeyTyped();
			} else {
				key2 = ' ';
				key1 = key2;
			}
			if (key1 != key2) {
				if (StdDraw.isKeyPressed(KeyEvent.VK_A)) {
					currentStateIdx--;
					if (currentStateIdx < 0) {
						currentStateIdx = states.size() - 1;
					}
				}
				if (StdDraw.isKeyPressed(KeyEvent.VK_S)) {
					currentStateIdx = (currentStateIdx + 1) % states.size();
				}
			}
			key1 = key2;
			
			mineSweeper.loadState(states.get(currentStateIdx));
			mineSweeper.draw();
		}
		
	}

	private static void handleUserInput(MineSweeper mineSweeper) {
		if (StdDraw.hasNextKeyTyped()) {
			key2 = StdDraw.nextKeyTyped();
		} else {
			key2 = ' ';
			key1 = key2;
		}
		if (key1 != key2) {
			if (StdDraw.isKeyPressed(KeyEvent.VK_Z)) {
				mineSweeper.mouseClicked(StdDraw.mouseX(), StdDraw.mouseY(), true);
			}
			if (StdDraw.isKeyPressed(KeyEvent.VK_X)) {
				ArrayList<Point> bestFlag = mineSweeper.getBestFlags();
				mineSweeper.mouseClicked(bestFlag.get(0).x, bestFlag.get(0).y, true);
			}
			if (StdDraw.isKeyPressed(KeyEvent.VK_C)) {
				mineSweeper.eliminateTiles();
			}
			if (StdDraw.isKeyPressed(KeyEvent.VK_V)) {
				solve = !solve;
			}
			if (StdDraw.isKeyPressed(KeyEvent.VK_B)) {
				mineSweeper.setDebugMode(true);
			}
			if (StdDraw.isKeyPressed(KeyEvent.VK_H)) {
				mineSweeper.setHintsEnabled(!mineSweeper.isHintsEnabled());
			}
		}
		key1 = key2;
		
		mouse2 = StdDraw.mousePressed();
		if (StdDraw.mousePressed() && mouse1 != mouse2) {
			mineSweeper.mouseClicked(StdDraw.mouseX(), StdDraw.mouseY(), false);
		}
		mouse1 = mouse2;
		
	}
}
