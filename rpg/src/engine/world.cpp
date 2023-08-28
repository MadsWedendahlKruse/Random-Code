#include "engine/world.hpp"
#include "engine/mobile_object.hpp"
#include "FastNoiseLite.h"

namespace rpg {
namespace engine {

World::World(const sf::Vector2i &dimensions, int seed) 
    : dimensions(dimensions),
    // This feels like a pretty janky way to create the border...
    world_border{AABB(sf::Vector2f(0, 0), sf::Vector2f(dimensions.x, 0)), 
                 AABB(sf::Vector2f(0, 0), sf::Vector2f(0, dimensions.y)),
                 AABB(sf::Vector2f(0, dimensions.y), sf::Vector2f(dimensions.x, 0)), 
                 AABB(sf::Vector2f(dimensions.x, 0), sf::Vector2f(0, dimensions.y))} {
    this->seed = seed;
    // Allocate the tiles vector
    tiles.resize((dimensions.x + 1) * (dimensions.y + 1));
    // Generate the world
    // generateWorld();
    // Noise generation
    FastNoiseLite noise_generator;
    noise_generator.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise_generator.SetSeed(seed);
    noise_generator.SetFrequency(0.01f);
    noise_generator.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise_generator.SetFractalOctaves(6);
    noise_generator.SetFractalLacunarity(2.0f);
    noise_generator.SetFractalGain(0.5f);
    float bush_threshold = 0.3f;
    float grass_threshold = 0.1f;
    float sand_threshold = 0.0f;
    float water_shallow_threshold = -0.1f;
    for (int y = 0; y < dimensions.y; y++) {
        for (int x = 0; x < dimensions.x; x++) {
            sf::Vector2i position(x, y);
            float noise = noise_generator.GetNoise((float) x, (float) y);
            if (noise > bush_threshold) {
                // createGameObject(position, "object.bush_short");
                createTile(position, "tile.grass");
            } else if (noise > grass_threshold) {
                createTile(position, "tile.grass");
            } else if (noise > sand_threshold) {
                createTile(position, "tile.sand");
            } else if (noise > water_shallow_threshold) {
                createTile(position, "tile.water_shallow");
            } else {
                createTile(position, "tile.water");
            }
        }
    }
}

void World::update(float delta) {
    last_delta = delta;
    player->update(delta);
    for (auto &game_object : game_objects) {
        // Check if the game object is a mobile object
        MobileObject* mobile_object = static_cast<MobileObject*>(game_object.get());
        if (mobile_object != nullptr) {
            // Update the mobile object
            mobile_object->update(delta);
        }
    }
    // Check if the player is colliding with the world border
    for (auto &border : world_border) {
        if (player->isColliding(border)) {
            // Resolve the collision
            player->resolveCollision(border);
        }
    }
    // Check if the player collided with any game objects
    for (auto &game_object : game_objects) {
        if (player->isColliding(*game_object.get())) {
            // Resolve the collision
            player->resolveCollision(*game_object.get());
        }
    }
    // Sort the drawables by y position so that they are drawn in the correct order
    std::sort(drawables.begin(), drawables.end(), [](GameObject* a, GameObject* b) -> bool {
        return a->getAABB().getPosition().y < b->getAABB().getPosition().y;
    });
}

void World::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::VertexArray vertex_array(sf::PrimitiveType::Quads);
    // Get the viewport of the view
    sf::FloatRect viewport = getViewport(target.getView());
    // Draw the tiles as the background
    for (const auto& tile : getVisibleTiles(viewport)) {
        for (const auto& vertex : tile->getVertices()) {
            vertex_array.append(vertex);
        }
    }
    // Draw the drawables
    for (auto &drawable : drawables) {
        // Check if the drawable is in the view
        if (!viewport.intersects(drawable->getAABB())) {
            continue;
        }
        for (const auto& vertex : drawable->getVertices()) {
            vertex_array.append(vertex);
        }
    }
    // Draw the vertex array
    states.texture = &game_registry.getTextureAtlas();
    target.draw(vertex_array, states);
}

void World::drawDebug(sf::RenderTarget &target, sf::RenderStates states) const {
    // Get the viewport of the view
    sf::FloatRect viewport = getViewport(target.getView());
    // Draw a grid
    sf::RectangleShape grid_line;
    grid_line.setFillColor(sf::Color::Transparent);
    grid_line.setSize(sf::Vector2f(1, 1));
    grid_line.setOutlineColor(sf::Color::Green);
    grid_line.setOutlineThickness(0.025f);
    for (int i = 0; i <= viewport.width; i++) {
        for (int j = 0; j <= viewport.height; j++) {
            grid_line.setPosition((int) viewport.left + i, (int) viewport.top + j);
            target.draw(grid_line, states);
        }
    }
    // Draw the world border (for debugging)
    for (auto &border : world_border) {
        target.draw(border, states);
    }
    // Draw the drawables
    for (auto &drawable : drawables) {
        // Check if the drawable is in the view
        if (!viewport.intersects(drawable->getAABB())) {
            continue;
        }
        // drawable->drawDebug(target, states);
    }
}

void World::createTile(const sf::Vector2i& position, const std::string &registry_name) {
    Tile tile(sf::Vector2f(position), game_registry.getTileData(registry_name));
    tiles[position.x + position.y * dimensions.x] = tile;
    // This would probably be more efficient to do for all the tiles once we've added them all
    // For editing the world in real time this is a lot nicer though
    // connectTileToNeighbours(tile);
    // for (auto &neighbour : getTileNeighbours(tile)) {
    //     if (neighbour != nullptr) {
    //         connectTileToNeighbours(*neighbour);
    //     }
    // }
}

void World::createGameObject(const sf::Vector2i& position, const std::string &registry_name) {
    std::shared_ptr<GameObject> game_object = std::make_shared<GameObject>(sf::Vector2f(position), game_registry.getObjectData(registry_name));
    game_objects.push_back(game_object);
    // GameObject *game_object_ptr = const_cast<GameObject*>(&game_object);
    // drawables.push_back(game_object_ptr);
    drawables.push_back(game_object.get());
}

void World::createPlayer(const sf::Vector2i& position) {
    this->player = std::make_unique<Player>(sf::Vector2f(position), game_registry.getEntityData("entity.player"));
    GameObject *player_ptr = this->player.get();
    drawables.push_back(player_ptr);
}

void World::updateView(sf::View &view) const {
    // Update the view to follow the player
    view.setCenter(player->getPosition());
    // Check if the view is outside the world
    if (view.getCenter().x - view.getSize().x / 2.0f < 0) {
        // Set the view to the left edge of the world
        view.setCenter(view.getSize().x / 2.0f, view.getCenter().y);
    }
    if (view.getCenter().x + view.getSize().x / 2.0f > dimensions.x) {
        // Set the view to the right edge of the world
        view.setCenter(dimensions.x - view.getSize().x / 2.0f, view.getCenter().y);
    }
    if (view.getCenter().y - view.getSize().y / 2.0f < 0) {
        // Set the view to the top edge of the world
        view.setCenter(view.getCenter().x, view.getSize().y / 2.0f);
    }
    if (view.getCenter().y + view.getSize().y / 2.0f > dimensions.y) {
        // Set the view to the bottom edge of the world
        view.setCenter(view.getCenter().x, dimensions.y - view.getSize().y / 2.0f);
    }
    // Check if the current view is bigger than the world
    if (view.getSize().x > dimensions.x) {
        // Set the view to the width of the world
        view.setCenter(dimensions.x / 2.0f, view.getCenter().y);
    }
    if (view.getSize().y > dimensions.y) {
        // Set the view to the height of the world
        view.setCenter(view.getCenter().x, dimensions.y / 2.0f);
    }
}

bool World::isPointInWorld(const sf::Vector2f &point) const {
    return point.x >= 0 && point.x <= dimensions.x &&
           point.y >= 0 && point.y <= dimensions.y;
}

const Tile* World::getTile(int x, int y) const {
    if (x < 0 || x >= dimensions.x || y < 0 || y >= dimensions.y) {
        return nullptr;
    }
    return &tiles[x + y * dimensions.x];
}

std::vector<const Tile*> World::getTileNeighbours(const Tile &tile) const {
    std::vector<const Tile*> neighbours;
    sf::Vector2f position = tile.getPosition();
    // Define directions in order: N, NE, E, SE, S, SW, W, NW
    std::vector<sf::Vector2f> directions = {
        {0, -1}, {1, -1}, {1, 0}, {1, 1},
        {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}
    };
    for (const auto& direction : directions) {
        if (!isPointInWorld(position + direction)) {
            neighbours.push_back(nullptr);
            continue;
        }
        neighbours.push_back(getTile(position.x + direction.x, position.y + direction.y));
    }
    return neighbours;
}

void World::connectTileToNeighbours(Tile &tile) {
    std::vector<const Tile*> neighbours_tiles = getTileNeighbours(tile);
    resources::connected_textures::ConnectedTexture::Neighbours neighbours;
    // This is a bit of a hack, but it works for now
    neighbours.N = neighbours_tiles[0] != nullptr && neighbours_tiles[0]->getRegistryName() == tile.getRegistryName();
    neighbours.NE = neighbours_tiles[1] != nullptr && neighbours_tiles[1]->getRegistryName() == tile.getRegistryName();
    neighbours.E = neighbours_tiles[2] != nullptr && neighbours_tiles[2]->getRegistryName() == tile.getRegistryName();
    neighbours.SE = neighbours_tiles[3] != nullptr && neighbours_tiles[3]->getRegistryName() == tile.getRegistryName();
    neighbours.S = neighbours_tiles[4] != nullptr && neighbours_tiles[4]->getRegistryName() == tile.getRegistryName();
    neighbours.SW = neighbours_tiles[5] != nullptr && neighbours_tiles[5]->getRegistryName() == tile.getRegistryName();
    neighbours.W = neighbours_tiles[6] != nullptr && neighbours_tiles[6]->getRegistryName() == tile.getRegistryName();
    neighbours.NW = neighbours_tiles[7] != nullptr && neighbours_tiles[7]->getRegistryName() == tile.getRegistryName();
    tile.connectTexture(neighbours);
}

sf::FloatRect World::getViewport(const sf::View &view) const {
    // It seems really weird that we have to construct the viewport "manually"
    sf::Vector2f view_size = view.getSize();
    sf::Vector2f view_center = view.getCenter();
    // -1 and + 1 is to ensure the tiles on the edge of the view are drawn
    sf::FloatRect viewport(view_center.x - view_size.x / 2.0f - 1, view_center.y - view_size.y / 2.0f - 1, view_size.x + 1, view_size.y + 1);
    return viewport;
}

std::vector<const Tile*> World::getVisibleTiles(const sf::FloatRect &viewport) const {
    std::vector<const Tile*> visible_tiles;
    int x_start = std::max(0, (int) viewport.left);
    int x_end = std::min((int) (viewport.left + viewport.width), dimensions.x);
    int y_start = std::max(0, (int) viewport.top);
    int y_end = std::min((int) (viewport.top + viewport.height), dimensions.y);
    
    for (int x = x_start; x <= x_end; x++) {
        for (int y = y_start; y <= y_end; y++) {
            visible_tiles.push_back(getTile(x, y));
        }
    }
    return visible_tiles;
}


void World::generateWorld() {
    std::vector<sf::Vector2i> initial_perimeter = generatePerimeter();
    std::vector<sf::Vector2i> perimeter;
    // Connect the points in the perimeter
    for (int i = 0; i < initial_perimeter.size() - 1; i++) {
        std::vector<sf::Vector2i> line = calculateLine(initial_perimeter[i], initial_perimeter[i + 1]);
        // Insert the line into the perimeter
        perimeter.insert(perimeter.end(), line.begin(), line.end());
    }
    // Connect the last point to the first point
    std::vector<sf::Vector2i> line = calculateLine(initial_perimeter[initial_perimeter.size() - 1], initial_perimeter[0]);
    perimeter.insert(perimeter.end(), line.begin(), line.end());
    // Convert the perimeter to a mask
    std::vector<std::vector<bool>> perimeter_mask = pointsToPerimeterMask(perimeter);
    // Flood fill the world
    std::vector<std::vector<bool>> flood_mask = std::vector<std::vector<bool>>(dimensions.x, std::vector<bool>(dimensions.y, false));
    floodFill(dimensions.x / 2, dimensions.y / 2, perimeter_mask, flood_mask);
    // Fill a circle of sand around each point along the perimeter
    std::vector<std::vector<bool>> sand_mask = std::vector<std::vector<bool>>(dimensions.x, std::vector<bool>(dimensions.y, false));
    for (auto &point : perimeter) {
        std::vector<sf::Vector2i> circle_points = getCirclePoints(point, rand() % 10);
        for (auto &circle_point : circle_points) {
            sand_mask[circle_point.x][circle_point.y] = true;
        }
    }
    // Create the tiles
    for (int x = 0; x < dimensions.x; x++) {
        for (int y = 0; y < dimensions.y; y++) {
            sf::Vector2i position(x, y);
            if (flood_mask[x][y]) {
                createTile(position, "tile.grass");
            } else if (sand_mask[x][y]) {
                createTile(position, "tile.sand");
            } else {
                createTile(position, "tile.water");
            }
        }
    }
}

std::vector<sf::Vector2i> World::generatePerimeter() {
    std::vector<sf::Vector2i> perimeter;
    // Center of the world
    sf::Vector2i center(dimensions.x / 2, dimensions.y / 2);
    // Noise generation
    FastNoiseLite noise_generator;
    noise_generator.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise_generator.SetSeed(seed);
    noise_generator.SetFrequency(0.03f);
    noise_generator.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise_generator.SetFractalOctaves(6);
    noise_generator.SetFractalLacunarity(2.0f);
    noise_generator.SetFractalGain(0.5f);
    // float perimeter_threshold = 0.0f;
    // for (int x = 0; x < dimensions.x; x++) {
    //     for (int y = 0; y < dimensions.y; y++) {
    //         sf::Vector2i position(x, y);
    //         float noise = noise_generator.GetNoise((float) x, (float) y);
    //         // Distance from the center of the world
    //         float distance = std::sqrt(std::pow(x - center.x, 2) + std::pow(y - center.y, 2));
    //         float normalized_distance = distance / (2.f * dimensions.x / 5.f);
    //         // Create a radial gradient that eases towards the center
    //         float gradient = std::cos(normalized_distance * M_PI / 2.f);
    //         // Combine the gradient with the noise
    //         float combined_value = noise * gradient;
    //         if (combined_value > perimeter_threshold) {
    //             createTile(position, "tile.grass");
    //         } else {
    //             createTile(position, "tile.water");
    //         }
    //     }
    // }
    int num_points = 500; // Number of points on the perimeter
    float noise_amplitude = 50.0f; // Amplitude of noise
    // float radius = dimensions.x * 2.f / 5.f; // Base radius of the circle
    float radius = 100.f; // Base radius of the circle

    // FastNoiseLite noise_generator;
    // noise_generator.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    for (int i = 0; i < num_points; i++) {
        float angle = (2.0f * M_PI * i) / num_points; // Angle in radians
        float noise_sample = noise_generator.GetNoise((float) i, (float) i); // Sample noise in [0, 1)
        float r = radius + noise_amplitude * noise_sample; // Modify radius with noise
        float x_pos = center.x + r * cos(angle);
        float y_pos = center.y + r * sin(angle);
        sf::Vector2i position((int) x_pos, (int) y_pos);
        perimeter.push_back(position);
    }
    return perimeter;
}

std::vector<sf::Vector2i> World::calculateLine(const sf::Vector2i &start, const sf::Vector2i &end) {
    std::vector<sf::Vector2i> line;
    int x0 = start.x;
    int y0 = start.y;
    int x1 = end.x;
    int y1 = end.y;
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;
    while (true) {
        line.push_back(sf::Vector2i(x0, y0));
        if (x0 == x1 && y0 == y1) {
            break;
        }
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
    return line;
}

std::vector<std::vector<bool>> World::pointsToPerimeterMask(const std::vector<sf::Vector2i> &points) {
    // Create the mask
    std::vector<std::vector<bool>> mask(dimensions.x, std::vector<bool>(dimensions.y, false));
    for (auto &point : points) {
        mask[point.x][point.y] = true;
    }
    return mask;
}

void World::floodFill(int x, int y, const std::vector<std::vector<bool>> &perimeter_mask, std::vector<std::vector<bool>> &filled_tiles) {
    // If the point is outside the bounds or already visited, return
    if (x < 0 || y < 0 || x >= perimeter_mask[0].size() || y >= perimeter_mask.size() || filled_tiles[x][y]) {
        return;
    }
    // If the point is part of the perimeter, return
    if (perimeter_mask[x][y]) {
        return;
    }
    // Mark the tile as filled
    filled_tiles[x][y] = true;
    // Recursively fill the neighboring tiles
    floodFill(x + 1, y, perimeter_mask, filled_tiles);
    floodFill(x - 1, y, perimeter_mask, filled_tiles);
    floodFill(x, y + 1, perimeter_mask, filled_tiles);
    floodFill(x, y - 1, perimeter_mask, filled_tiles);
}

std::vector<sf::Vector2i> World::getCirclePoints(const sf::Vector2i &center, int radius) {
    std::vector<sf::Vector2i> points;
    int x = 0;
    int y = radius;
    int m = 5 - 4 * radius;

    while (x <= y) {
        // Add all points in horizontal and vertical lines crossing through the current circumference points
        for (int i = -x; i <= x; i++) {
            points.emplace_back(center.x + i, center.y + y);
            points.emplace_back(center.x + i, center.y - y);
        }
        for (int i = -y; i <= y; i++) {
            points.emplace_back(center.x + i, center.y + x);
            points.emplace_back(center.x + i, center.y - x);
        }

        if (m > 0) {
            y--;
            m -= 8 * y;
        }

        x++;
        m += 8 * x + 4;
    }

    return points;
}


} // namespace engine
} // namespace rpg