#include "resources/game_registry.hpp"
#include "engine/constants.hpp"

#include <iostream>
#include <filesystem>
#include <fstream>

#include "rectpack2D-master/src/finders_interface.h"
#include "nlohmann/json.hpp"

namespace rpg {
namespace resources {

GameRegistry& GameRegistry::getInstance() {
    static GameRegistry instance;
    return instance;
}

GameRegistry::GameRegistry() {
    // TODO: Do this automatically
    // this->assets_path = "D:/Random-Code/rpg/assets";
    this->font.loadFromFile(getResourcesFolder() + "/fonts/PixeloidSans-mLxMm.TTF");
    // Register some stuff
    registerEntry("entity.player");
    registerEntry("object.rock");
    registerEntry("object.bush_short");
    registerEntry("object.bush_tall");
    registerEntry("object.tree_stump");
    registerEntry("object.crate");
    registerEntry("object.chest");
    registerEntry("tile.grass");
    registerEntry("tile.dirt");
    registerEntry("tile.water");
    registerEntry("tile.water_shallow");
    registerEntry("tile.sand");
    registerEntry("ui.button");
    registerEntry("ui.menu");
    buildTextureAtlas();
}

GameRegistry::TileData GameRegistry::createTileData(const std::string &name, const std::string &prefix) {
    TileData data;
    // Create the registry name
    std::string registry_name = getRegistryName(name, prefix);
    data.registry_name = registry_name;
    // Load the sprite
    loadTexture(name, prefix);
    // Get the sprite rect
    // data.sprite = &sprite_manager.getSprite(registry_name);
    return data;
}

GameRegistry::TileData GameRegistry::getTileData(const std::string &name) const {
    return getData(name, TILE_PREFIX, tiles);
}

GameRegistry::ObjectData GameRegistry::createObjectData(const std::string &name, const std::string &prefix) {
    TileData tile_data = createTileData(name, prefix);
    // Add tile data to object data
    ObjectData data;
    data.registry_name = tile_data.registry_name;
    // data.sprite = tile_data.sprite;
    // Data for the footprint
    sf::Vector2f footprint_position, footprint_dimensions;
    // Check if the object has a json file
    std::string object_json_path = getResourcesFolder() + "/" + OBJECT_PREFIX + "/" + name + OBJECT_JSON_SUFFIX;
    if (std::filesystem::exists(object_json_path)) {
        std::cout << "Found object file: " << object_json_path << std::endl;
        // Load the footprint from the json file
        std::ifstream file(object_json_path);
        nlohmann::json json = nlohmann::json::parse(file);
        auto footprint = json["footprint"];
        footprint_position = sf::Vector2f(footprint["position"]["x"], footprint["position"]["y"]);
        footprint_dimensions = sf::Vector2f(footprint["dimensions"]["width"], footprint["dimensions"]["height"]);
    } else {
        // Generate the footprint
        sf::IntRect texture_rect = resource_manager.getTextureRect(data.registry_name);
        footprint_position = sf::Vector2f(0, texture_rect.height / 2) * engine::constants::WORLD_SPRITE_SCALE;
        footprint_dimensions = sf::Vector2f(texture_rect.width, texture_rect.height / 2) * engine::constants::WORLD_SPRITE_SCALE;;
    }
    data.footprint = engine::AABB(footprint_position, footprint_dimensions);
    return data;
}

GameRegistry::ObjectData GameRegistry::getObjectData(const std::string &name) const {
    return getData(name, OBJECT_PREFIX, objects);
}

GameRegistry::EntityData GameRegistry::createEntityData(const std::string &name, const std::string &prefix) {
    ObjectData object_data = createObjectData(name, prefix);
    // Add object data to entity data
    EntityData data;
    data.registry_name = object_data.registry_name;
    // data.sprite = object_data.sprite;
    data.footprint = object_data.footprint;
    // Load all the animations for the entity
    std::string entity_folder = getResourcesFolder() + "/" + ENTITY_PREFIX + "/" + name;
    // Create a map of all the animations
    std::map<std::string, Animation&> animations;
    for (const auto &entry : std::filesystem::directory_iterator(entity_folder)) {
        std::filesystem::path path = entry.path();
        // Check if the file is an image
        if (!path.has_extension() || path.extension().string() != ".png") {
            continue;
        }
        std::string registry_name = getRegistryName(path.stem().string(), ENTITY_PREFIX);
        resource_manager.loadTexture(path, registry_name);
        // Check if the animation exists
        if (resource_manager.hasAnimation(registry_name)) {
            animations.insert({registry_name, resource_manager.getAnimation(registry_name)});
        }
    }
    data.animations = animations;
    return data;
}

GameRegistry::EntityData GameRegistry::getEntityData(const std::string &name) const {
    return getData(name, ENTITY_PREFIX, entities);
}

void GameRegistry::registerEntry(const std::string &registry_name) {
    // Split the registry name into the prefix and the name
    std::string name, prefix;
    splitRegistryName(registry_name, name, prefix);
    if (prefix == TILE_PREFIX) {
        TileData data = createTileData(name, prefix);
        tiles[registry_name] = data;
    } else if (prefix == OBJECT_PREFIX) {
        ObjectData data = createObjectData(name, prefix);
        objects[registry_name] = data;
    } else if (prefix == ENTITY_PREFIX) {
        EntityData data = createEntityData(name, prefix);
        entities[registry_name] = data;
    } else if (prefix == "ui") {
        // Not sure what to do here...
        // If it doesn't have any associated data just load the sprite?
        loadTexture(name, prefix);
        loadTexture(name + "_hovered", prefix);
        loadTexture(name + "_clicked", prefix);

    }
}

std::string GameRegistry::getRegistryName(const std::string &name, const std::string &prefix) {
    return prefix + "." + name;
}

void GameRegistry::splitRegistryName(const std::string &registry_name, std::string &name, std::string &prefix) {
    size_t pos = registry_name.find('.');
    if (pos == std::string::npos) {
        std::cerr << "Invalid registry name: " << registry_name << std::endl;
        return;
    }
    prefix = registry_name.substr(0, pos);
    name = registry_name.substr(pos + 1);
}

void GameRegistry::loadTexture(const std::string &name, const std::string &prefix) {
    std::string registry_name = getRegistryName(name, prefix);
    // Generate the texture path
    std::string texture_path = generateTexturePath(name, prefix);
    // Check if the path points to a valid file
    if (std::filesystem::exists(texture_path)) {
        // Load the texture
        resource_manager.loadTexture(texture_path, registry_name);
    } else {
        // If the texture doesn't exist try to see if it has any animations, variations or connected textures
        resource_manager.loadExtraFiles(texture_path, registry_name);
    }
}

std::string GameRegistry::getResourcesFolder() {
    // Do something more elegant here
    return "D:/Random-Code/rpg/resources";
}

std::string GameRegistry::getDirectory(const std::string &registry_name) {
    std::string name, prefix;
    splitRegistryName(registry_name, name, prefix);
    if (prefix == GameRegistry::ENTITY_PREFIX) {
        return getResourcesFolder() + "/" + prefix + "/" + name;
    }
    return getResourcesFolder() + "/" + prefix;
}

std::string GameRegistry::generateTexturePath(const std::string &name, const std::string &prefix) {
    /**
     * In the future this might have to be more specific to each of the
     * different types of assets, e.g. tiles might be part of some kind of
     * tileset, and the path would have to be something like this:
     * "assets/textures/tilesets/<tileset_name>/<name>.png"
    */
    // if (prefix == TILE_PREFIX) {
    //     return getResourcesFolder() + "/textures/" + prefix + "/" + name + ".png";
    // }
    // if (prefix == OBJECT_PREFIX) {
    //     return getResourcesFolder() + "/textures/" + prefix + "/" + name + ".png";
    // }
    if (prefix == GameRegistry::ENTITY_PREFIX) {
        return getResourcesFolder() + "/" + prefix + "/" + name + "/" + name + ".png";
    }
    return getResourcesFolder() + "/" + prefix + "/" + name + ".png";
}

} // namespace resources
} // namespace rpg