#include "TextManager.h"

#include<SFML/System.hpp>

TileCoords TextManager::get_coords(char c) {
	switch (c) {
	default:
		return { 255, 255 };
	case 'A':
		return { 0, 0 };
	case 'B':
		return { 1, 0 };
	case 'C':
		return { 2, 0 };
	case 'D':
		return { 3, 0 };
	case 'E':
		return { 4, 0 };
	case 'F':
		return { 5, 0 };
	case 'G':
		return { 6, 0 };
	case 'H':
		return { 7, 0 };
	case 'I':
		return { 8, 0 };
	case 'J':
		return { 9, 0 };
	case 'K':
		return { 10, 0 };
	case 'L':
		return { 11, 0 };
	case 'M':
		return { 12, 0 };
	case 'N':
		return { 13, 0 };
	case 'O':
		return { 14, 0 };
	case 'P':
		return { 15, 0 };
	case 'Q':
		return { 0, 1 };
	case 'R':
		return { 1, 1 };
	case 'S':
		return { 2, 1 };
	case 'T':
		return { 3, 1 };
	case 'U':
		return { 4, 1 };
	case 'V':
		return { 5, 1 };
	case 'W':
		return { 6, 1 };
	case 'X':
		return { 7, 1 };
	case 'Y':
		return { 8, 1 };
	case 'Z':
		return { 9, 1 };

	}
}

void TextManager::start_text(const char* t) {
	writing = true;
	text = t;
	current_index = 0;
	x = TEXT_X_INIT;
	y = TEXT_Y_INIT;
	line_count = 0;
	last_update = 0;
	start_index = 0;
}

void TextManager::update(float delta_time) {
	if (!text) return;
	if (!writing) return;

	last_update += delta_time;
	if (last_update >= letter_delay_ms) {
		last_update = 0;
		current_index++;
		Log::log(LogType::INFO, "Writing letter");
	}

	graphics->draw_sprite(background, 0, 36);

	if (text[current_index] == '\0') {
		stop_text();
	}

	x = TEXT_X_INIT;
	y = TEXT_Y_INIT;
	line_count = 0;

	for (size_t i = start_index; i < current_index; i++) {
		char c = text[i];

		TileCoords coords = get_coords(c);
		if (coords.x == 255 && coords.y == 255) {
			x += font_size;
			if (x >= TEXT_X_INIT + 80) {
				x = TEXT_X_INIT;
				y += font_size + 2;
				line_count++;

				if (line_count >= 2) {
					start_index = current_index;
					break;
				}
			}

			continue;
		}

		CustomSpriteName spr{};
		spr.a = font;
		spr.b = coords.x;
		spr.c = coords.y;

		graphics->draw_custom_sprite(spr, x, y);
		x += font_size;

		if (x >= TEXT_X_INIT + 80) {
			x = TEXT_X_INIT;
			y += font_size + 2;
			line_count++;

			if (line_count >= 2) {
				start_index = current_index;
				break;
			}
		}
	}
}

void TextManager::stop_text() {
	Log::log(LogType::INFO, "Text stopped");
	writing = false;
}

bool TextManager::is_done() const { return !writing; }