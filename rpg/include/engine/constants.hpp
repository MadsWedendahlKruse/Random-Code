#pragma once

namespace rpg {
namespace engine {
/**
 * @namespace constants
 * @brief Constants used throughout the engine.
 * 
 * This might change to a config class in the future.
*/
namespace constants {
/**
 * @brief The aspect ratio of the game window.
 * 
 * This is used to calculate the height of the window from the width.
 * The default value is 16:9. Constant for the moment, but might be
 * configurable in the future.
*/
constexpr float ASPECT_RATIO = 16.0f / 9.0f;
/**
 * @brief The size of a cell in the world grid (in pixels).
 * 
 * This ensures that the pixels are scaled correctly when drawing the object.
 * This also dicatates the relation between the size of the texture and the
 * size of the object. For example, if the texture is 16x16 pixels, then the
 * object will be 1x1 meters (fill up one cell in the world grid).
 * Artifacts appear when using 16.0f, so 15.999f is used instead. This gets rid
 * of the articating and looks the same as 16.0f. Kind of a hacky solution.
*/
constexpr float WORLD_GRID_CELL_SIZE = 15.999f;
// constexpr float WORLD_GRID_CELL_SIZE = 16.f;
/**
 * @brief The width of the world grid (in meters/cells).
*/
// constexpr float WORLD_GRID_WIDTH = 128.0f;
constexpr float WORLD_GRID_WIDTH = 32.0f;
/**
 * @brief The height of the world grid (in meters/cells).
*/
constexpr float WORLD_GRID_HEIGHT = WORLD_GRID_WIDTH / ASPECT_RATIO;
/**
 * @brief The scale of the world sprites.
 * 
 * This is used to scale the sprites to the correct size when drawing them.
 * This is the inverse of WORLD_GRID_CELL_SIZE.
*/
constexpr float WORLD_SPRITE_SCALE = 1.0f / WORLD_GRID_CELL_SIZE;
/**
 * @brief The size of a cell in the UI grid (in pixels).
 * 
 * This means the UI is built of 8x8 pixel cells. This is used to scale the
 * UI sprites to the correct size when drawing them.
*/
constexpr float UI_GRID_CELL_SIZE = 8.0f;
/**
 * @brief The width of the UI grid (in cells).
 * 
 * The texture pixels should be the same size when drawing the world as
 * well as when drawing the UI, so the UI grid is scaled to match the
 * world grid, assuming the world grid is 32 cells wide.
*/
constexpr float UI_GRID_WIDTH = WORLD_GRID_CELL_SIZE / UI_GRID_CELL_SIZE * 32.0f;
/**
 * @brief The height of the UI grid (in cells).
*/
constexpr float UI_GRID_HEIGHT = UI_GRID_WIDTH / ASPECT_RATIO;
/**
 * @brief The scale of the UI sprites.
 * 
 * This is used to scale the sprites to the correct size when drawing them.
 * This is the inverse of UI_GRID_CELL_SIZE.
*/
constexpr float UI_SPRITE_SCALE = 1.0f / UI_GRID_CELL_SIZE;

} // namespace constants
} // namespace engine
} // namespace rpg