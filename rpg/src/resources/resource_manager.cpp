#include "resources/resource_manager.hpp"
#include "resources/game_registry.hpp"
#include "resources/connected_textures/fence_texture.hpp"
#include "resources/connected_textures/blob_texture.hpp"
#include "resources/vertex_quad.hpp"
#include "engine/constants.hpp"
#include "rectpack2D-master/src/finders_interface.h"
#include <nlohmann/json.hpp>

#include <iostream>
#include <filesystem>
#include <fstream>

namespace rpg {
namespace resources {

ResourceManager::ResourceManager() {
    if (!default_texture.loadFromFile(GameRegistry::getResourcesFolder() + "/missing.png")) {
        throw std::runtime_error("ResourceManager::" + std::string(__func__) + "(): Could not load default texture.");
    }
    setTexture("default", default_texture, sf::IntRect(0, 0, default_texture.getSize().x, default_texture.getSize().y));
}

void ResourceManager::loadTexture(const std::filesystem::path &path, const std::string &registry_name) {
    // Check if the path is empty
    if (path.empty()) {
        std::cout << "ResourceManager::" + std::string(__func__) + "(): Empty path" << std::endl;
        return;
    }
    // Check if the path points to a valid file
    if (!std::filesystem::exists(path)) {
        std::cout << "ResourceManager::" + std::string(__func__) + "(): Path does not point to a valid file: " << path << std::endl;
        return;
    }
    // Check if the registry name is valid
    if (registry_name.empty()) {
        std::cout << "ResourceManager::" + std::string(__func__) + "(): Empty registry name" << std::endl;
        return;
    }
    // Check if texture is already loaded
    auto it = textures.find(registry_name);
    // If it's not loaded, attempt to load it
    if (it == textures.end()) {
        // The texture is initially loaded as an image
        sf::Image image;
        if (image.loadFromFile(path.string())) { // loadFromFile will notifiy the user if it fails
            // Create a texture from the image
            sf::Texture texture;
            texture.loadFromImage(image);
            // Create a sprite rect for the texture atlas
            sf::IntRect texture_rect = sf::IntRect(0, 0, image.getSize().x, image.getSize().y);
            // Add the texture to the map
            setTexture(registry_name, texture, texture_rect);
            std::cout << "Loaded texture: " << path << std::endl;
            // Check if we loaded any extra files
            if (!loadExtraFiles(path, registry_name)) {
                // If we didn't load anything extra crop the texture to remove any transparent pixels
                texture_rect = getMinimumRect(image);
                texture.loadFromImage(cropImage(image, texture_rect));
                // Overwrite with the cropped texture
                setTexture(registry_name, texture, texture_rect);
            }
        }
    } else {
        std::cout << "Texture already loaded: " << path << std::endl;
    }
}

const sf::IntRect ResourceManager::getTextureRect(const std::string &registry_name) {
    return getVertexQuad(registry_name).getTextureRect();
}

const VertexQuad& ResourceManager::getVertexQuad(const std::string &registry_name) {
    // If it's not loaded, return the default vertices
    if (vertex_quads.find(registry_name) == vertex_quads.end()) {
        std::cout << "Vertex quad not loaded: " << registry_name << ". Returning default vertex quads." << std::endl;
        return default_vertex_quad;
    }
    return vertex_quads.at(registry_name);
}

bool ResourceManager::hasAnimation(const std::string &registry_name) {
    return animations.find(registry_name) != animations.end();
}

Animation& ResourceManager::getAnimation(const std::string &registry_name) {
    // If it's not loaded, return the default animation
    if (!hasAnimation(registry_name)) {
        // std::cout << "Animation not loaded: " << registry_name ". Returning default animation." << std::endl;
        // return Animation();
        // TODO: Return default animation instead of throwing an error
        throw std::runtime_error("ResourceManager::" + std::string(__func__) + "(): Animation not loaded: " + registry_name);
    }
    return animations.at(registry_name);
}

bool ResourceManager::hasVariations(const std::string &registry_name) {
    return variations.find(registry_name) != variations.end();
}

std::shared_ptr<WeightedTexture> ResourceManager::getVariations(const std::string &registry_name) {
    // If it's not loaded, return the default variations
    if (!hasVariations(registry_name)) {
        // std::cout << "Variations not loaded: " << registry_name ". Returning default variations." << std::endl;
        // return WeightedTexture();
        throw std::runtime_error("ResourceManager::" + std::string(__func__) + "(): Variations not loaded: " + registry_name);
    }
    return variations.at(registry_name);
}

bool ResourceManager::hasConnectedTexture(const std::string &registry_name) {
    return connected_textures.find(registry_name + "_connected") != connected_textures.end();
}

std::shared_ptr<connected_textures::ConnectedTexture> ResourceManager::getConnectedTexture(const std::string &registry_name) {
    // If it's not loaded, return the default connected texture
    if (!hasConnectedTexture(registry_name)) {
        // std::cout << "Connected texture not loaded: " << registry_name ". Returning default connected texture." << std::endl;
        // return ConnectedTexture();
        throw std::runtime_error("ResourceManager::" + std::string(__func__) + "(): Connected texture not loaded: " + registry_name);
    }
    return connected_textures.at(registry_name + "_connected");
}

void ResourceManager::buildTextureAtlas() {
    // Bunch of setup stuff
    constexpr bool allow_flip = false;
	const auto runtime_flipping_mode = rectpack2D::flipping_option::DISABLED;
    using spaces_type = rectpack2D::empty_spaces<allow_flip, rectpack2D::default_empty_spaces>;
    using rect_type = rectpack2D::output_rect_t<spaces_type>;
    auto report_successful = [](rect_type&) {
		return rectpack2D::callback_result::CONTINUE_PACKING;
	};
	auto report_unsuccessful = [](rect_type&) {
		return rectpack2D::callback_result::ABORT_PACKING;
	};
    const unsigned int max_side = 1024;
    texture_atlas.create(max_side, max_side);
    const int discard_step = -4;
    // Keep track of what rectangles end up where
    std::vector<rect_type> rectangles;
    std::vector<std::string> keys;
    // TODO: Sort them based on size?
    for (const auto &vertex_quad : vertex_quads) {
        std::string key = vertex_quad.first;
        sf::IntRect texture_rect = vertex_quad.second.getTextureRect();
        rectangles.push_back(rect_type(texture_rect.left, texture_rect.top, texture_rect.width, texture_rect.height));
        keys.push_back(key);
    }
    // Pack the rectangles into the texture atlas
    const auto result_size = rectpack2D::find_best_packing<spaces_type>(
        rectangles,
        make_finder_input(
            max_side,
            discard_step,
            report_successful,
            report_unsuccessful,
            runtime_flipping_mode
        )
    );
    // Move the sprite rects to their new positions
    std::cout << "Texture atlas: " << result_size.w << " " << result_size.h << std::endl;
    for (unsigned int i = 0; i < rectangles.size(); i++) {
        auto r = rectangles[i];
        std::cout << keys[i] << ": " << r.x << " " << r.y << " " << r.w << " " << r.h << std::endl;
        // Add the sprite to the texture atlas
        texture_atlas.update(textures[keys[i]], r.x, r.y);
        // Move the sprite rect to the new position
        vertex_quads[keys[i]].setTextureRect(sf::IntRect(r.x, r.y, r.w, r.h));
        // Move animations, variations, and connected textures to the new position
        if (hasAnimation(keys[i])) {
            animations.at(keys[i]).moveTo(sf::Vector2f(r.x, r.y));
        }
        if (hasVariations(keys[i])) {
            variations.at(keys[i])->moveTo(sf::Vector2f(r.x, r.y));
        }
        /**
        It is deliberate that we check for the connected texture manually here
        instead of using hasConnectedTexture(). This is because the connected
        texture is stored at registry_name + "_connected", which for ease of use
        gets appended automatically in hasConnectedTexture().
        */
        if (connected_textures.find(keys[i]) != connected_textures.end()) {
            connected_textures.at(keys[i])->moveTo(sf::Vector2f(r.x, r.y));
        }
    }
    texture_atlas.copyToImage().saveToFile("D:/Random-Code/rpg/resources/atlas.png");
}

void ResourceManager::setTexture(const std::string &registry_name, const sf::Texture &texture, const sf::IntRect &texture_rect) {
    textures[registry_name] = texture;
    vertex_quads[registry_name].setTextureRect(texture_rect);
    // TODO: Figure out if this is still necessary?
    // Scaling of the sprite is different for sprites used in the UI
    if (registry_name.find("ui") != std::string::npos) {
        vertex_quads[registry_name].setScale(engine::constants::UI_SPRITE_SCALE);
        // vertex_quads[registry_name].setScale(1.0f);
    } else {
        vertex_quads[registry_name].setScale(engine::constants::WORLD_SPRITE_SCALE);
        // vertex_quads[registry_name].setScale(1.0f);
    }
}

const sf::Texture& ResourceManager::getTexture(const std::string &registry_name) {
    // Check if texture is already loaded
    auto it = textures.find(registry_name);
    // If it's not loaded, return the default texture
    if (it == textures.end()) {
        std::cout << "Texture not loaded: " << registry_name << ". Returning default texture." << std::endl;
        return textures["default"];
    }
    return textures[registry_name];
}

bool ResourceManager::loadExtraFiles(const std::filesystem::path &path, const std::string &registry_name) {
    std::filesystem::path path_copy = path;
    std::string dir_and_stem = path_copy.replace_extension("").string();
    // Possible files to load
    std::string animations_filename = dir_and_stem + ANIMATION_EXTENSION;
    std::string variations_filename = dir_and_stem + VARIATIONS_EXTENSION;
    std::vector<std::string> connected_texture_filenames = {
        dir_and_stem + CONNECTED_TEXTURE_FENCE_EXTENSION,
        dir_and_stem + CONNECTED_TEXTURE_BLOB_EXTENSION
    };
    // Check if any of the files exist
    bool foundExtraFile = false;
    if (std::filesystem::exists(animations_filename)) {
        std::cout << "Found animation file: " << animations_filename << std::endl;
        loadAnimation(animations_filename, registry_name);
        foundExtraFile = true;
    }
    if (std::filesystem::exists(variations_filename)) {
        std::cout << "Found variation file: " << variations_filename << std::endl;
        loadVariations(variations_filename, registry_name);
        foundExtraFile = true;
    }
    for (const auto& filename : connected_texture_filenames) {
        if (std::filesystem::exists(filename)) {
            std::cout << "Found connected texture file: " << filename << std::endl;
            loadConnectedTexture(filename, registry_name);
            foundExtraFile = true;
            break;
        }
    }
    return foundExtraFile;
}

const sf::IntRect ResourceManager::getMinimumRect(const sf::Image &image) {
    int minX = image.getSize().x, minY = image.getSize().y, maxX = 0, maxY = 0;
    for (unsigned y = 0; y < image.getSize().y; ++y) {
        for (unsigned x = 0; x < image.getSize().x; ++x) {
            sf::Color pixel = image.getPixel(x, y);
            if (pixel.a > 0) {  // Adjust this threshold as needed
                minX = std::min(minX, static_cast<int>(x));
                minY = std::min(minY, static_cast<int>(y));
                maxX = std::max(maxX, static_cast<int>(x));
                maxY = std::max(maxY, static_cast<int>(y));
            }
        }
    }
    // There seems to be some kind of off-by-one error here
    sf::IntRect bounds(minX, minY, maxX - minX + 1, maxY - minY + 1);
    return bounds;
}

const sf::Image ResourceManager::cropImage(const sf::Image &image, const sf::IntRect &rect) {
    sf::Image cropped;
    cropped.create(rect.width, rect.height);
    for (unsigned y = 0; y < rect.height; ++y) {
        for (unsigned x = 0; x < rect.width; ++x) {
            cropped.setPixel(x, y, image.getPixel(x + rect.left, y + rect.top));
        }
    }
    return cropped;
}

std::vector<sf::Image> ResourceManager::splitTexture(const sf::Texture& source_texture, int frame_width, int frame_height) {
    std::vector<sf::Image> frame_images;
    // Calculate the number of frames
    int frame_count = source_texture.getSize().x / frame_width;
    // Convert the source texture to image for processing
    sf::Image source_image = source_texture.copyToImage();
    for (int i = 0; i < frame_count; ++i) {
        sf::Image frame_image;
        frame_image.create(frame_width, frame_height);
        for (int y = 0; y < frame_height; ++y) {
            for (int x = 0; x < frame_width; ++x) {
                sf::Color pixel = source_image.getPixel(i * frame_width + x, y);
                frame_image.setPixel(x, y, pixel);
            }
        }
        frame_images.push_back(frame_image);
    }
    return frame_images;
}

std::vector<sf::IntRect> ResourceManager::getRects(const sf::Texture &texture) {
    // TODO: Consider something more flexible in the future
    // For now, assume that the texture is a horizontal strip of frames
    std::vector<sf::IntRect> rects;
    int width = texture.getSize().x;
    int height = texture.getSize().y;
    int frame_count = width / height;
    for (int i = 0; i < frame_count; i++) {
        rects.push_back(sf::IntRect(i * height, 0, height, height));
    }
    return rects;
}

void ResourceManager::loadAnimation(const std::filesystem::path &path, const std::string &registry_name) {
    // Check if animation is already loaded
    if (!hasAnimation(registry_name)) {
        // Get the texture for the animation
        const sf::Texture &texture = getTexture(registry_name);
        // // Split the texture into frames
        // std::vector<sf::Image> frames = splitTexture(*texture, texture->getSize().y, texture->getSize().y);
        // // Store each frame in the sprite textures map
        // for (int i = 0; i < frames.size(); i++) {
        //     std::string frame_name = registry_name + "_" + std::to_string(i);
        //     // Crop the frame to remove excess transparent pixels
        //     sf::IntRect frame_rect = getMinimumRect(frames[i]);
        //     frames[i] = cropImage(frames[i], frame_rect);
        //     // Create a texture from the frame
        //     sf::Texture frame_texture;
        //     frame_texture.loadFromImage(frames[i]);
        //     textures[frame_name] = &frame_texture;
        //     sprite_rects[frame_name] = frame_rect;
        // }
        std::vector<sf::IntRect> frame_rects = getRects(texture);
        // Load the frame rate from the animation file
        std::ifstream file(path);
        nlohmann::json json = nlohmann::json::parse(file);
        int frame_rate = json["frame_rate"];
        std::cout << "Loaded animation: " << registry_name << " (" << frame_rects.size() << " frames, " << frame_rate << " fps)" << std::endl;
        animations.insert({registry_name, Animation(frame_rects, frame_rate)});
    } else {
        std::cout << "Animation already loaded: " << registry_name << std::endl;
    }
}

void ResourceManager::loadVariations(const std::filesystem::path &path, const std::string &registry_name) {
    // Check if variations are already loaded
    if (!hasVariations(registry_name)) {
        // Get the texture for the variations
        const sf::Texture &texture = getTexture(registry_name);
        std::vector<sf::IntRect> variation_rects = getRects(texture);
        // Load the weightings from the variations file
        std::ifstream file(path);
        nlohmann::json json = nlohmann::json::parse(file);
        std::vector<int> weights;
        for (auto& variation : json["variations"]) {
            weights.push_back(variation["weight"]);
        }
        std::cout << "Loaded variations: " << registry_name << " (" << variation_rects.size() << " variations)" << std::endl;
        variations.insert({registry_name, std::make_shared<WeightedTexture>(variation_rects, weights)});
    } else {
        std::cout << "Variations already loaded: " << registry_name << std::endl;
    }
}

void ResourceManager::loadConnectedTexture(const std::filesystem::path &path, const std::string &registry_name) {
    // Check if connected texture is already loaded
    if (!hasConnectedTexture(registry_name)) {
        // Get the texture for the connected texture
        sf::Texture texture;
        // Load the texture from the file (we already know it exists)
        texture.loadFromFile(path.string());
        /**
         * Note that the TEXTURE is stored at registry_name + "_connected", but the
         * CONNECTED TEXTURE is stored at registry_name. This makes it a lot easier
         * to check if e.g. a tile has a connected texture, and also easier to retrieve it
         * since we don't have to getConnectedTexture(registry_name + "_connected"), but
         * can just use getConnectedTexture(registry_name).
        */
        setTexture(registry_name + "_connected", texture, sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));
        std::shared_ptr<connected_textures::ConnectedTexture> connected_texture;
        if (path.string().find(CONNECTED_TEXTURE_FENCE_EXTENSION) != std::string::npos) {
            connected_texture = std::make_shared<connected_textures::FenceTexture>(texture.getSize());
        } else if (path.string().find(CONNECTED_TEXTURE_BLOB_EXTENSION) != std::string::npos) {
            connected_texture = std::make_shared<connected_textures::BlobTexture>(texture.getSize());
        }
        connected_textures.insert({registry_name + "_connected", connected_texture});
        std::cout << "Loaded connected texture: " << registry_name + "_connected" << std::endl;
    } else {
        std::cout << "Connected texture already loaded: " << registry_name << std::endl;
    }
}

} // namespace resources
} // namespace rpg