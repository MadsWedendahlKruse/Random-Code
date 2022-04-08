package planet_attack.core;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import planet_attack.GUI;
import planet_attack.Player;
import planet_attack.StdDraw;
import planet_attack.asteroids.Asteroid;
import planet_attack.asteroids.AsteroidSize;
import planet_attack.enemies.Enemy;
import planet_attack.enemies.EnemyCargo;
import planet_attack.math.Vector;

public class World {

	public static final int WORLD_WIDTH = 1024;
	public static final int WORLD_HEIGHT = 1024;
	public static final int CHUNK_SIZE = 64;
	public static final int WORLD_GRID_WIDTH = WORLD_WIDTH/CHUNK_SIZE;
	public static final int WORLD_GRID_HEIGHT = WORLD_HEIGHT/CHUNK_SIZE;
	private Chunk[][] chunks;
	
	private int maxPlayers, maxEnemies, maxAsteroids;
	public static final int MAX_ASTEROIDS_BUFFER = 20; // Used to prevent the max amount of asteroids from being reached immediately 
	public static double CARGO_SPAWN_CHANCE = 0.2;
	
	// Player and enemy lists are only used for easier access. Actual logic is handled by the chunks
	private ArrayList<Player> players = new ArrayList<Player>();
	private ArrayList<Enemy> enemies = new ArrayList<Enemy>();
	private ArrayList<PhysicsObject> spawnQueue = new ArrayList<PhysicsObject>();
	private boolean spawnedCargoShip = false; // whether a cargo ship has been spawned on this wave
	
	private boolean isPaused = true;
	private int wave = 0;

	public World(int maxPlayers, int maxEnemies, int maxBulletsPlayer, int maxBulletsEnemy, int maxAsteroids,
			int maxPowerUps) {
		this.maxPlayers = maxPlayers;
		this.maxEnemies = maxEnemies;
		this.maxAsteroids = maxAsteroids + MAX_ASTEROIDS_BUFFER;
	}
	
	/**
	 * Generates a random world based on the given seed
	 */
	public void randomize() {
		initChunks();
		List<Integer> chunkIdxRange = IntStream.range(0, WORLD_GRID_WIDTH*WORLD_GRID_HEIGHT).boxed().collect(Collectors.toList());
		Collections.shuffle(chunkIdxRange, GameLoop.RANDOM);
		int spawnedPlayers = 0, spawnedEnemies = 0, spawnedAsteroids = 0, i = 0;
		while ((spawnedPlayers + spawnedEnemies + spawnedAsteroids) < (maxPlayers + maxEnemies + maxAsteroids - MAX_ASTEROIDS_BUFFER)) {
			Vector vel = new Vector();
			int chunkX = chunkIdxRange.get(i) % WORLD_GRID_WIDTH;
			int chunkY = chunkIdxRange.get(i) / WORLD_GRID_HEIGHT;
			Chunk chunk = chunks[chunkX][chunkY];
			Vector chunkCenter = chunk.getCenter();
			chunkCenter.add(GameLoop.RANDOM.nextDouble()*CHUNK_SIZE/2, GameLoop.RANDOM.nextDouble()*CHUNK_SIZE/2);
			// Object to be spawned next
			PhysicsObject object = null;
			// Check if we've spawned enough players
			if (spawnedPlayers < maxPlayers) {
				Player player = new Player(chunkCenter, i, this);
				players.add(player);
				chunk.add(player);
				spawnedPlayers++;
			// Check if we've spawned enough enemies
			} else if (spawnedEnemies < maxEnemies) {
				spawnRandomEnemy(chunkX, chunkY);
				spawnedEnemies++;
			// Check if we've spawned enough asteroids
			} else if (spawnedAsteroids < (maxAsteroids - MAX_ASTEROIDS_BUFFER)) {
				AsteroidSize asteroidSize = AsteroidSize.values()[GameLoop.RANDOM.nextInt(AsteroidSize.values().length)];
				object = new Asteroid(chunkCenter, vel, asteroidSize, this);
				object.rotateTo(GameLoop.RANDOM.nextDouble() * 2*Math.PI);
				spawnedAsteroids++;
			}
			if (object != null) {
				spawnObject(object);
			}
			i++;
		}
	}

	private void initChunks() {
		chunks = new Chunk[WORLD_GRID_WIDTH][WORLD_GRID_HEIGHT];
		for (int chunkX = 0; chunkX < WORLD_GRID_WIDTH; chunkX++) {
			for (int chunkY = 0; chunkY < WORLD_GRID_HEIGHT; chunkY++) {
				chunks[chunkX][chunkY] = new Chunk(chunkX*CHUNK_SIZE, chunkY*CHUNK_SIZE, this);
			}
		}
	}

	public void draw(int currentTick) {
		StdDraw.setPenColor(StdDraw.BLACK);
		StdDraw.filledRectangle(WORLD_WIDTH/2 - GUI.cameraX, WORLD_HEIGHT/2 - GUI.cameraY, WORLD_WIDTH/2, WORLD_HEIGHT/2);
		for (int chunkX = 0; chunkX < WORLD_GRID_WIDTH; chunkX++) {
			for (int chunkY = 0; chunkY < WORLD_GRID_HEIGHT; chunkY++) {
				if (GUI.CAMERA_AABB.isColliding(chunks[chunkX][chunkY].getChunkAabb())) {
					chunks[chunkX][chunkY].draw(currentTick);
				}
			}
		}
	}

	public void update(int currentTick) {
		if (isPaused) {
			handleSpawning();
			for (Player player : players) {
				if (player.handleInput()) {
					isPaused = false;
				}
			}
			if (isPaused) {
				return;
			}
		}
		if (enemies.size() == 0) {
			spawnedCargoShip = false;
			spawnNewWave();
		}
		for (int chunkX = 0; chunkX < WORLD_GRID_WIDTH; chunkX++) {
			for (int chunkY = 0; chunkY < WORLD_GRID_HEIGHT; chunkY++) {
				if (!chunks[chunkX][chunkY].isEmpty()) {
					chunks[chunkX][chunkY].update(currentTick);
				}
			}
		}
		for (int chunkX = 0; chunkX < WORLD_GRID_WIDTH; chunkX++) {
			for (int chunkY = 0; chunkY < WORLD_GRID_HEIGHT; chunkY++) {
				if (!chunks[chunkX][chunkY].isEmpty()) {
					chunks[chunkX][chunkY].checkCollisions();
				}
			}
		}
		handleSpawning();
	}
	
	private void spawnNewWave() {
		List<Integer> chunkIdxRange = IntStream.range(0, WORLD_GRID_WIDTH*WORLD_GRID_HEIGHT).boxed().collect(Collectors.toList());
		Collections.shuffle(chunkIdxRange, GameLoop.RANDOM);
		for (int i = 0; i < chunkIdxRange.size() && enemies.size() < maxEnemies; i++) {
			int chunkX = chunkIdxRange.get(i) % WORLD_GRID_WIDTH;
			int chunkY = chunkIdxRange.get(i) / WORLD_GRID_HEIGHT;
			spawnRandomEnemy(chunkX, chunkY);
		}
		wave++;
		maxEnemies += 5;
		System.out.println("Spawned wave " + wave + " with " + enemies.size() + " enemies!");
	}
	
	private void spawnRandomEnemy(int chunkX, int chunkY) {
		Chunk chunk = chunks[chunkX][chunkY];
		if (chunk.isEmpty()) {
			//Enemy enemy = new EnemyKamikaze(chunk.getCenter(), this);
			Enemy enemy = Enemy.createEnemy(chunk.getCenter(), this);
			if (!spawnedCargoShip && GameLoop.RANDOM.nextDouble() < CARGO_SPAWN_CHANCE) {
				enemy = new EnemyCargo(chunk.getCenter(), this);
				spawnedCargoShip = true;
			}
			spawnObject(enemy);
			enemies.add(enemy);
		}
	}
	
	private void handleSpawning() {
		if (!spawnQueue.isEmpty()) {
			for (PhysicsObject nextSpawn : spawnQueue) {
				Chunk chunk = getChunk(nextSpawn.getPos());
				if (chunk != null) {
					chunk.add(nextSpawn);
				} else {
					System.out.println(nextSpawn.getClass().getSimpleName() + " requested Chunk @ " + nextSpawn.getPos() + " but the chunk was outside the map!");
				}
			}
			spawnQueue.clear();
		}
	}
	
	public void spawnObject(PhysicsObject object) {
		spawnQueue.add(object);
	}
	
	private boolean isPointInWorld(double x, double y) {
		return x >= 0 && y >= 0 && x <= WORLD_WIDTH && y <= WORLD_HEIGHT;
	}
	
	public Player getPlayer(int playerNum) {
		// Player 0 sounds weird, so player numbers start at 1. Might be a bad convention...
		return players.get(playerNum - 1);
	}
	
	public Player getRandomPlayer() {
		return getPlayer(GameLoop.RANDOM.nextInt(maxPlayers) + 1);
	}
	
	public ArrayList<Chunk> getNeighbourChunks(double x, double y) {
		ArrayList<Chunk> neighbourChunks = new ArrayList<Chunk>();
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				/*
				if (i == 0 && j == 0) {
					continue;
				}
				*/
				double neighbourChunkX = x + i*CHUNK_SIZE;
				double neighbourChunkY = y + j*CHUNK_SIZE;
				if (isPointInWorld(neighbourChunkX, neighbourChunkY)) {
					neighbourChunks.add(getChunk(neighbourChunkX, neighbourChunkY));
				}
			}
		}
		return neighbourChunks;
	}
	
	public ArrayList<Chunk> getNeighbourChunks(Vector point) {
		return getNeighbourChunks(point.getX(), point.getY());
	}
	
	public Chunk getChunk(double x, double y) {
		if (isPointInWorld(x, y)) {
			return chunks[(int) (x / CHUNK_SIZE)][(int) (y / CHUNK_SIZE)];
		}
		return null;
	}
	
	private Chunk getChunk(Vector pos) {
		return getChunk(pos.getX(), pos.getY());
	}
	
	public void reduceEnemyCount(Enemy enemy) {
		enemies.remove(enemy);
		System.out.println("Killed an enemy! Enemies left: " + enemies.size());
	}
	
	public ArrayList<Enemy> getEnemies() {
		return enemies;
	}
	
	public boolean isPaused() {
		return isPaused;
	}
	
	public int getWave() {
		return wave;
	}
}
