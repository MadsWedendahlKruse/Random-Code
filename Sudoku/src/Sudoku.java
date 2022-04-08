import java.awt.Color;
import java.awt.Font;
import java.awt.Point;
import java.awt.event.KeyEvent;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class Sudoku {
	public static Integer[][][] GRID = new Integer[9][3][3];
	public static boolean[][] LOCKED_SPOTS = new boolean[9][9];
	public static ArrayList<Integer[][][]> SAVES = new ArrayList<Integer[][][]>();
	public static final boolean DRAW_WHILE_SAVING = false;
	public static final Font BIG_FONT = new Font("Arial", Font.BOLD, 80);
	public static final Font SMALL_FONT = new Font("Arial", Font.BOLD, 16);
	public static final int WINDOW_SIZE = 900;
	public static boolean hintsEnabled = false;
	
	public static int bruteForceAttempts = 0;
	
	public static void main(String[] args) throws FileNotFoundException {
		StdDraw.setCanvasSize(WINDOW_SIZE, WINDOW_SIZE);
		StdDraw.setXscale(0, 9);
		StdDraw.setYscale(0, 9);
		StdDraw.show(0);
		String level = "notfun.sdk";
		loadFile(level);
		// Mouse input samples
		boolean mouse1 = false;
		boolean mouse2 = false;
		// Key input samples
		char key1 = ' ';
		char key2 = ' ';
		boolean draw = true;
		while (true) {
			mouse2 = StdDraw.mousePressed();
			if (StdDraw.hasNextKeyTyped()) {
				key2 = StdDraw.nextKeyTyped();
			} else {
				key2 = ' ';
				key1 = key2;
			}
			if (StdDraw.mousePressed() && mouse1 != mouse2) {
				incrementSpot(StdDraw.mouseX(), StdDraw.mouseY());
				draw = true;
			}
			mouse1 = mouse2;
			if (key1 != key2) {
				if (StdDraw.isKeyPressed(KeyEvent.VK_1) || StdDraw.isKeyPressed(KeyEvent.VK_NUMPAD1)) {
					setValue(StdDraw.mouseX(), StdDraw.mouseY(), 1);
				} else if (StdDraw.isKeyPressed(KeyEvent.VK_2) || StdDraw.isKeyPressed(KeyEvent.VK_NUMPAD2)) {
					setValue(StdDraw.mouseX(), StdDraw.mouseY(), 2);
				} else if (StdDraw.isKeyPressed(KeyEvent.VK_3) || StdDraw.isKeyPressed(KeyEvent.VK_NUMPAD3)) {
					setValue(StdDraw.mouseX(), StdDraw.mouseY(), 3);
				} else if (StdDraw.isKeyPressed(KeyEvent.VK_4) || StdDraw.isKeyPressed(KeyEvent.VK_NUMPAD4)) {
					setValue(StdDraw.mouseX(), StdDraw.mouseY(), 4);
				} else if (StdDraw.isKeyPressed(KeyEvent.VK_5) || StdDraw.isKeyPressed(KeyEvent.VK_NUMPAD5)) {
					setValue(StdDraw.mouseX(), StdDraw.mouseY(), 5);
				} else if (StdDraw.isKeyPressed(KeyEvent.VK_6) || StdDraw.isKeyPressed(KeyEvent.VK_NUMPAD6)) {
					setValue(StdDraw.mouseX(), StdDraw.mouseY(), 6);
				} else if (StdDraw.isKeyPressed(KeyEvent.VK_7) || StdDraw.isKeyPressed(KeyEvent.VK_NUMPAD7)) {
					setValue(StdDraw.mouseX(), StdDraw.mouseY(), 7);
				} else if (StdDraw.isKeyPressed(KeyEvent.VK_8) || StdDraw.isKeyPressed(KeyEvent.VK_NUMPAD8)) {
					setValue(StdDraw.mouseX(), StdDraw.mouseY(), 8);
				} else if (StdDraw.isKeyPressed(KeyEvent.VK_9) || StdDraw.isKeyPressed(KeyEvent.VK_NUMPAD9)) {
					setValue(StdDraw.mouseX(), StdDraw.mouseY(), 9);
				} else if (StdDraw.isKeyPressed(KeyEvent.VK_ENTER)) {
					loadFile(level);
					System.out.println("Board was reset.\n");
				} else if (StdDraw.isKeyPressed(KeyEvent.VK_Z)) {
					undo();
				} else if (StdDraw.isKeyPressed(KeyEvent.VK_ESCAPE)) {
					loadFile("blank.sdk");
				} else if (StdDraw.isKeyPressed(KeyEvent.VK_H)) {
					hintsEnabled = !hintsEnabled;
				}
				if (StdDraw.isKeyPressed(KeyEvent.VK_0)) {
					
					for (SolveMode solveMode : SolveMode.values()) {
						System.out.println("Solved " + uniqueSolve(solveMode) + " spots, mode = " + solveMode);
					}
					
					//System.out.println("Solved " + uniqueGridSolve() + " spots (uniqueGridSolve).");
					//System.out.println("Solved " + uniqueVerticalSolve() + " spots (uniqueVerticalSolve).");
					//System.out.println("Solved " + uniqueHorizontalSolve() + " spots (uniqueHorizontalSolve).\n");
				}
				if (StdDraw.isKeyPressed(KeyEvent.VK_PLUS)) {
					System.out.println("Solved " + simpleSolve() + " spots (simpleSolve).\n");
				}
				if (StdDraw.isKeyPressed(KeyEvent.VK_X)) {
					long bruteForceStartTime = System.currentTimeMillis();
					bruteForce();
					System.out.println("Brute forced sudoku in " + (System.currentTimeMillis() - bruteForceStartTime) + " ms");
				}
				draw = true;
			}
			key1 = key2;
			if (draw) {
				drawSudoku();
				draw = false;
			}
			if (SAVES.isEmpty() || !isSameBoard(GRID, SAVES.get(SAVES.size() - 1))) {
				saveBoard();
			}
		}
	}
	
	private static void drawSudoku() {
		StdDraw.clear();
		drawBoard();
		checkAllValues();
		if (hintsEnabled) {
			drawAllPossibleValues();
		}
		drawValues();
		StdDraw.show(0);
	}
	
	private static void undo() {
		if (!SAVES.isEmpty()) {
			SAVES.remove(SAVES.size() - 1);
			if (!SAVES.isEmpty()) {
				loadArray(SAVES.get(SAVES.size() - 1));
			}
		}
	}

	private static void drawBoard() {
		StdDraw.setPenColor(StdDraw.GRAY);
		for (int i = 1; i < 9; i++) {
			if (i % 3 == 0) {
				StdDraw.setPenRadius(0.008);
			} else {
				StdDraw.setPenRadius();
			}
			StdDraw.line(i, 0, i, WINDOW_SIZE);
			StdDraw.line(0, i, WINDOW_SIZE, i);
		}
	}

	private static void drawValues() {
		StdDraw.setFont(BIG_FONT);
		for (int i = 0; i < 9; i++) {
			double x = getX(i);
			double y = getY(i);
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					double dx = x + j + 0.5;
					double dy = y + k + 0.38;
					if (GRID[i][j][k] != 0) {
						if (LOCKED_SPOTS[(int) dx][(int) dy]) {
							StdDraw.setPenColor(StdDraw.BLACK);
						} else {
							StdDraw.setPenColor(StdDraw.GRAY);
						}
						StdDraw.text(dx, dy, "" + GRID[i][j][k]);
					}
				}
			}
		}
	}

	private static double getX(int index) {
		double x = -1;
		switch (index % 3) {
		case 0:
			x = 0;
			break;
		case 1:
			x = 3;
			break;
		case 2:
			x = 6;
			break;
		default:
			break;
		}
		return x;
	}

	private static double getY(int index) {
		double y = -1;
		switch (index / 3) {
		case 0:
			y = 0;
			break;
		case 1:
			y = 3;
			break;
		case 2:
			y = 6;
			break;
		default:
			break;
		}
		return y;
	}

	private static int getIndex(double x, double y) {
		int dx = (int) x / 3;
		int dy = (int) y / 3;
		int gridIdx = -1;
		if (dx == 0) {
			if (dy == 0) {
				gridIdx = 0;
			} else if (dy == 1) {
				gridIdx = 3;
			} else if (dy == 2) {
				gridIdx = 6;
			}
		} else if (dx == 1) {
			if (dy == 0) {
				gridIdx = 1;
			} else if (dy == 1) {
				gridIdx = 4;
			} else if (dy == 2) {
				gridIdx = 7;
			}
		} else if (dx == 2) {
			if (dy == 0) {
				gridIdx = 2;
			} else if (dy == 1) {
				gridIdx = 5;
			} else if (dy == 2) {
				gridIdx = 8;
			}
		}
		return gridIdx;
	}

	public static int getValue(double x, double y) {
		int gridIdx = getIndex(x, y);
		int gridX = (int) (x - getX(gridIdx));
		int gridY = (int) (y - getY(gridIdx));
		return GRID[gridIdx][gridX][gridY];
	}

	public static void setValue(double x, double y, int value) {
		int gridIdx = getIndex(x, y);
		int gridX = (int) (x - getX(gridIdx));
		int gridY = (int) (y - getY(gridIdx));
		if (!LOCKED_SPOTS[(int) x][(int) y]) {
			GRID[gridIdx][gridX][gridY] = value;
		}
	}

	public static void colorSpot(double x, double y, Color color) {
		StdDraw.setPenColor(color);
		StdDraw.filledSquare((int) x + 0.5, (int) y + 0.5, 0.45);
		StdDraw.setPenColor(StdDraw.WHITE);
		StdDraw.filledSquare((int) x + 0.5, (int) y + 0.5, 0.40);
	}

	public static int[] getGridValues(double x, double y) {
		int gridIdx = getIndex(x, y);
		int[] values = new int[9];
		int i = 0;
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				values[i] = GRID[gridIdx][j][k];
				i++;
			}
		}
		return values;
	}

	public static int[] getHorizontalValues(double x, double y) {
		int gridIdx = getIndex(x, y);
		double originY = getY(gridIdx);
		int[] values = new int[9];
		if (gridIdx == 1 || gridIdx == 4 || gridIdx == 7) {
			gridIdx -= 1;
		} else if (gridIdx == 2 || gridIdx == 5 || gridIdx == 8) {
			gridIdx -= 2;
		}
		int i = 0;
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				values[i] = GRID[gridIdx + j][k][(int) (y - originY)];
				i++;
			}
		}
		return values;
	}

	public static int[] getVerticalValues(double x, double y) {
		int gridIdx = getIndex(x, y);
		double originX = getX(gridIdx);
		int[] values = new int[9];
		if (gridIdx == 3 || gridIdx == 4 || gridIdx == 5) {
			gridIdx -= 3;
		} else if (gridIdx == 6 || gridIdx == 7 || gridIdx == 8) {
			gridIdx -= 6;
		}
		int i = 0;
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				values[i] = GRID[gridIdx + j * 3][(int) (x - originX)][k];
				i++;
			}
		}
		return values;
	}

	public static boolean isContained(int value, int[] values, int timesContained) {
		if (value == 0) {
			return false;
		}
		int count = 0;
		for (int i = 0; i < values.length; i++) {
			if (value == values[i]) {
				count++;
			}
		}
		return count >= timesContained;
	}

	public static boolean isValueValid(double x, double y, int value, int timesContained) {
		boolean invalid;
		int[] gridValues = getGridValues(x, y);
		int[] horizontalValues = getHorizontalValues(x, y);
		int[] verticalValues = getVerticalValues(x, y);
		boolean grid = isContained(value, gridValues, timesContained);
		boolean horizontal = isContained(value, horizontalValues, timesContained);
		boolean vertical = isContained(value, verticalValues, timesContained);
		invalid = grid || horizontal || vertical;
		return !invalid;
	}

	public static void incrementSpot(double x, double y) {
		int value = getValue(x, y);
		if (value != 9) {
			setValue(x, y, ++value);
		} else {
			setValue(x, y, 0);
		}
	}

	public static void decrementSpot(double x, double y) {
		int value = getValue(x, y);
		if (value != 0) {
			setValue(x, y, --value);
		} else {
			setValue(x, y, 9);
		}
	}

	public static int checkAllValues() {
		int invalidCount = 0;
		for (int x = 0; x < 9; x++) {
			for (int y = 0; y < 9; y++) {
				if (!isValueValid(x, y, getValue(x, y), 2)) {
					colorSpot(x, y, StdDraw.RED);
					invalidCount++;
				} else {
					colorSpot(x, y, StdDraw.WHITE);
				}
			}
		}
		return invalidCount;
	}

	public static boolean isBoardValid() {
		for (int x = 0; x < 9; x++) {
			for (int y = 0; y < 9; y++) {
				if (getPossibleValues(x, y).isEmpty()) {
					return false;
				}
			}
		}
		return checkAllValues() > 0;
	}
	
	public static ArrayList<Integer> getPossibleValues(double x, double y) {
		ArrayList<Integer> possibleValues = new ArrayList<Integer>();
		if (getValue(x, y) != 0) {
			return possibleValues;
		}
		for (int i = 1; i <= 9; i++) {
			if (isValueValid(x, y, i, 1)) {
				possibleValues.add(i);
			}
		}
		return possibleValues;
	}

	public static void drawAllPossibleValues() {
		for (int x = 0; x < 9; x++) {
			for (int y = 0; y < 9; y++) {
				ArrayList<Integer> possibleValues = getPossibleValues(x, y);
				StdDraw.setFont(SMALL_FONT);
				StdDraw.setPenColor(StdDraw.GRAY);
				int i = 0;
				for (int j = 0; j < 3; j++) {
					for (int k = 0; k < 3; k++) {
						if (i < possibleValues.size() && possibleValues.get(i) != 0) {
							StdDraw.text(x + (k + 1) * 0.25, y + (j + 1) * 0.25, "" + possibleValues.get(i));
							i++;
						}
					}
				}
			}
		}
	}

	public static boolean isSolved() {
		boolean solved = true;
		for (int x = 0; x < 9; x++) {
			for (int y = 0; y < 9; y++) {
				int value = getValue(x, y);
				if (value == 0 || !isValueValid(x, y, value, 2)) {
					solved = false;
				}
			}
		}
		return solved;
	}

	public static void reset() {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					GRID[i][j][k] = 0;
				}
			}
		}
	}

	public static void loadFile(String filename) throws FileNotFoundException {
		Scanner input = new Scanner(new File(filename));
		for (int y = 8; y >= 0; y--) {
			for (int x = 0; x < 9; x++) {
				int loadedValue = input.nextInt();
				LOCKED_SPOTS[x][y] = false;
				setValue(x, y, loadedValue);
				if (getValue(x, y) != 0) {
					LOCKED_SPOTS[x][y] = true;
				}
			}
		}
		input.close();
		SAVES.clear();
	}

	public static void loadArray(Integer[][][] integers) {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					GRID[i][j][k] = integers[i][j][k];
				}
			}
		}
	}

	public static void saveBoard() {
		Integer[][][] save = new Integer[9][3][3];
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					save[i][j][k] = GRID[i][j][k];
				}
			}
		}
		SAVES.add(save);
		if (DRAW_WHILE_SAVING) {
			drawSudoku();
		}
	}

	public static void printBoard() {
		for (int i = 0; i < 9; i++) {
			System.out.println("Grid " + i + ": ");
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					System.out.print(GRID[i][j][k] + ", ");
				}
			}
			System.out.println("\n");
		}
		System.out.println();
	}

	public static void printBoard(int[][][] array) {
		for (int i = 0; i < 9; i++) {
			System.out.println("Grid " + i + ": ");
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					System.out.print(array[i][j][k] + ", ");
				}
			}
			System.out.println();
		}
		System.out.println();
	}

	public static boolean isSameBoard(Integer[][][] board1, Integer[][][] board2) {
		boolean same = true;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					if (board1[i][j][k] != board2[i][j][k]) {
						same = false;
					}
				}
			}
		}
		return same;
	}
	
	public static void printIdents(int indents) {
		for (int i = 0; i < indents; i++) {
			System.out.print(" ");
		}
	}
	
	public static void bruteForce(int depth) {
		bruteForceAttempts++;
		if (isSolved()) {
			System.out.println("Solved sudoku in " + bruteForceAttempts + " steps!");
			saveBoard();
			return;
		}
		// Find the tile with the least amount of possible values
		int minPossibleValuesCount = 9;
		ArrayList<Integer> minPossibleValues = new ArrayList<Integer>();
		int minX = -1, minY = -1;
		for (int x = 0; x < 9; x++) {
			for (int y = 0; y < 9; y++) {
				if (getValue(x, y) == 0) {
					ArrayList<Integer> possibleValues = getPossibleValues(x, y);
					if (possibleValues.size() < minPossibleValuesCount) {
						minPossibleValues = possibleValues;
						minPossibleValuesCount = possibleValues.size();
						minX = x;
						minY = y;
					}
				}
				
			}
		}
		printIdents(depth);
		System.out.println("Brute forcing, depth = " + depth + ", attempts = " + bruteForceAttempts + ". ");
		printIdents(depth);
		System.out.println("Found " + minPossibleValuesCount + " possible value(s) " + minPossibleValues + " @ (" + minX + ", " + minY + ")");
		for (Integer possibleValue : minPossibleValues) {
			if (isSolved()) {
				break;
			}
			printIdents(depth);
			System.out.println("Trying " + possibleValue);
			setValue(minX, minY, possibleValue);
			saveBoard();
			bruteForce(++depth);
		}
		if (!isSolved()) {
			undo();
		}
		//drawSudoku();
	}
	
	public static void bruteForce() {
		bruteForce(0);
		bruteForceAttempts = 0;
	}
	

	public static int simpleSolve() {
		int solves = 0;
		for (int x = 0; x < 9; x++) {
			for (int y = 0; y < 9; y++) {
				ArrayList<Integer> possibleValues = getPossibleValues(x, y);
				if (possibleValues.size() == 1) {
					setValue(x, y, possibleValues.get(0));
					solves++;
				}
			}
		}
		return solves;
	}
	
	public static int uniqueSolve(SolveMode solveMode) {
		int solves = 0;
		for (int i = 0; i < 9; i++) {
			int[] possibleCount = new int[9];
			double x, y;
			int jMax, kMax;
			switch (solveMode) {
			case GRID:
				x = getX(i);
				y = getY(i);
				jMax = kMax = 3;
				break;
			case HORIZONTAL:
				y = i;
				x = 0;
				jMax = 1;
				kMax = 9;
				break;
			case VERTICAL:
				y = 0;
				x = i;
				jMax = 9;
				kMax = 1;
				break;
			default:
				x = y = 0;
				jMax = kMax = 0;
				break;
			}
			int uniqueValue = -1;
			for (int j = 0; j < jMax; j++) {
				for (int k = 0; k < kMax; k++) {
					ArrayList<Integer> possibleValues = getPossibleValues(x + k, y + j);
					for (Integer possibleValue : possibleValues) {
						possibleCount[possibleValue - 1]++;
					}
				}
			}
			for (int j = 0; j < possibleCount.length; j++) {
				if (possibleCount[j] == 1) {
					uniqueValue = j + 1;
				}
			}
			if (uniqueValue != -1) {
				for (int j = 0; j < jMax; j++) {
					for (int k = 0; k < kMax; k++) {
						ArrayList<Integer> possibleValues = getPossibleValues(x + k, y + j);
						for (int possibleValue : possibleValues) {
							if (possibleValue == uniqueValue) {
								setValue(x + k, y + j, uniqueValue);
								solves++;
							}
						}
					}
				}
			}
		}
		return solves;
	}
	
	public static int uniqueSolve() {
		int solves = 0;
		for (SolveMode solveMode : SolveMode.values()) {
			solves += uniqueSolve(solveMode);
		}
		return solves;
	}
}

enum SolveMode {
	GRID,
	HORIZONTAL,
	VERTICAL
}
