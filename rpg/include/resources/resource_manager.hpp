#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <filesystem>

#include "resources/animation.hpp"
#include "resources/weighted_texture.hpp"
#include "resources/connected_textures/connected_texture.hpp"
#include "resources/vertex_quad.hpp"

namespace rpg {
namespace resources {

/**
 * @class ResourceManager
 * @brief The resource manager.
 * 
 * The resource manager is used to load and store resources 
 * such as textures, animations, etc.
*/
class ResourceManager {
public:
    // Only allow GameRegistry to access the constructor.
    friend class GameRegistry;
    /**
     * @brief Load a texture.
     * The texture will be added to the texture atlas when buildTextureAtlas() is called.
     * @param path The path to the texture.
     * @param registry_name The key to assign to the texture, e.g. "tile.grass".
    */
    void loadTexture(const std::filesystem::path &path, const std::string &registry_name = "");
    /**
     * @brief Get the texture rect of a texture.
     * @param registry_name The key of the texture, e.g. "tile.grass".
     * @return The texture rect.
    */
    const sf::IntRect getTextureRect(const std::string &registry_name);
    /**
     * @brief Get the vertex quad of a texture.
     * @param registry_name The key of the texture, e.g. "tile.grass".
     * @return The vertex quad.
    */
    const VertexQuad& getVertexQuad(const std::string &registry_name);
    /**
     * @brief Check if an animation exists.
     * @param registry_name The key of the animation, e.g. "entity.player_walk_down".
     * @return True if the animation exists, false otherwise.
    */
    bool hasAnimation(const std::string &registry_name);
    /**
     * @brief Get a specific animation.
     * @param registry_name The key of the animation, e.g. "entity.player_walk_down".
     * @return The animation.
    */
    Animation& getAnimation(const std::string &registry_name);
    /**
     * @brief Check if a texture has variations.
     * @param registry_name The key of the texture, e.g. "tile.grass".
     * @return True if the texture has variations, false otherwise.
    */
    bool hasVariations(const std::string &registry_name);
    /**
     * @brief Get a collection of variations of a texture.
     * @param registry_name The key of the texture, e.g. "tile.grass".
     * @return The texture collection.
    */
    std::shared_ptr<WeightedTexture> getVariations(const std::string &registry_name);
    /**
     * @brief Check if a texture has connected textures.
     * @param registry_name The key of the texture, e.g. "tile.grass".
     * @return True if the texture has connected textures, false otherwise.
    */
    bool hasConnectedTexture(const std::string &registry_name);
    /**
     * @brief Get a connected texture.
     * @param registry_name The key of the texture, e.g. "tile.grass".
     * @return The connected texture.
    */
    std::shared_ptr<connected_textures::ConnectedTexture> getConnectedTexture(const std::string &registry_name);
    /**
     * @brief Build the texture atlas.
     * This is used to store all of the textures.
     * This should only be called once after all of the textures have been loaded.
     * TODO: Maybe this should be called automatically when the first texture is loaded?
    */
    void buildTextureAtlas();
    /**
     * @brief Get the texture atlas.
     * @return The texture atlas.
    */
    const inline sf::Texture& getTextureAtlas() const { return texture_atlas; }

    // Singleton stuff
    ResourceManager(ResourceManager const&) = delete;
    void operator=(ResourceManager const&) = delete;
private:
    /**
     * @brief Construct a new ResourceManager object.
     * This is private because this is a singleton class.
    */
    ResourceManager();
    /**
     * @brief A map of registry names to textures.
    */
    std::map<std::string, sf::Texture> textures;
    /**
     * @brief The default texture.
     * This is returned if a texture isn't found.
    */
    sf::Texture default_texture;
    /**
     * @brief A map of registry names to vertex quads.
    */
    std::map<std::string, VertexQuad> vertex_quads;
    /**
     * @brief The default vertex quad.
     * This is returned if the quad isn't found.
    */
    VertexQuad default_vertex_quad;
    /**
     * @brief A map of registry names to animations.
    */
    std::map<std::string, Animation> animations;
    /**
     * @brief The suffix for animation files.
    */
    static constexpr const char* ANIMATION_EXTENSION = ".animation.json";
    /**
     * @brief A map of registry names to texture collections.
     * This is used for variations of the same texture, e.g. a grass tile with
     * different variations of grass.
    */
    std::map<std::string, std::shared_ptr<WeightedTexture>> variations;
    /**
     * @brief The suffix for variation files.
    */
    static constexpr const char* VARIATIONS_EXTENSION = ".variations.json";
    /**
     * @brief A map of registry names to connected textures.
     * This is used for connected textures, e.g. a dirt tile next to a grass tile.
    */
    std::map<std::string, std::shared_ptr<connected_textures::ConnectedTexture>> connected_textures;
    /**
     * @brief The suffix for connected texture files.
    */
    static constexpr const char* CONNECTED_TEXTURE_BLOB_EXTENSION = "_connected_blob.png";
    static constexpr const char* CONNECTED_TEXTURE_FENCE_EXTENSION = "_connected_fence.png";
    /**
     * @brief The texture atlas.
     * This is used to store all of the textures.
    */
    sf::Texture texture_atlas;
    /**
     * @brief Set the texture for a registry name.
     * The texture is NOT added to the texture atlas here, it is only added to
     * the map of textures. The texture atlas is built when buildTextureAtlas() is called.
     * @param registry_name The key of the texture, e.g. "tile.grass".
     * @param texture The texture.
     * @param texture_rect The texture rect. 
     * The position of the rect must be relative to the given texture. Once the texture
     * atlas is built, the position of the rect will be moved to the position of the
     * texture in the texture atlas. The size of the rect will remain the same.
    */
    void setTexture(const std::string &registry_name, const sf::Texture &texture, const sf::IntRect &texture_rect);
    /**
     * @brief Get a texture by its key, e.g. "tile.grass".
     * If the texture isn't already loaded, the default texture is returned.
     * @param registry_name The key of the texture.
     * @return The texture.
    */
    const sf::Texture& getTexture(const std::string &registry_name);
    /**
     * @brief Load the extra files associated with a texture.
     * This loads animations, variations, connected textures, etc.
     * This also checks if the extra files exist.
     * @param path The path to the texture file.
     * @param registry_name The key of the texture, e.g. "tile.grass".
     * @return True if any of the extra files were exist (and were loaded), false otherwise.
    */
    bool loadExtraFiles(const std::filesystem::path &path, const std::string &registry_name);
    /**
     * @brief Get the minimum rect for an image.
     * This is used to crop the image to remove any transparent pixels.
     * The size of the rect is the size of the image in the texture atlas.
     * @param image The image.
     * @return The minimum rect.
    */
    const sf::IntRect getMinimumRect(const sf::Image &image);
    /**
     * @brief Crop an image.
     * This crops the image to the specified rect.
     * @param image The image.
     * @param rect The rect to crop the image to.
     * @return The cropped image.
    */
    const sf::Image cropImage(const sf::Image &image, const sf::IntRect &rect);
    /**
     * @brief Extract the rects (subimages) from a texture.
     * This is used for animations and variations.
     * @param texture The texture.
     * @return The rects.
    */
    std::vector<sf::IntRect> getRects(const sf::Texture &texture);
    /**
     * @brief Split a texture into frames.
     * This is used for animations and variations.
     * @param source_texture The source texture.
     * @param frame_width The width of each frame.
     * @param frame_height The height of each frame.
     * @return The frames.
    */
    std::vector<sf::Image> splitTexture(const sf::Texture& source_texture, int frame_width, int frame_height);
    /**
     * @brief Load the animation for a texture.
     * @param path The path to the animation file.
        * @param registry_name The key of the animation, e.g. "entity.player_idle_down".
    */
    void loadAnimation(const std::filesystem::path &path, const std::string &registry_name);
    /**
     * @brief Load the variations for a texture.
     * @param path The path to the variation file.
     * @param registry_name The key of the texture, e.g. "tile.grass".
    */
    void loadVariations(const std::filesystem::path &path, const std::string &registry_name);
    /**
     * @brief Load the connected texture for a texture.
     * @param path The path to the connected texture.
     * @param registry_name The key of the texture, e.g. "tile.grass".
    */
    void loadConnectedTexture(const std::filesystem::path &path, const std::string &registry_name);
};

} // namespace resources
} // namespace rpg