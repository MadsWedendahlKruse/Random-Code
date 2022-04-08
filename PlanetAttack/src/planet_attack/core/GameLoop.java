package planet_attack.core;

import java.util.Random;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import planet_attack.GUI;

public class GameLoop {
	
	public static final int MAX_PLAYERS = 1;
	public static final int MAX_ENEMIES = 5;
	public static final int MAX_BULLETS_PLAYER = 250;
	public static final int MAX_BULLETS_ENEMIES = 30;
	public static final int MAX_ASTEROIDS = 150;
	public static final int MAX_POWER_UPS = 3;

	/**
	 * One tick is the time between each iteration of the game loop measured in milliseconds.
	 */
	public static final int TICK_TIME = 20;
	public static final double SECONDS_PER_TICK = (double) TICK_TIME / 1000;
	
	/**
	 * The current tick. Incremented after each iteration of the game loop.
	 */
	private static int currentTick = 0;
	
	public static final long SEED = System.currentTimeMillis();
	public static final Random RANDOM = new Random(SEED);
	
	public static final World WORLD = new World(MAX_PLAYERS, MAX_ENEMIES, MAX_BULLETS_PLAYER, MAX_BULLETS_ENEMIES,
			MAX_ASTEROIDS, MAX_POWER_UPS);
	public static final GUI GUI = new GUI(WORLD);
	
	public static void main(String[] args) {
		WORLD.randomize();
		final ScheduledExecutorService executorService = Executors.newSingleThreadScheduledExecutor();
		executorService.scheduleAtFixedRate(GameLoop::loop, 0, TICK_TIME, TimeUnit.MILLISECONDS);
		
	}

	public static void loop() {
		try {
			WORLD.update(currentTick);
			GUI.draw(currentTick);
			if (!WORLD.isPaused()) {
				currentTick++;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	/*
	public static int getCurrentTick() {
		return currentTick;
	}
	
	/*************************************************************************
	 * Functions for converting SI-units with time to ticks (maybe this should be
	 * somewhere else)
	 *************************************************************************/

	/**
	 * Converts seconds to ticks.
	 */
	public static int secondsToTicks(double seconds) {
		return (int) (seconds / SECONDS_PER_TICK);
	}

	/**
	 * Converts unit per second to unit per tick.
	 */
	public static double perSecondToPerTick(double unitPerSecond) {
		return unitPerSecond * SECONDS_PER_TICK;
	}

	/**
	 * Converts unit per second^2 to unit per tick^2.
	 */
	public static double perSecondSquaredToPerTickSquared(double unitPerSecondSquared) {
		return perSecondToPerTick(perSecondToPerTick(unitPerSecondSquared));
	}

	/**
	 * Converts ticks to seconds.
	 */
	public static double ticksToSeconds(int ticks) {
		return ticks * SECONDS_PER_TICK;
	}

	/**
	 * Converts unit per tick to unit per second.
	 */
	public static double perTickToPerSecond(double unitPerTick) {
		return unitPerTick / SECONDS_PER_TICK;
	}

	/**
	 * Converts unit per tick^2 to unit per second^2.
	 */
	public static double perTickSquaredToPerSecondSquared(double unitPerTickSquared) {
		return perTickToPerSecond(perTickToPerSecond(unitPerTickSquared));
	}
}
