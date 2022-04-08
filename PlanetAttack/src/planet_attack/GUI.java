package planet_attack;

import java.awt.Color;
import java.awt.Font;
import java.awt.event.KeyEvent;
import java.util.ArrayList;

import planet_attack.core.World;
import planet_attack.enemies.Enemy;
import planet_attack.math.AABB;
import planet_attack.math.Vector;

public class GUI {
	
	public static final int GAME_WINDOW_WIDTH = 1900; // pixels
	public static final int GAME_WINDOW_HEIGHT = 900; // pixels
	public static final int GAME_GAMERA_VIEW_WIDTH = 512; // meters
	public static final int GAME_GAMERA_VIEW_HEIGHT = (int) ((double) (GAME_WINDOW_HEIGHT)/GAME_WINDOW_WIDTH*GAME_GAMERA_VIEW_WIDTH); // meters
	public static final AABB CAMERA_AABB = new AABB(0, 0, GAME_GAMERA_VIEW_WIDTH, GAME_GAMERA_VIEW_HEIGHT);
	//public static final Polygon CAMERA_HITBOX = new Polygon(new Vector(0, 0), GAME_GAMERA_VIEW_WIDTH, GAME_GAMERA_VIEW_HEIGHT);
	private static boolean isCanvasInit = false;
	public static final boolean DRAW_DEBUG = false;
	
	// Not really sure about the units for these...
	public static final int MINIMAP_MIN_X = 2;
	public static final int MINIMAP_MIN_Y = 190;
	public static final int MINIMAP_WIDTH = 50;
	public static final int MINIMAP_HEIGHT = 50;
	
	public static final int PLAYER_HEALTH_X = 55;
	public static final int PLAYER_HEALTH_Y = 235;
	public static final int PLAYER_HEALTH_WIDTH = 1;
	public static final int PLAYER_HEALTH_HEIGHT = 4;
	public static final int PLAYER_HEALTH_SPACING = 1;
	
	public static final int PLAYER_SCORE_X = 64;
	public static final int PLAYER_SCORE_Y = 225;
	
	public static double cameraX;
	public static double cameraY;
	
	private World world;
	
	public GUI(World world) {
		this.world = world;
		StdDraw.setFont(new Font("Serif", Font.ITALIC, 18));
	}
	
	public void draw(int currentTick) {
		if (!isCanvasInit) {
			initCanvas();
		}
		moveCamera();
		handleInput();
		world.draw(currentTick);
		drawMinimap();
		drawPlayerHealth(world.getPlayer(1));
		drawPlayerScore(world.getPlayer(1));
		StdDraw.show(0);
	}

	public void initCanvas() {
		StdDraw.setCanvasSize(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);
		StdDraw.setXscale(0, GAME_GAMERA_VIEW_WIDTH);
		StdDraw.setYscale(0, GAME_GAMERA_VIEW_HEIGHT);
		StdDraw.show(0);
		isCanvasInit = true;
	}
	
	public void moveCamera() {
		Player playerOne = world.getPlayer(1);
		cameraX = Math.min(Math.max(playerOne.getX() - GAME_GAMERA_VIEW_WIDTH/2, 0), World.WORLD_WIDTH - GAME_GAMERA_VIEW_WIDTH);
		cameraY = Math.min(Math.max(playerOne.getY() - GAME_GAMERA_VIEW_HEIGHT/2, 0), World.WORLD_HEIGHT - GAME_GAMERA_VIEW_HEIGHT);
		CAMERA_AABB.moveTo(new Vector(cameraX, cameraY));
	}
		
	private void drawMinimap() {
		StdDraw.setPenColor(StdDraw.DARK_GRAY);
		Vector center = new Vector(MINIMAP_MIN_X + (MINIMAP_WIDTH)/2, MINIMAP_MIN_Y + (MINIMAP_HEIGHT)/2);
		// Minimap border
		StdDraw.rectangle(center.getX(), center.getY(), MINIMAP_WIDTH/2, MINIMAP_HEIGHT/2);
		ArrayList<Enemy> enemies = world.getEnemies();
		for (Enemy enemy : enemies) {
			drawMarkerMinimap(enemy.getPos(), StdDraw.RED);
		}
		Player player = world.getPlayer(1);
		drawMarkerMinimap(player.getPos(), StdDraw.YELLOW);
	}
	
	public Vector pointToMinimap(double x, double y) {
		return new Vector(x * MINIMAP_WIDTH/World.WORLD_WIDTH + MINIMAP_MIN_X, y * MINIMAP_HEIGHT/World.WORLD_HEIGHT + MINIMAP_MIN_Y);
	}
	
	public void drawMarkerMinimap(Vector point, Color color) {
		StdDraw.setPenColor(color);
		Vector pointMinimap = pointToMinimap(point.getX(), point.getY());
		StdDraw.filledRectangle(pointMinimap.getX(), pointMinimap.getY(), 1, 1);
	}
	
	private void drawPlayerHealth(Player player) {
		int maxHealth = player.getMaxHealth();
		int health = player.getHealth();
		StdDraw.setPenColor(StdDraw.YELLOW);
		for (int i = 0; i < health; i++) {
			StdDraw.filledRectangle(PLAYER_HEALTH_X + i * (PLAYER_HEALTH_SPACING + 2*PLAYER_HEALTH_WIDTH), PLAYER_HEALTH_Y, PLAYER_HEALTH_WIDTH, PLAYER_HEALTH_HEIGHT);
		}
		StdDraw.setPenColor(StdDraw.DARK_GRAY);
		for (int i = maxHealth - 1; i >= health; i--) {
			StdDraw.filledRectangle(PLAYER_HEALTH_X + i * (PLAYER_HEALTH_SPACING + 2*PLAYER_HEALTH_WIDTH), PLAYER_HEALTH_Y, PLAYER_HEALTH_WIDTH, PLAYER_HEALTH_HEIGHT);
		}
	}
	
	private void drawPlayerScore(Player player) {
		StdDraw.setPenColor(StdDraw.WHITE);
		StdDraw.text(PLAYER_SCORE_X, PLAYER_SCORE_Y, "SCORE " + player.getScore());
	}
	
	public static void rectangle(double x, double y, double halfWidth, double halfHeight) {
		StdDraw.rectangle(x - cameraX, y - cameraY, halfWidth, halfHeight);
	}
	
	public static void line(double x0, double y0, double x1, double y1) {
		StdDraw.line(x0 - cameraX, y0 - cameraY, x1 - cameraX, y1 - cameraY);
	}
	
	public static void picture(double x, double y, String texture, double width, double height, double rotation) {
		try {
			StdDraw.picture(x - cameraX, y - cameraY, texture, width, height, rotation);
		} catch (IllegalArgumentException e) {
			StdDraw.picture(x - cameraX, y - cameraY, "error_texture.png", width, height, rotation);
		}
	}
	
	public static void text(double x, double y, String string) {
		StdDraw.text(x - cameraX, y - cameraY, string);
	}
	
	public void handleInput() {
		if (StdDraw.isKeyPressed(KeyEvent.VK_UP)) {
			cameraY += 10;
		}
		if (StdDraw.isKeyPressed(KeyEvent.VK_DOWN)) {
			cameraY -= 10;
		}
		if (StdDraw.isKeyPressed(KeyEvent.VK_RIGHT)) {
			cameraX += 10;
		}
		if (StdDraw.isKeyPressed(KeyEvent.VK_LEFT)) {
			cameraX -= 10;
		}
		CAMERA_AABB.moveTo(new Vector(cameraX, cameraY));
	}
}
