#include<SFML/System.hpp>

#include "SFMLGraphics.h"
#include "Log.h"
#include "Utils.h"
#include "Actor.h"

/*
* GRAPHIC LAYER
*/
SFMLGraphics::SFMLGraphics() {
	Log::log(LogType::INFO, "SFMLGraphics constructor called");
	resource_manager = std::make_unique<SFMLResourceManager>();

	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	window.create(desktop, "Trick or Treat");

	float window_ratio = static_cast<float>(desktop.width) / desktop.height;
	float target_ratio = static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT;

	float scale_x = 1.f;
	float scale_y = 1.f;

	if (window_ratio > target_ratio) {
		scale_x = target_ratio / window_ratio;
	}
	else {
		scale_y = window_ratio / target_ratio;
	}

	view = sf::View(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

	window.setView(view);

	window.setFramerateLimit(FRAME_RATE);
	window.setVerticalSyncEnabled(true);
	//window.setMouseCursorVisible(false);
}

void SFMLGraphics::update_view() { }

void SFMLGraphics::draw_sprite(const uint8_t& name, uint8_t x, uint8_t y) {
	std::optional<std::reference_wrapper<SFMLSprite>> spr_opt = resource_manager->get_sprite(name);
	if (!spr_opt.has_value()) return;
	SFMLSprite& spr = spr_opt.value().get();

	spr.frame->setPosition({ x + spr.offset.x, y + spr.offset.y });
	window.draw(*spr.frame);
}

void SFMLGraphics::draw_custom_sprite(CustomSpriteName name, uint8_t x, uint8_t y) {
	std::optional<std::reference_wrapper<SFMLSprite>> spr_opt = resource_manager->get_custom_sprite(name);
	if (!spr_opt.has_value()) return;
	SFMLSprite& spr = spr_opt.value().get();

	spr.frame->setPosition({ x + spr.offset.x, y + spr.offset.y });
	window.draw(*spr.frame);
}

void SFMLGraphics::clear() {
	window.clear(sf::Color::Black);
}

void SFMLGraphics::display() {
	update_view();
	window.display();
}

bool SFMLGraphics::is_open()const {
	return window.isOpen();
}

size_t SFMLGraphics::poll_events(Event(&events)[MAX_EVENTS]) {
	sf::Event event;

	size_t event_count = 0;

	while (window.pollEvent(event)) {
		if (event_count >= MAX_EVENTS) break;
		if (event.type == sf::Event::Closed) {
			Event e{};
			e.type = Type::WindowClosed;
			e.key = key_map(event.key.code);
			events[event_count++] = e;


			//DEBUG
			window.close();
		}

		if (event.type == sf::Event::KeyPressed) {
			Event e{};
			e.type = Type::KeyPressed;
			e.key = key_map(event.key.code);
			events[event_count++] = e;
		}

		if (event.type == sf::Event::KeyReleased) {
			Event e{};
			e.type = Type::KeyReleased;
			e.key = key_map(event.key.code);
			events[event_count++] = e;
		}
	}

	return event_count;
}

Key::Keyboard SFMLGraphics::key_map(sf::Keyboard::Key key) {
	switch (key) {
	case sf::Keyboard::D:
		return Key::Keyboard::RIGHT;
	case sf::Keyboard::A:
		return Key::Keyboard::LEFT;
	case sf::Keyboard::W:
		return Key::Keyboard::UP;
	case sf::Keyboard::S:
		return Key::Keyboard::DOWN;
	case sf::Keyboard::K:
		return Key::Keyboard::ACTION;
	default:
		return Key::Keyboard::NONE;
	}
}

void SFMLGraphics::draw_texture(CustomSpriteName name, CustomTextureID id, std::vector<uint8_t> sprite_names) {
	std::optional<std::reference_wrapper<SFMLSprite>> spr_opt = resource_manager->get_custom_sprite(name);
	if (spr_opt.has_value()) return;

	std::optional<std::reference_wrapper<sf::Texture>> txt_opt = resource_manager->get_texture(id);
	if (!txt_opt.has_value()) return;

	sf::Texture& base_txt = txt_opt.value().get();
	CustomTextureID texture = resource_manager->create_texture(base_txt.getSize().x, base_txt.getSize().y);

	sf::RenderTexture tmp_texture;
	tmp_texture.create(base_txt.getSize().x, base_txt.getSize().y);
	tmp_texture.clear(sf::Color::Transparent);

	for (uint8_t spr_name : sprite_names) {
		if (spr_name == 255) continue;
		std::optional<std::reference_wrapper<SFMLSprite>> tmp_spr_opt =
			resource_manager->get_sprite(spr_name);

		if (!tmp_spr_opt.has_value()) {
			Log::log(LogType::ERROR, "No sprite found with name " + std::to_string(spr_name));
			continue;
		}

		SFMLSprite original = tmp_spr_opt.value().get();
		original.frame->setPosition(0.f + original.offset.x, 0.f + original.offset.y);

		tmp_texture.draw(*original.frame);
		tmp_texture.display();
	}

	tmp_texture.display();

	std::shared_ptr<sf::Texture> final_texture = std::make_shared<sf::Texture>(tmp_texture.getTexture());
	resource_manager->put_texture(texture, final_texture);

	SFMLSprite custom_spr{};
	custom_spr.frame = std::make_shared<sf::Sprite>();
	custom_spr.frame->setTexture(*final_texture);
	custom_spr.offset = { 0, 0 };
	custom_spr.texture = Resources::TextureID::NONE;
	custom_spr.custom_texture = texture;
	custom_spr.width = final_texture->getSize().x;
	custom_spr.height = final_texture->getSize().y;

	Log::log(LogType::INFO, "custom sprite created: " + std::to_string(name.a) + ":" + std::to_string(name.b) + ":" + std::to_string(name.c));
	resource_manager->put_sprite(name, custom_spr);

	return;
}

void SFMLGraphics::draw_texture(CustomSpriteName name, CustomTextureID id, std::vector<CustomSpriteName> sprite_names) {
	std::optional<std::reference_wrapper<SFMLSprite>> spr_opt = resource_manager->get_custom_sprite(name);
	if (spr_opt.has_value()) return;

	std::optional<std::reference_wrapper<sf::Texture>> txt_opt = resource_manager->get_texture(id);
	if (!txt_opt.has_value()) return;

	sf::Texture& base_txt = txt_opt.value().get();
	CustomTextureID texture = resource_manager->create_texture(base_txt.getSize().x, base_txt.getSize().y);

	sf::RenderTexture tmp_texture;
	tmp_texture.create(base_txt.getSize().x, base_txt.getSize().y);
	tmp_texture.clear(sf::Color::Black);

	for (CustomSpriteName spr_name : sprite_names) {
		if (spr_name == EMPTY_TILE) continue;
		std::optional<std::reference_wrapper<SFMLSprite>> tmp_spr_opt = resource_manager->get_custom_sprite(spr_name);

		if (!tmp_spr_opt.has_value()) {
			Log::log(LogType::ERROR, "No custom sprite found with name " + std::to_string(name.a) + ":" + std::to_string(name.b) + ":" + std::to_string(name.c));
			continue;
		}

		SFMLSprite original = tmp_spr_opt.value().get();
		original.frame->setPosition(0.f + original.offset.x, 0.f + original.offset.y);

		tmp_texture.draw(*original.frame);
		tmp_texture.display();
	}

	tmp_texture.display();

	std::shared_ptr<sf::Texture> final_texture = std::make_shared<sf::Texture>(tmp_texture.getTexture());
	resource_manager->put_texture(texture, final_texture);

	SFMLSprite custom_spr{};
	custom_spr.frame = std::make_shared<sf::Sprite>();
	custom_spr.frame->setTexture(*final_texture);
	custom_spr.offset = { 0, 0 };
	custom_spr.texture = Resources::TextureID::NONE;
	custom_spr.custom_texture = texture;
	custom_spr.width = final_texture->getSize().x;
	custom_spr.height = final_texture->getSize().y;

	resource_manager->put_sprite(name, custom_spr);

	return;
}

void SFMLGraphics::draw_texture_tileset(Resources::TextureID tileset_id, CustomSpriteName name, CustomTextureID id, CustomSpriteName map[VERTICAL_TILES][HORIZONTAL_TILES]) {
	std::optional<std::reference_wrapper<SFMLSprite>> spr_opt = resource_manager->get_custom_sprite(name);
	if (spr_opt.has_value()) {
		Log::log(LogType::INFO, "Custom sprite with name " + std::to_string(name.a) + ":" + std::to_string(name.b) + ":" + std::to_string(name.c) + " already exists");
		return;
	}

	std::optional<std::reference_wrapper<sf::Texture>> txt_opt = resource_manager->get_texture(id);
	if (!txt_opt.has_value()) return;

	std::optional<std::reference_wrapper<Tileset>> tileset_opt = resource_manager->get_tileset(tileset_id);
	if (!tileset_opt.has_value()) return;

	Tileset tileset = tileset_opt.value().get();

	std::optional<std::reference_wrapper<sf::Texture>> tileset_texture_opt = resource_manager->get_texture(tileset.texture);
	if (!tileset_texture_opt.has_value()) return;

	sf::Texture& tileset_texture = tileset_texture_opt.value().get();


	sf::Texture& base_txt = txt_opt.value().get();
	CustomTextureID texture = resource_manager->create_texture(base_txt.getSize().x, base_txt.getSize().y);

	sf::RenderTexture tmp_texture;
	tmp_texture.create(base_txt.getSize().x, base_txt.getSize().y);
	tmp_texture.clear(sf::Color::Black);

	for (size_t column = 0; column < VERTICAL_TILES; column++) {
		for (size_t row = 0; row < HORIZONTAL_TILES; row++) {
			if (map[column][row] == EMPTY_TILE) continue;

			CustomSpriteName spr_name = map[column][row];

			std::optional<std::reference_wrapper<SFMLSprite>> tmp_spr_opt =
				resource_manager->get_custom_sprite(spr_name);

			if (!tmp_spr_opt.has_value()) {
				Log::log(LogType::ERROR, "No custom sprite found with name " + std::to_string(name.a) + ":" + std::to_string(name.b) + ":" + std::to_string(name.c));
				continue;
			}

			const unsigned int x = row * tileset.tile_width;
			const unsigned int y = column * tileset.tile_height;

			SFMLSprite original = tmp_spr_opt.value().get();
			original.frame->setPosition(x, y);

			tmp_texture.draw(*original.frame);
		}
	}

	tmp_texture.display();

	std::shared_ptr<sf::Texture> final_texture = std::make_shared<sf::Texture>(tmp_texture.getTexture());
	resource_manager->put_texture(texture, final_texture);

	SFMLSprite custom_spr{};
	custom_spr.frame = std::make_shared<sf::Sprite>();
	custom_spr.frame->setTexture(*final_texture);
	custom_spr.offset = { REGULAR_TILE_OFFSET_X, REGULAR_TILE_OFFSET_Y };
	custom_spr.texture = Resources::TextureID::NONE;
	custom_spr.custom_texture = texture;
	custom_spr.width = final_texture->getSize().x;
	custom_spr.height = final_texture->getSize().y;

	resource_manager->put_sprite(name, custom_spr);

	return;
}

void SFMLGraphics::create_tileset(uint8_t name, Resources::TextureID id, uint8_t width, uint8_t height, uint8_t tile_width, uint8_t tile_height, uint8_t margin, uint8_t spacing) {
	resource_manager->load_texture(id, width, height);
	
	Tileset tileset{};
	tileset.texture = id;
	tileset.tile_width = tile_width;
	tileset.tile_height = tile_height;
	tileset.tile_height = tile_height;
	tileset.margin = margin;
	tileset.spacing = spacing;
	tileset.columns = (resource_manager->get_texture_size(id).width - 2 * tileset.margin + tileset.spacing) / (tileset.tile_width + tileset.spacing);
	tileset.rows = (resource_manager->get_texture_size(id).height - 2 * tileset.margin + tileset.spacing) / (tileset.tile_height + tileset.spacing);

	resource_manager->create_tileset(tileset, name);
}