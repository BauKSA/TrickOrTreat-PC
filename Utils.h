#pragma once
#include <cstdint>
#include <vector>
#include <functional>

#include "ResourceIDs.h"

constexpr uint8_t NONE_SPRITE = 255;


constexpr unsigned int SCREEN_WIDTH = 128;
constexpr unsigned int SCREEN_HEIGHT = 64;
constexpr unsigned int TEXT_X_INIT = 9;
constexpr unsigned int TEXT_Y_INIT = SCREEN_HEIGHT - 26;

constexpr uint8_t TILE_SIZE = 4;

constexpr uint8_t MAP_WIDTH = 124;
constexpr uint8_t MAP_HEIGHT = 52;
constexpr uint8_t VERTICAL_TILES = MAP_HEIGHT / TILE_SIZE;
constexpr uint8_t HORIZONTAL_TILES = MAP_WIDTH / TILE_SIZE;

constexpr uint8_t STAGES = 1;

constexpr unsigned int REGULAR_TILE_OFFSET_X = 2;
constexpr unsigned int REGULAR_TILE_OFFSET_Y = 8;

constexpr float FRAME_RATE = 1.f / 15.f;

enum class Directions {
	NONE,
	UP,
	DOWN,
	RIGHT,
	LEFT
};

struct TextureSize {
	unsigned int width;
	unsigned int height;
};

struct SpriteOffset {
	uint8_t x;
	uint8_t y;
};

struct COORDS {
	uint8_t x;
	uint8_t y;
};

using CustomTextureID = uint8_t;
struct CustomSpriteName {
	uint8_t a;
	uint8_t b;
	uint8_t c;

	bool operator==(const CustomSpriteName& other) const {
		return a == other.a && b == other.b && c == other.c;
	}
};

namespace std {
	template<>
	struct hash<CustomSpriteName> {
		std::size_t operator()(const CustomSpriteName& k) const noexcept {
			// Combinamos los tres uint8_t en un size_t
			return (static_cast<size_t>(k.a) << 16) |
				(static_cast<size_t>(k.b) << 8) |
				static_cast<size_t>(k.c);
		}
	};
}

using AnimationName = uint8_t;
using AnimationFrames = std::vector<uint8_t>;
constexpr CustomSpriteName EMPTY_TILE = { 255, 255, 255 };

//EVENTS
struct MousePosition {
	int x;
	int y;
};

namespace Key {
	enum Mouse {
		LEFT_CLICK,
		RIGHT_CLICK
	};

	enum Keyboard {
		NONE,
		UP,
		DOWN,
		RIGHT,
		LEFT,
	};
};

enum class Type {
	KeyPressed,
	KeyReleased,
	MouseButtonPressed,
	MouseButtonReleased,
	WindowClosed
};

struct Event {
	Type type;
	Key::Keyboard key;
	Key::Mouse mouse;
	MousePosition mouse_position;
};

constexpr uint8_t MAX_EVENTS = 4;

struct Tileset {
	Resources::TextureID texture;
	uint8_t tile_width;
	uint8_t tile_height;
	uint8_t spacing;
	uint8_t margin;
	uint8_t columns;
	uint8_t rows;
};

struct TileCoords {
	uint8_t x;
	uint8_t y;
};