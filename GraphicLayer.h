#pragma once
#include<string>
#include<memory>
#include<vector>
#include<optional>

#include "ResourceIDs.h"
#include "Utils.h"

class ResourceManager {
public:
    virtual void load_texture(Resources::TextureID id, uint8_t width, uint8_t height) = 0;
    virtual CustomTextureID create_texture(uint8_t width, uint8_t height) = 0;
    virtual CustomTextureID create_texture(CustomTextureID id, uint8_t width, uint8_t height) = 0;
    virtual void create_sprite(Resources::TextureID texture, const uint8_t& name, uint8_t offset_x, uint8_t offset_y) = 0;
    virtual TextureSize get_texture_size(Resources::TextureID id) = 0;
    virtual void create_animation(AnimationName name, AnimationFrames paths) = 0;
    virtual uint8_t next_animation_frame(AnimationName name, uint8_t current_frame) = 0;
    virtual void create_tileset(Tileset tileset, uint8_t name) = 0;

    virtual SpriteOffset get_sprite_offset(uint8_t name) = 0;
    virtual uint8_t get_animation_frame(uint8_t animation, uint8_t frame) = 0;
    virtual void draw_tileset(Resources::TextureID texture, TileCoords coords) = 0;
    virtual std::optional<std::reference_wrapper<Tileset>> get_tileset(Resources::TextureID id) = 0;
};

class GraphicLayer {
public:
    virtual ~GraphicLayer() {}
    virtual void draw_sprite(const uint8_t& name, uint8_t x, uint8_t y) = 0;
    virtual void draw_custom_sprite(CustomSpriteName name, uint8_t x, uint8_t y) = 0;
    virtual void draw_texture(CustomSpriteName name, CustomTextureID id, std::vector<uint8_t> sprites) = 0;
    virtual void draw_texture(CustomSpriteName name, CustomTextureID id, std::vector<CustomSpriteName> sprites) = 0;
    virtual void draw_texture_tileset(Resources::TextureID tileset_id, CustomSpriteName name, CustomTextureID id, CustomSpriteName map[VERTICAL_TILES][HORIZONTAL_TILES]) = 0;
    virtual void clear() = 0;
    virtual void display() = 0;
    virtual bool is_open() const = 0;
    virtual size_t poll_events(Event(&events)[MAX_EVENTS]) = 0;
    virtual void create_animation(AnimationName name, AnimationFrames paths) = 0;

    virtual void load_texture(Resources::TextureID id, uint8_t width, uint8_t height) = 0;
    virtual void create_sprite(Resources::TextureID texture, const uint8_t& name, uint8_t offset_x, uint8_t offset_y) = 0;
    virtual TextureSize get_texture_size(Resources::TextureID id) = 0;
    virtual CustomTextureID create_texture(uint8_t width, uint8_t height) = 0;
    virtual CustomTextureID create_texture(CustomTextureID id, uint8_t width, uint8_t height) = 0;
    virtual SpriteOffset get_sprite_offset(uint8_t name) = 0;
    virtual uint8_t next_animation_frame(AnimationName name, uint8_t current_frame) = 0;
    virtual uint8_t get_animation_frame(uint8_t animation, uint8_t frame) = 0;
	virtual void create_tileset(uint8_t name, Resources::TextureID id, uint8_t width, uint8_t height, uint8_t tile_width, uint8_t tile_height, uint8_t margin = 0, uint8_t spacing = 0) = 0;
    virtual void draw_tileset(Resources::TextureID texture, TileCoords coords) = 0;

    virtual void close() = 0;
};