#pragma once
#include<chrono>

#include "Utils.h"
#include "Graphics.h"
#include "Log.h"

class TextManager {
private:
	uint8_t font;
	uint8_t background;
	uint8_t font_size = 5;

	bool writing = false;

	const char* text = nullptr;
	int current_index = 0;
	int start_index = 0;
	uint8_t x = 0, y = 0;
	int line_count = 0;
	const int letter_delay_ms = 5;
	float last_update = 0;

	CustomTextureID temp_texture;

	TileCoords get_coords(char c);
public:
	TextManager(): font(101), background(102) {
		Log::log(LogType::INFO, "creating text manager");
		graphics->create_tileset(font, Resources::TextureID::FONT, 80, 15, 5, 5);
		graphics->load_texture(Resources::TextureID::FONT_BACKGROUND, 128, 28);
		graphics->create_sprite(Resources::TextureID::FONT_BACKGROUND, background, 0, 0);
		temp_texture = graphics->create_texture(128, 28);
	}

	static TextManager& instance() {
		static TextManager mngr;
		return mngr;
	}

	void start_text(const char* t);
	void update(float delta_time);
	void stop_text();
	bool is_done() const;
};