package planet_attack.core;

import java.util.ArrayList;
import java.util.Iterator;

import planet_attack.GUI;
import planet_attack.Player;
import planet_attack.StdDraw;
import planet_attack.bullets.Bullet;
import planet_attack.bullets.BulletBounce;
import planet_attack.math.AABB;
import planet_attack.math.Vector;

/**
 * Game world is divided into chunks of a predefined size for optimisation
 * reasons. Things like collision detection only has to be checked within a
 * chunk, instead of checking all objects against everything else. Drawing
 * objects can also be limited to drawing the objects that area within the
 * chunks that the camera can see.
 */
public class Chunk {

	/**
	 * Width/height of a chunk
	 */
	public static final int SIZE = World.CHUNK_SIZE;

	/**
	 * All of the objects currently in the chunk
	 */
	private ArrayList<PhysicsObject> objects;

	private AABB chunkAabb;

	private World world;

	/**
	 * Constructs a chunk containing the given objects.
	 * 
	 * @param x x-coordinate of the bottom left corner of the Chunk
	 * @param y y-coordinate of the bottom left corner of the Chunk
	 */
	public Chunk(int x, int y, ArrayList<PhysicsObject> objects, World world) {
		this.objects = objects;
		this.world = world;
		chunkAabb = new AABB(x, y, SIZE, SIZE);
	}

	public Chunk(int x, int y, World world) {
		this(x, y, new ArrayList<PhysicsObject>(), world);
	}

	/**
	 * Update all objects in the chunk
	 */
	public void update(int currentTick) {
		for (Iterator<PhysicsObject> iterator = objects.listIterator(); iterator.hasNext();) {
			PhysicsObject object = (PhysicsObject) iterator.next();
			// Ensures we only update each object once per tick
			if (currentTick > object.getLastTick()) {
				object.update(currentTick);
			}
			// Make sure we only bounce an object on the edge once
			boolean hasBounced = false;
			// Position of the objects AABB next tick
			AABB objectAabbNextTick = new AABB(object.getAABB());
			objectAabbNextTick.move(object.getVel());
			// Check which chunks the corners of the AABB are in
			Vector[] cornersNextTick = objectAabbNextTick.getCorners();
			for (Vector corner : cornersNextTick) {
				Chunk chunkNextTick = world.getChunk(corner.getX(), corner.getY());
				if (chunkNextTick != null) {
					chunkNextTick.add(object);
				} else {
					if (object instanceof Bullet && !(object instanceof BulletBounce)) {
						continue;
					}
					if (!hasBounced) {
						bounceOnBorder(object);
						hasBounced = true;
					}
				}
			}
			// Remove object from the chunk if it doesn't exist or isn't within the chunk
			if (!object.exists() || !object.isColliding(chunkAabb)) {
				iterator.remove();
			}
		}
	}

	/**
	 * Draw all objects in the chunk that are within view of the game camera.
	 */
	public void draw(int currentTick) {
		if (GUI.DRAW_DEBUG) {
			chunkAabb.draw();
		}
		for (PhysicsObject object : objects) {
			// Only draw if it visible by the camera and hasn't been drawn in this tick
			if (object.isColliding(GUI.CAMERA_AABB) && currentTick == object.getLastTick()) {
				if (!object.isDrawn() || currentTick == 0) {
					object.draw();
					object.setDrawn(true);
				}
				if (GUI.DRAW_DEBUG) {
					if (object instanceof Player) {
						chunkAabb.draw(StdDraw.YELLOW);
					}
					Vector center = getCenter();
					StdDraw.setPenColor(StdDraw.BLUE);
					GUI.line(center.getX(), center.getY(), object.getX(), object.getY());
				}
			}
		}
	}

	public void checkCollisions() {
		if (objects.size() > 1) {
			for (int i = 0; i < objects.size(); i++) {
				for (int j = i + 1; j < objects.size(); j++) {
					if (objects.get(i).isColliding(objects.get(j))) {
						objects.get(i).doCollision(objects.get(j));
					}
				}
			}
		}
	}
	
	public void bounceOnBorder(PhysicsObject object) {
		if (object.getX() > World.WORLD_WIDTH || object.getX() < 0) {
			object.setVelX(-object.getVelX());
		}
		if (object.getY() > World.WORLD_HEIGHT || object.getY() < 0) {
			object.setVelY(-object.getVelY());
		}
	}
	
	public void add(PhysicsObject object) {
		if (!objects.contains(object) && object.exists()) {
			objects.add(object);
		}
	}

	/**
	 * @return Centre-point of the chunk
	 */
	public Vector getCenter() {
		return chunkAabb.getCenter();
	}

	public boolean isEmpty() {
		return objects.isEmpty();
	}

	public AABB getChunkAabb() {
		return chunkAabb;
	}
	
	@Override
	public String toString() {
		return "Chunk " + chunkAabb.toString();
	}
	
	public ArrayList<PhysicsObject> getObjects() {
		return objects;
	}
}
