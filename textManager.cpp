#include "TextManager.h"

#include<SFML/System.hpp>

#include <string.h>
#include <errno.h>

static void safe_copy(char* dest, const char* src, size_t dest_size)
{
	if (!dest || !src || dest_size == 0) return; // chequeo básico

	// Copia hasta dest_size - 1 caracteres y asegura terminador nulo
	strncpy_s(dest, dest_size, src, _TRUNCATE);
}

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
	case '.':
		return { 10, 1 };
	}
}

void TextManager::start_text(const Text* t[], uint8_t size) {
	writing = true;
	text = t;
	text_size = size;
	current_index = 1;
	second_line_index = 0;
	index = 0;
}

void TextManager::update(float delta_time) {
	if (!text) return;
	if (!writing) return;

	graphics->draw_sprite(background, 0, 30);

	last_update += delta_time;
	if (last_update >= letter_delay_ms) {
		last_update -= letter_delay_ms;
		waiting_point = !waiting_point;

		if (!waiting) {
			current_index++;
			if (current_index >= 25) {
				second_line_index++;
				current_index = 26;
			}
		}
	}

	if (index >= text_size) {
		return stop_text();
	}

	x = TEXT_X_INIT;
	y = TEXT_Y_INIT;

	char first_line[26];
    safe_copy(first_line, text[index]->first_line, sizeof(first_line));

	char second_line[26];
	safe_copy(second_line, text[index]->second_line, sizeof(second_line));

	//Corremos línea 1:
	for (size_t i = 0; i < current_index; i++) {
		char c = first_line[i];

		TileCoords coords = get_coords(c);
		if (coords.x == 255 && coords.y == 255) {
			x += font_size;
			continue;
		}

		if (c == '\0') break;

		CustomSpriteName spr{};
		spr.a = font;
		spr.b = coords.x;
		spr.c = coords.y;

		graphics->draw_custom_sprite(spr, x, y);
		x += font_size;
	}

	if (current_index < 26) return;
	y += font_size + 2;
	x = TEXT_X_INIT;

	//Corremos la línea 2
	for (size_t i = 0; i < second_line_index; i++) {
		char c = second_line[i];

		TileCoords coords = get_coords(c);
		if (coords.x == 255 && coords.y == 255) {
			x += font_size;
			continue;
		}

		if (c == '\0') break;

		CustomSpriteName spr{};
		spr.a = font;
		spr.b = coords.x;
		spr.c = coords.y;

		graphics->draw_custom_sprite(spr, x, y);
		x += font_size;
	}

	if (second_line_index >= 26) waiting = true;

	if (waiting) {
		if (!waiting_point) return;
		TileCoords coords = get_coords('.');

		CustomSpriteName spr{};
		spr.a = font;
		spr.b = coords.x;
		spr.c = coords.y;

		graphics->draw_custom_sprite(spr, 107, y);
	}
}

void TextManager::stop_text() {
	writing = false;
}

bool TextManager::is_done() const { return !writing; }