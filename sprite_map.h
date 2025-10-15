#pragma once
#ifndef SPRITE_MAP
#define SPRITE_MAP
#include<stdint.h>

#include "ResourceIDs.h"
#include "tileset.h"
#include "map_player.h"
#include "font_background.h"
#include "font.h"

struct TextureEntry {
    Resources::TextureID id;
    const unsigned char* data;
};

const TextureEntry texture_map[] = {
    { Resources::TextureID::TILESET, tileset },
    { Resources::TextureID::PLAYER_MAP, player_map_hex },
    {Resources::TextureID::FONT_BACKGROUND, font_background},
    {Resources::TextureID::FONT, font},
};

static const int texture_count = sizeof(texture_map) / sizeof(TextureEntry);

inline const unsigned char* get_texture_map(Resources::TextureID id) {
    for (int i = 0; i < texture_count; ++i) {
        if (texture_map[i].id == id)
            return texture_map[i].data;
    }

    return nullptr;
}

#endif // !SPRITE_MAP
