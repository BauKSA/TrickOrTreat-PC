#pragma once
#include<SFML/Graphics.hpp>
#include<unordered_map>
#include<string>
#include<memory>
#include<optional>

#include "GraphicLayer.h"
#include "ResourceIDs.h"
#include "Utils.h"
#include "Log.h"

struct SFMLSprite {
	Resources::TextureID texture;
	CustomTextureID custom_texture;
	uint8_t width;
	uint8_t height;
	sf::Vector2f offset;
	std::shared_ptr<sf::Sprite> frame;
};

struct SFMLAnimation {
	uint8_t name;
	uint8_t total_frames;
	std::vector<SFMLSprite> sprites;
};

class SFMLResourceManager : public ResourceManager {
private:
	std::unordered_map<Resources::TextureID, std::shared_ptr<sf::Texture>> textures;
	std::unordered_map<CustomTextureID, std::shared_ptr<sf::Texture>> custom_textures;
	std::unordered_map<uint8_t, SFMLSprite> sprites;
	std::unordered_map<CustomSpriteName, SFMLSprite> custom_sprites;
	std::unordered_map<AnimationName, AnimationFrames> animations;
	std::vector<Tileset> tilesets;

	CustomTextureID custom_texture_id = 0;
	sf::RenderWindow debug_monitor;
public:
	SFMLResourceManager() {
		debug_monitor.create(sf::VideoMode(200, 200), "Debug Monitor");
	}
	void load_texture(Resources::TextureID id, uint8_t width, uint8_t height) override;
	CustomTextureID create_texture(uint8_t width, uint8_t height) override;
	CustomTextureID create_texture(CustomTextureID id, uint8_t width, uint8_t height) override;
	void create_sprite(Resources::TextureID texture, const uint8_t& name, uint8_t offset_x, uint8_t offset_y) override;
	void create_animation(AnimationName name, AnimationFrames paths) override;
	uint8_t next_animation_frame(AnimationName name, uint8_t current_frame) override;

	void put_texture(CustomTextureID id, std::shared_ptr<sf::Texture> texture) { custom_textures[id] = texture; }
	void put_sprite(CustomSpriteName name, SFMLSprite sprite)
	{ custom_sprites[name] = sprite; }
	void create_tileset(Tileset tileset, uint8_t name) override;

	//GETTERS
	std::optional<std::reference_wrapper<SFMLSprite>> get_sprite(uint8_t name);
	std::optional<std::reference_wrapper<SFMLSprite>> get_custom_sprite(CustomSpriteName name);
	std::optional<std::reference_wrapper<sf::Texture>> get_texture(Resources::TextureID id);
	std::optional<std::reference_wrapper<sf::Texture>> get_texture(CustomTextureID id);
	TextureSize get_texture_size(Resources::TextureID id) override;
	SpriteOffset get_sprite_offset(uint8_t name) override;
	uint8_t get_animation_frame(uint8_t animation, uint8_t frame) override;
	void draw_tileset(Resources::TextureID texture, TileCoords coords) override;
	std::optional<std::reference_wrapper<Tileset>> get_tileset(Resources::TextureID id) override;
};

class SFMLGraphics : public GraphicLayer {
private:
	std::unique_ptr<SFMLResourceManager> resource_manager;

	sf::RenderWindow window;
	sf::View view;

	void update_view();
	Key::Keyboard key_map(sf::Keyboard::Key key);
public:
	SFMLGraphics();

	void draw_texture(CustomSpriteName name, CustomTextureID id, std::vector<uint8_t> sprites) override;
	void draw_texture(CustomSpriteName name, CustomTextureID id, std::vector<CustomSpriteName> sprites) override;
	void draw_texture_tileset(Resources::TextureID tileset_id, CustomSpriteName name, CustomTextureID id, CustomSpriteName map[VERTICAL_TILES][HORIZONTAL_TILES]) override;
	void draw_sprite(const uint8_t& name, uint8_t x, uint8_t y) override;
    void draw_custom_sprite(CustomSpriteName name, uint8_t x, uint8_t y) override;
	void clear() override;
	void display() override;
	bool is_open() const override;
	size_t poll_events(Event(&events)[MAX_EVENTS]) override;
	void close() override { window.close(); }
	void create_animation(AnimationName name, AnimationFrames frames) override { resource_manager->create_animation(name, frames); }
	void create_tileset(uint8_t name, Resources::TextureID id, uint8_t width, uint8_t height, uint8_t tile_width, uint8_t tile_height, uint8_t margin = 0, uint8_t spacing = 0) override;

	//TODO: Habría que achicar esto un poco, duplica muchos métodos del ResourceManager
	//Resource Manager
	void load_texture(Resources::TextureID id, uint8_t width, uint8_t height)
		override {
		return resource_manager->load_texture(id, width, height);
	}
	void create_sprite(Resources::TextureID texture, const uint8_t& name, uint8_t offset_x, uint8_t offset_y)
		override {
		return resource_manager->create_sprite(texture, name, offset_x, offset_y);
	};
	TextureSize get_texture_size(Resources::TextureID id) override { return resource_manager->get_texture_size(id); };
	CustomTextureID create_texture(uint8_t width, uint8_t height)
		override {
		return resource_manager->create_texture(width, height);
	};
	CustomTextureID create_texture(CustomTextureID id, uint8_t width, uint8_t height)
		override {
		return resource_manager->create_texture(id, width, height);
	};
	SpriteOffset get_sprite_offset(uint8_t name) override { return resource_manager->get_sprite_offset(name); }
	uint8_t next_animation_frame(AnimationName name, uint8_t current_frame)
		override {
		return resource_manager->next_animation_frame(name, current_frame);
	}
	uint8_t get_animation_frame(uint8_t animation, uint8_t frame)
		override {
		return resource_manager->get_animation_frame(animation, frame);
	}
	void draw_tileset(Resources::TextureID texture, TileCoords coords) override {
		return resource_manager->draw_tileset(texture, coords);
	}
};