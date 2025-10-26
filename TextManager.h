#pragma once

#include "Utils.h"
#include "Graphics.h"
#include "Log.h"

struct Text {
	char first_line[26];
	char second_line[26];
};

class TextManager {
private:
	uint8_t font;
	uint8_t background;
	uint8_t font_size = 5;

	bool writing = false;
	bool waiting_point = true;

	bool set_new_line = false;

	const Text** text;
	uint8_t text_size = 0;
	uint8_t index = 0;
	uint8_t current_index = 0;
	uint8_t second_line_index = 0;

	uint8_t x = 0, y = 0;
	const int letter_delay_ms = 5;
	float last_update = 0;

	bool waiting = false;

	CustomTextureID temp_texture;

	Callback after_text_callback;
	void* ctx;

	TileCoords get_coords(char c);
public:
	TextManager() : font(101), background(102), text{},
		after_text_callback(nullptr),
		ctx(nullptr) {
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

	void start_text(const Text* text[], uint8_t size, Callback cb = nullptr, void* ctx = nullptr);
	void update(float delta_time);
	void stop_text();

	bool is_done() const;
	void next_text();
};