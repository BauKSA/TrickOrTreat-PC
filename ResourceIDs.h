#pragma once
#ifndef RESOURCES
#define RESOURCES

namespace Resources {
    enum class TextureID : uint8_t {
        NONE,
        PLAYER_MAP_WALK,
        PLAYER_MAP,
        TILESET,
        FONT,
        FONT_BACKGROUND
    };

    enum class Sprites : uint8_t {
        PLAYER_MAP,
        PLAYER_MAP_WALK,
        FONT
    };
}

#endif // !RESOURCES