import java.awt.Color;
import java.awt.Font;
import java.awt.Point;
import java.util.ArrayList;
import java.util.Random;

public class MineSweeper {

	public static final int WINDOW_SIZE = 900;

	private boolean[][] isMine;
	private boolean[][] isFlagged;
	private boolean[][] isVisible;
	private int[][] mineNeighbours;
	private int[][] mineLikelihood;
	private int maxMineLikelihood;
	private int width;
	private int height;
	private int mineCount;
	private boolean gameOver = false;
	private boolean victory = false;
	private int floodThreshold = 1000;
	private boolean firstClick = true;
	private Point gameOverMine;
	private Font font;
	
	/**
	 * Tracks the points that were clicked since eliminateTiles() was called
	 */
	private ArrayList<Point> clickedPoints = new ArrayList<Point>();
	
	// Stuff for StdDraw
	private boolean isCanvasInit = false;
	private double aspectRatio;

	private boolean debugMode = false;
	private boolean hintsEnabled = false;
	
	public MineSweeper(int width, int height, int mineCount) {
		this(width, height, mineCount, -1);
	}

	public MineSweeper(int width, int height, int mineCount, int seed) {
		this.width = width;
		this.height = height;
		this.mineCount = mineCount;
		isMine = new boolean[width][height];
		isFlagged = new boolean[width][height];
		isVisible = new boolean[width][height];
		Random random = new Random();
		if (seed != -1) {
			random = new Random(seed);
		}
		int minesSoFar = 0;
		while (minesSoFar < mineCount) {
			int mineX = random.nextInt(width);
			int mineY = random.nextInt(height);
			if (!isMine[mineX][mineY]) {
				isMine[mineX][mineY] = true;
				minesSoFar++;
			}
		}
		mineNeighbours = new int[width][height];
		mineLikelihood = new int[width][height];
		aspectRatio = (double) width / height;
		int fontSize = 500 / Math.max(width, height);
		font = new Font("Arial", Font.BOLD, fontSize);
		StdDraw.setFont(font);
	}

	private void calculateNeighbourCounts() {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				if (isMine[x][y]) {
					continue;
				}
				countNeighbourMines(x, y);
			}
		}
	}

	private void initCanvas() {
		int canvasWidth = Math.min((int) (WINDOW_SIZE * aspectRatio), 1900);
		StdDraw.setCanvasSize(canvasWidth, WINDOW_SIZE);
		StdDraw.setXscale(0, width);
		StdDraw.setYscale(0, height);
		StdDraw.show(0);
		isCanvasInit = true;
	}

	private void drawGrid() {
		StdDraw.setPenColor(StdDraw.GRAY);
		StdDraw.setPenRadius();
		for (int i = 1; i < Math.max(width, height); i++) {
			StdDraw.line(i, 0, i, WINDOW_SIZE);
			StdDraw.line(0, i, WINDOW_SIZE, i);
		}
	}

	private void drawMines() {
		StdDraw.setPenColor(StdDraw.BLACK);
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				if (isMine[x][y]) {
					StdDraw.filledCircle(x + 0.5, y + 0.5, 0.4);
					if (isFlagged[x][y]) {
						StdDraw.setPenColor(StdDraw.RED);
						StdDraw.setPenRadius(0.025);
						StdDraw.line(x + 0.2, y + 0.2, x + 0.8, y + 0.8);
						StdDraw.line(x + 0.2, y + 0.8, x + 0.8, y + 0.2);
						StdDraw.setPenRadius();
						StdDraw.setPenColor(StdDraw.BLACK);
					}

				}
			}
		}
	}

	private boolean isPointOnBoard(int x, int y) {
		return x >= 0 && x < width && y >= 0 && y < height;
	}
	
	private ArrayList<Point> getNeighbours(int x, int y, int size) {
		ArrayList<Point> neighbours = new ArrayList<Point>();
		for (int dx = -size/2; dx <= size/2; dx++) {
			for (int dy = -size/2; dy <= size/2; dy++) {
				int neighbourX = x + dx;
				int neighbourY = y + dy;
				if (neighbourX == x && neighbourY == y) {
					continue;
				}
				if (isPointOnBoard(neighbourX, neighbourY)) {
					neighbours.add(new Point(neighbourX, neighbourY));
				}
			}
		}
		return neighbours;
	}
	
	private ArrayList<Point> getNeighbours(int x, int y) {
		return getNeighbours(x, y, 3);
	}

	private void countNeighbourMines(int x, int y) {
		mineNeighbours[x][y] = 0;
		for (Point neighbour : getNeighbours(x, y)) {
			if (isMine[neighbour.x][neighbour.y]) {
				mineNeighbours[x][y]++;
			}
		}
	}

	private void drawNeighbourCount() {
		StdDraw.setPenColor(StdDraw.BLACK);
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				if (mineNeighbours[x][y] > 0) {
					StdDraw.text(x + 0.5, y + 0.4, "" + mineNeighbours[x][y]);
				}
			}
		}
	}

	private void drawTileCover() {
		StdDraw.setPenColor(StdDraw.LIGHT_GRAY);
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				if (!isVisible[x][y]) {
					StdDraw.filledSquare(x + 0.5, y + 0.5, 0.45);
				}
			}
		}
	}

	private void drawFlagged() {
		StdDraw.setPenColor(StdDraw.RED);
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				if (isFlagged[x][y]) {
					StdDraw.filledSquare(x + 0.5, y + 0.5, 0.2);
				}
			}
		}
	}

	private void drawBigText(String text, Color color) {
		StdDraw.setFont(new Font("Arial", Font.BOLD, 120));
		StdDraw.setPenColor();
		StdDraw.text(width / 2 - 0.1, height / 2 - 0.1, text);
		StdDraw.setPenColor(color);
		StdDraw.text(width / 2, height / 2, text);
		StdDraw.setFont(font);
	}

	private void drawGameOver() {
		drawBigText("GAME OVER", StdDraw.RED);
	}

	private void drawVictory() {
		drawBigText("VICTORY", StdDraw.GREEN);
	}

	private void drawMineLikelihood() {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				if (mineLikelihood[x][y] > 0) {
					StdDraw.setPenColor(StdDraw.RED);
					StdDraw.text(x + 0.5, y + 0.4, "" + mineLikelihood[x][y]);
				}
			}
		}
	}

	private void calculateMineLikelihood(int x, int y) {
		// Used to check whether the potential mine is a corner.
		int[][] cornerNeighboursX = { { 0, 1, 1 }, { 0, -1, -1 } };
		int[][] cornerNeighboursY = { { 1, 1, 0 }, { -1, -1, 0 } };
		/* Start off by calculating the likelihood of there being a mine based on how
		 * many neighbours think they are within range of a mine. */
		if (isVisible[x][y] && mineNeighbours[x][y] > 0) {
			// Count the amount of neighbours that are visible
			int notVisibleNeighbours = 0;
			for (Point neighbour : getNeighbours(x, y)) {
				if (!isVisible[neighbour.x][neighbour.y]) {
					notVisibleNeighbours++;
					if (!isFlagged[x][y]) {
						mineLikelihood[neighbour.x][neighbour.y]++;
					}
				}
			}
			// If the amount of not visible mines is the same as the amount of mines, then all neighbours must be mines
			if (notVisibleNeighbours == mineNeighbours[x][y]) {
				for (Point neighbour : getNeighbours(x, y)) {
					mineLikelihood[neighbour.x][neighbour.y] += 100;
				}
			}
		}
		// Check the amount of visible corners and increase the likelihood accordingly
		int cornerCount = 0;
		for (int i = 0; i < cornerNeighboursX.length; i++) {
			for (int j = 0; j < cornerNeighboursY.length; j++) {
				for (int k = 0; k < cornerNeighboursX[0].length; k++) {
					int cornerX = x + cornerNeighboursX[i][k];
					int cornerY = y + cornerNeighboursY[j][k];
					if (isPointOnBoard(cornerX, cornerY) && isVisible[cornerX][cornerY] && !isFlagged[x][y]) {
						cornerCount++;
					}
				}
			}
		}
		if (!isVisible[x][y] && !isFlagged[x][y]) {
			mineLikelihood[x][y] += cornerCount * 1;
		}
	}

	private void calculateMineLikelihood() {
		mineLikelihood = new int[width][height];
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				calculateMineLikelihood(x, y);
			}
		}
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				if (isVisible[x][y]) {
					int flaggedNeighbours = 0;
					ArrayList<Point> neighbours = getNeighbours(x, y);
					for (Point neighbour : neighbours) {
						if (isFlagged[neighbour.x][neighbour.y]) {
							flaggedNeighbours++;
						}
					}
					if (flaggedNeighbours == mineNeighbours[x][y]) {
						for (Point neighbour : neighbours) {
							mineLikelihood[neighbour.x][neighbour.y] = -1;
						}
					}
				}
			}
		}
		
	}

	public ArrayList<Point> getBestFlags() {
		ArrayList<Point> bestFlags = new ArrayList<Point>();
		// Find the largest mine likelihood
		maxMineLikelihood = 0;
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				if (mineLikelihood[x][y] > maxMineLikelihood && !isFlagged[x][y]) {
					maxMineLikelihood = mineLikelihood[x][y];
				}
			}
		}
		// Add all tiles the max mine likelihood
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				if (!isVisible[x][y] && mineLikelihood[x][y] == maxMineLikelihood) {
					bestFlags.add(new Point(x, y));
				}
			}
		}
		return bestFlags;
	}

	private void drawBestFlag() {
		ArrayList<Point> bestFlags = getBestFlags();
		StdDraw.setPenRadius(0.010);
		for (Point bestFlag : bestFlags) {
			if (mineLikelihood[bestFlag.x][bestFlag.y] > 0) {
				StdDraw.square(bestFlag.x + 0.5, bestFlag.y + 0.5, 0.4);
			}
		}
		StdDraw.setPenRadius();
	}
	
	public void eliminateTiles(boolean allTiles) {
		if (allTiles) {
			for (int x = 0; x < width; x++) {
				for (int y = 0; y < height; y++) {
					if (isVisible[x][y] && mineNeighbours[x][y] > 0) {
						for (Point neighbour : getNeighbours(x, y)) {
							if (mineLikelihood[neighbour.x][neighbour.y] < 0) {
								mouseClicked(neighbour.x, neighbour.y, false, false);
							}
						}
					}
				}
			}
		} else {
			for (Point point : clickedPoints) {
				for (Point neighbour : getNeighbours(point.x, point.y, 5)) {
					if (mineLikelihood[neighbour.x][neighbour.y] < 0) {
						mouseClicked(neighbour.x, neighbour.y, false, false);
					}
				}
			}
			clickedPoints.clear();
		}
	}
	
	public void eliminateTiles() {
		eliminateTiles(false);
	}

	public void draw() {
		if (!isCanvasInit) {
			initCanvas();
		}
		drawGrid();
		drawNeighbourCount();
		drawTileCover();
		drawFlagged();
		if (hintsEnabled) {
			drawBestFlag();
			drawMineLikelihood();
		}
		if (gameOver) {
			drawMines();
			drawExplosion(gameOverMine.x, gameOverMine.y);
			drawGameOver();
		}
		if (victory) {
			drawVictory();
			System.out.println("VICTORY!");
		}
		if (debugMode) {
			drawDebug();
		}
		StdDraw.show(0);
		StdDraw.clear();
	}

	private void checkVictory() {
		// Count amount of flagged mines
		int flaggedMines = 0;
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				if (isMine[x][y] && isFlagged[x][y]) {
					flaggedMines++;
				}
			}
		}
		// Count amount of tiles that are not visible
		int notVisibleCount = 0;
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				if (!isVisible[x][y]) {
					notVisibleCount++;
				}
			}
		}
		if (flaggedMines == mineCount && notVisibleCount == mineCount) {
			victory = true;
		}
	}

	private void drawStar(int x, int y, double penSize, Color color) {
		StdDraw.setPenColor(color);
		StdDraw.setPenRadius(penSize);
		StdDraw.line(x + 0.2, y + 0.2, x + 0.8, y + 0.8);
		StdDraw.line(x + 0.2, y + 0.8, x + 0.8, y + 0.2);
		StdDraw.line(x + 0.2, y + 0.5, x + 0.8, y + 0.5);
		StdDraw.line(x + 0.5, y + 0.8, x + 0.5, y + 0.2);
		StdDraw.setPenRadius();
		StdDraw.setPenColor(StdDraw.BLACK);

	}

	private void drawExplosion(int x, int y) {
		drawStar(x, y, 0.015, StdDraw.RED);
		drawStar(x, y, 0.008, StdDraw.ORANGE);
	}

	public void mouseClicked(double mouseX, double mouseY, boolean rightClick, boolean saveClick) {
		int x = (int) mouseX;
		int y = (int) mouseY;
		if (!isPointOnBoard(x, y)) {
			return;
		}
		if (saveClick) {
			clickedPoints.add(new Point(x, y));
		}
		if (firstClick) {
			if (isMine[x][y]) {
				isMine[x][y] = false;
				mineCount--;
			}
			for (Point neighbour : getNeighbours(x, y)) {
				if (isMine[neighbour.x][neighbour.y]) {
					isMine[neighbour.x][neighbour.y] = false;
					mineCount--;
				}
			}
			firstClick = false;
			calculateNeighbourCounts();
			floodReveal(x, y);
			calculateMineLikelihood();
			return;
		}
		mineLikelihood[x][y] = 0;
		if (rightClick && !isVisible[x][y]) {
			isFlagged[x][y] = !isFlagged[x][y];
			calculateMineLikelihood();
			checkVictory();
			return;
		}
		if (isMine[x][y] && !isFlagged[x][y]) {
			gameOverMine = new Point(x, y);
			gameOver = true;
			return;
		}
		if (!isVisible[x][y]) {
			if (mineNeighbours[x][y] == 0) {
				floodReveal(x, y);
			} else {
				isVisible[x][y] = true;
			}
			calculateMineLikelihood();
			checkVictory();
		}
	}

	public void mouseClicked(double mouseX, double mouseY, boolean rightClick) {
		mouseClicked(mouseX, mouseY, rightClick, true);
	}
	
	public boolean isGameOver() {
		return gameOver;
	}

	public boolean isVictory() {
		return victory;
	}

	private void floodReveal(int x, int y) {
		boolean[][] hasVisited = new boolean[width][height];
		flood(x, y, 0, hasVisited);
	}

	private void flood(int x, int y, int floodCount, boolean[][] hasVisited) {
		if (!isPointOnBoard(x, y) || floodCount > floodThreshold || isMine[x][y] || isFlagged[x][y]
				|| hasVisited[x][y]) {
			return;
		}
		isVisible[x][y] = true;
		if (mineNeighbours[x][y] == 0) {
			floodCount++;
			hasVisited[x][y] = true;
			flood(x + 1, y, floodCount, hasVisited);
			flood(x - 1, y, floodCount, hasVisited);
			flood(x, y + 1, floodCount, hasVisited);
			flood(x, y - 1, floodCount, hasVisited);
			flood(x + 1, y + 1, floodCount, hasVisited);
			flood(x - 1, y + 1, floodCount, hasVisited);
			flood(x - 1, y - 1, floodCount, hasVisited);
			flood(x + 1, y - 1, floodCount, hasVisited);
		}
	}

	public void setDebugMode(boolean debugMode) {
		this.debugMode = debugMode;
	}

	private void drawDebug() {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				StdDraw.setPenColor(StdDraw.BOOK_RED);
				StdDraw.setFont(new Font("Arial", Font.PLAIN, 10));
				StdDraw.text(x + 0.25, y + 0.15, "(" + x + ", " + y + ")");
				StdDraw.text(x + 0.35, y + 0.25, "likely = " + mineLikelihood[x][y]);
				StdDraw.text(x + 0.45, y + 0.35, "neighbours = " + mineNeighbours[x][y]);
				StdDraw.text(x + 0.45, y + 0.45, "isMine = " + isMine[x][y]);
				int flaggedNeighbours = 0;
				for (Point neighbour : getNeighbours(x, y)) {
					if (isFlagged[neighbour.x][neighbour.y]) {
						flaggedNeighbours++;
					}
				}
				StdDraw.text(x + 0.45, y + 0.55, "flaggedN = " + flaggedNeighbours);
				int notVisibleNeighbours = 0;
				for (Point neighbour : getNeighbours(x, y)) {
					if (!isVisible[neighbour.x][neighbour.y]) {
						notVisibleNeighbours++;
					}
				}
				StdDraw.text(x + 0.45, y + 0.65, "notVisN = " + notVisibleNeighbours);
				StdDraw.setPenColor();
				StdDraw.setFont();
			}
		}
	}
	
	public MineSweeperState getCurrentState() {
		return new MineSweeperState(isMine, isFlagged, isVisible, gameOver);
	}
	
	public void loadState(MineSweeperState state) {
		this.isMine = MineSweeperState.copyArray(state.getIsMine());
		this.isFlagged = MineSweeperState.copyArray(state.getIsFlagged());
		this.isVisible = MineSweeperState.copyArray(state.getIsVisble());
		this.gameOver = state.isGameOver();
		calculateNeighbourCounts();
		calculateMineLikelihood();
	}
	
	public void setHintsEnabled(boolean hintsEnabled) {
		this.hintsEnabled = hintsEnabled;
	}
	
	public boolean isHintsEnabled() {
		return hintsEnabled;
	}
	
	public int getMaxMineLikelihood() {
		return maxMineLikelihood;
	}
}
