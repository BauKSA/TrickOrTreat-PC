#include "SFMLGraphics.h"
#include "Log.h"
#include "Actor.h"
#include "sprite_map.h"


/*
* RESOURCE MANAGER
*/
void SFMLResourceManager::load_texture(Resources::TextureID id, uint8_t width, uint8_t height) {
	sf::Image img;
	img.create(width, height, sf::Color::Black);

	const unsigned char* texture = get_texture_map(id);

	if (texture == nullptr) {
		Log::log(LogType::ERROR, "No texture map found for ID " + std::to_string(static_cast<int>(id)));
		return;
	}

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int bit_index = y * width + x;
			int byte_index = bit_index / 8;
			int bit_offset = 7 - (bit_index % 8);
			bool set = (texture[byte_index] >> bit_offset) & 1;
			if (set)
				img.setPixel(x, y, sf::Color::White);
		}
	}

	std::shared_ptr<sf::Texture> txt = std::make_shared<sf::Texture>();
	txt->loadFromImage(img);

	textures[id] = txt;
}

void SFMLResourceManager::create_sprite(Resources::TextureID texture, const uint8_t& name, uint8_t offset_x, uint8_t offset_y) {
	std::unordered_map<Resources::TextureID, std::shared_ptr<sf::Texture>>::iterator it = textures.find(texture);
	if (it == textures.end()) {
		Log::log(LogType::ERROR, "No texture with ID " + std::to_string(static_cast<int>(texture)));
		return;
	}

	SFMLSprite spr;
	spr.frame = std::make_shared<sf::Sprite>();
	spr.frame->setTexture(*it->second);
	spr.texture = texture;
	spr.width = it->second->getSize().x;
	spr.height = it->second->getSize().y;
	spr.offset = { static_cast<float>(offset_x), static_cast<float>(offset_y) };

	//Log::log(LogType::INFO, "sprite created: " + name);

	sprites[name] = spr;
}

std::optional<std::reference_wrapper<SFMLSprite>> SFMLResourceManager::get_sprite(uint8_t name) {
	std::unordered_map<uint8_t, SFMLSprite>::iterator it = sprites.find(name);
	if (it == sprites.end()) {
		Log::log(LogType::ERROR, "No base sprite with name " + name);
		return std::nullopt;
	}

	SFMLSprite& spr = it->second;
	return spr;
}

std::optional<std::reference_wrapper<SFMLSprite>> SFMLResourceManager::get_custom_sprite(CustomSpriteName name) {
	std::unordered_map<CustomSpriteName, SFMLSprite>::iterator it = custom_sprites.find(name);
	if (it == custom_sprites.end()) {
		Log::log(LogType::ERROR, "No custom sprite with name " + std::to_string(name.a) + ":" + std::to_string(name.b) + ":" + std::to_string(name.c));
		return std::nullopt;
	}

	SFMLSprite& spr = it->second;
	return spr;
}

std::optional<std::reference_wrapper<sf::Texture>> SFMLResourceManager::get_texture(Resources::TextureID id) {
	std::unordered_map<Resources::TextureID, std::shared_ptr<sf::Texture>>::iterator it = textures.find(id);
	if (it == textures.end()) {
		Log::log(LogType::ERROR, "No texture with id " + std::to_string(static_cast<int>(id)));
		return std::nullopt;
	}

	sf::Texture& txt = *it->second;
	return txt;
}

std::optional<std::reference_wrapper<sf::Texture>> SFMLResourceManager::get_texture(CustomTextureID id) {
	std::unordered_map<CustomTextureID, std::shared_ptr<sf::Texture>>::iterator it = custom_textures.find(id);
	if (it == custom_textures.end()) {
		Log::log(LogType::ERROR, "No custom texture with id " + std::to_string(static_cast<int>(id)));
		return std::nullopt;
	}

	sf::Texture& txt = *it->second;
	return txt;
}

CustomTextureID SFMLResourceManager::create_texture(uint8_t width, uint8_t height) {
	CustomTextureID id = custom_texture_id++;
	custom_textures[id] = std::make_shared<sf::Texture>();
	custom_textures[id]->create(width, height);

	return id;
}

CustomTextureID SFMLResourceManager::create_texture(CustomTextureID id, uint8_t width, uint8_t height) {
	custom_textures[id]->create(width, height);

	return id;
}

TextureSize SFMLResourceManager::get_texture_size(Resources::TextureID id) {
	std::unordered_map<Resources::TextureID, std::shared_ptr<sf::Texture>>::iterator
		it = textures.find(id);

	if (it == textures.end()) {
		Log::log(LogType::ERROR, "No texture found with id " + std::to_string(static_cast<int>(id)));
		TextureSize error{};
		error.width = -1;
		error.height = -1;

		return error;
	}

	TextureSize size{};
	size.width = it->second->getSize().x;
	size.height = it->second->getSize().y;

	return size;
}

SpriteOffset SFMLResourceManager::get_sprite_offset(uint8_t name) {
	std::optional<std::reference_wrapper<SFMLSprite>> spr_opt = get_sprite(name);
	if (!spr_opt.has_value()) {
		Log::log(LogType::ERROR, "No base sprite found with name " + name);
		SpriteOffset error{};
		error.x = -1;
		error.y = -1;

		return error;
	}

	SFMLSprite spr = spr_opt.value().get();

	SpriteOffset offset{};
	offset.x = spr.offset.x;
	offset.y = spr.offset.y;

	return offset;
}

void SFMLResourceManager::create_animation(AnimationName name, AnimationFrames frames) {
	for (uint8_t& frame : frames) {
		std::optional<std::reference_wrapper<SFMLSprite>> spr_opt = get_sprite(frame);
		if (!spr_opt.has_value()) {
			Log::log(LogType::WARNING, "cannot create animation - sprite " + std::to_string(frame) + "does not exist");
			return;
		}
	}

	animations[name] = frames;
}

uint8_t SFMLResourceManager::get_animation_frame(uint8_t animation, uint8_t frame) {
	std::unordered_map<AnimationName, AnimationFrames>::iterator
		it = animations.find(animation);

	if (it == animations.end()) {
		Log::log(LogType::ERROR, "no animation found with name: " + animation);
		return 255;
	}

	if (frame >= it->second.size()) {
		Log::log(LogType::ERROR, "not animation frame in index " + std::to_string(frame));
		return 255;
	}

	return it->second.at(frame);
}

uint8_t SFMLResourceManager::next_animation_frame(AnimationName animation, uint8_t frame) {
	std::unordered_map<AnimationName, AnimationFrames>::iterator
		it = animations.find(animation);

	if (it == animations.end()) {
		Log::log(LogType::ERROR, "no animation found with name: " + animation);
		return 0;
	}

	const AnimationFrames& frames = it->second;
	const uint8_t size = frames.size();

	frame++;
	if (frame >= size) return 0;
	return frame;
}

void SFMLResourceManager::create_tileset(Tileset tileset, uint8_t name) {
	tilesets.push_back(tileset);

	std::optional<std::reference_wrapper<sf::Texture>> texture_opt = get_texture(tileset.texture);
	if (!texture_opt.has_value()) {
		Log::log(LogType::ERROR, "No texture with ID: " + std::to_string(static_cast<int>(tileset.texture)));
		return;
	}

	sf::Texture& texture = texture_opt.value().get();

	Log::log(LogType::INFO, "Creating tileset with texture ID: " + std::to_string(static_cast<int>(tileset.texture)));
	Log::log(LogType::INFO, "Tileset details - tile width: " + std::to_string(tileset.tile_width) +
		", tile height: " + std::to_string(tileset.tile_height) +
		", texture width: " + std::to_string(texture.getSize().x) +
		", texture height: " + std::to_string(texture.getSize().y) +
		", columns: " + std::to_string(tileset.columns) +
		", rows: " + std::to_string(tileset.columns));

	const uint8_t columns = texture.getSize().x / tileset.tile_width;
	const uint8_t rows = texture.getSize().y / tileset.tile_height;
	const uint8_t tiles = rows * columns;

	uint8_t row = 0;
	uint8_t column = 0;

	debug_monitor.clear(sf::Color::Black);
	for(size_t i = 0; i < tiles; i++) {
		uint8_t src_x = column * tileset.tile_width;
		uint8_t src_y = row * tileset.tile_height;

		SFMLSprite spr{};
		spr.frame = std::make_shared<sf::Sprite>();
		spr.frame->setTexture(texture);
		spr.frame->setTextureRect(sf::IntRect(src_x, src_y, tileset.tile_width, tileset.tile_height));
		spr.texture = tileset.texture;


		CustomSpriteName tile_name = { name, column, row };
		put_sprite(tile_name, spr);
		Log::log(LogType::INFO, "creating tile sprite with name: " + std::to_string(tile_name.a) + ":" + std::to_string(tile_name.b) + ":" + std::to_string(tile_name.c));
		spr.frame->setPosition(src_x, src_y);
		debug_monitor.draw(*spr.frame);
		spr.frame->setPosition(0, 0);

		column++;
		if (column >= columns) {
			column = 0;
			row++;
		}
	}

	debug_monitor.display();
}

void SFMLResourceManager::draw_tileset(Resources::TextureID id, TileCoords coords) {
}

std::optional<std::reference_wrapper<Tileset>> SFMLResourceManager::get_tileset(Resources::TextureID id) {
	std::vector<Tileset>::iterator it = std::find_if(tilesets.begin(), tilesets.end(),
		[id](const Tileset& t) { return t.texture == id; });

	if (it == tilesets.end()) {
		Log::log(LogType::ERROR, "No tileset found with texture id " + std::to_string(static_cast<int>(id)));
		return std::nullopt;
	}

	return *it;
}