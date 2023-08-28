#pragma once

#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "resources/resource_manager.hpp"
#include "engine/aabb.hpp"

namespace rpg {
namespace resources {

/**
 * @class GameRegistry
 * @brief The game registry.
 * 
 * The game registry is a singleton that manages game data.
 * This includes tiles, objects, entities, etc.
 * Relevant data for each entry is stored in a struct, and
 * can be accessed using the name of the entry, 
 * e.g. GameRegistry::getInstance().getTileData("grass").sprite_rect 
*/
class GameRegistry {
public:
    /**
     * @brief Get the GameRegistry instance.
    */
    static GameRegistry& getInstance();
    
    static constexpr const char* TILE_PREFIX = "tile";
    /**
     * @brief The data needed to construct a tile.
    */
    struct TileData {
        /**
         * The name of the tile, e.g. "tile.grass". This is used to identify the tile.
         * The name has be unique. If the name already exists, the tile will not be registered.
        */
        std::string registry_name;
    };
    TileData createTileData(const std::string &name, const std::string &prefix);
    TileData getTileData(const std::string &name) const;

    static constexpr const char* OBJECT_PREFIX = "object";
    static constexpr const char* OBJECT_JSON_SUFFIX = ".object.json";
    /**
     * @brief The data needed to construct an object.
     * Object is quite vague, maybe it should be GameObjectData?
    */
    struct ObjectData : public TileData {
        /**
         * The footprint of the object. This is optional.
         * The footprint is used for collision detection.
         * If no footprint is provided, the footprint will default to the bottom half of the object's sprite.
         * The coordinates of the footprint are relative to the object's position.
        */
        engine::AABB footprint;
    };
    ObjectData createObjectData(const std::string &name, const std::string &prefix);
    ObjectData getObjectData(const std::string &name) const;

    static constexpr const char* ENTITY_PREFIX = "entity";
    static constexpr const int MAX_HEALTH_DEFAULT = 10;
    /**
     * @brief The data needed to construct an entity.
    */
    struct EntityData : public ObjectData {
        /**
         * The maximum health of the entity. This is optional.
         * If no maximum health is provided, the maximum health will default to MAX_HEALTH_DEFAULT.
        */
        unsigned int max_health = MAX_HEALTH_DEFAULT;
        std::map<std::string, Animation&> animations;
    };
    EntityData createEntityData(const std::string &name, const std::string &prefix);
    EntityData getEntityData(const std::string &name) const;

    /**
     * @brief Register an entry.
     * @param registry_name The registry name of the entry.
     * This is used to identify the entry. The registry name has to be unique.
     * If the registry name already exists, the entry will not be registered.
     * The prefix of the registry name is used to determine the type of the entry.
     * For example, if the registry name is "tile.grass", the prefix is "tile",
     * and the entry will be registered as a tile.
    */
    void registerEntry(const std::string &registry_name);

    /**
     * @brief Load a texture.
     * @param name The name of the texture.
     * @param prefix The prefix of the texture.
    */
    void loadTexture(const std::string &name, const std::string &prefix);
    inline ResourceManager& getResourceManager() { return resource_manager; }

    inline sf::Font& getFont() { return font; }

    inline void buildTextureAtlas() { resource_manager.buildTextureAtlas(); }
    inline const sf::Texture& getTextureAtlas() { return resource_manager.getTextureAtlas(); }

    /**
     * @brief Get the path to the resources folder.
     * @return The path to the resources folder.
    */
    static std::string getResourcesFolder();

    GameRegistry(GameRegistry const&) = delete;
    void operator=(GameRegistry const&) = delete;
private:
    GameRegistry();

    std::unordered_map<std::string, TileData> tiles;
    std::unordered_map<std::string, ObjectData> objects;
    std::unordered_map<std::string, EntityData> entities;
    /**
     * @brief The sprite manager.
    */
    ResourceManager resource_manager;
    /**
     * @brief The font.
     * TODO: Move this somewhere else?
    */
    sf::Font font;

    template <typename DataType>
    DataType getData(const std::string &name, const std::string &prefix, const std::unordered_map<std::string, DataType> &container) const {
        std::string key = name;
        // Check if the name contains the prefix
        if (name.find(prefix) != 0) {
            key = getRegistryName(name, prefix);
        }
        auto it = container.find(key);
        if (it == container.end()) {
            std::cerr << prefix << " not found: " << name << std::endl;
            return {};
        }
        return it->second;
    }
    /**
     * @brief Get the registry name from a name and prefix.
     * @param name The name.
     * @param prefix The prefix.
     * @return The registry name.
    */
    static std::string getRegistryName(const std::string &name, const std::string &prefix);
    /**
     * @brief Split a registry name into the name and prefix.
     * @param registry_name The registry name.
     * @param name The name (out).
     * @param prefix The prefix (out).
    */
    static void splitRegistryName(const std::string &registry_name, std::string &name, std::string &prefix);
    /**
     * @brief Get the directory for a registry name.
     * @param registry_name The registry name.
     * @return The directory.
    */
    static std::string getDirectory(const std::string &registry_name);
    /**
     * @brief Generate a texture path.
     * @param name The name of the entry to generate a texture path for.
     * @return The texture path.
    */
    static std::string generateTexturePath(const std::string &name, const std::string &prefix);
};

} // namespace resources
} // namespace rpg