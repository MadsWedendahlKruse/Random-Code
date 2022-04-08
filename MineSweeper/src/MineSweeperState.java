
public class MineSweeperState {
	
	private boolean[][] isMine;
	private boolean[][] isFlagged;
	private boolean[][] isVisble;
	private boolean gameOver;
	
	public MineSweeperState(boolean[][] isMine, boolean[][] isFlagged, boolean[][] isVisble, boolean gameOver) {
		this.isMine = copyArray(isMine);
		this.isFlagged = copyArray(isFlagged);
		this.isVisble = copyArray(isVisble);
		this.gameOver = gameOver;
	}
	
	public static boolean[][] copyArray(boolean[][] source) {
		int width = source.length;
		int height = source[0].length;
		boolean[][] destination = new boolean[width][height];
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				destination[i][j] = source[i][j];
			}
		}
		return destination;
	}
	
	public boolean[][] getIsFlagged() {
		return isFlagged;
	}
	
	public boolean[][] getIsMine() {
		return isMine;
	}
	
	public boolean[][] getIsVisble() {
		return isVisble;
	}
	
	public boolean isGameOver() {
		return gameOver;
	}
}
