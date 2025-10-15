#pragma once
#include<string>

#include<iostream>

#include "Graphics.h"
#include "Utils.h"
#include "Log.h"

class Stage {
private:
	Resources::TextureID tileset;
	CustomTextureID texture;
	CustomSpriteName background;
	uint8_t collision_map[VERTICAL_TILES][HORIZONTAL_TILES] = { 0 };

	uint8_t name;
public:
	Stage() = default;

	Stage(uint8_t name, Resources::TextureID tileset)
		: name(name), tileset(tileset) {
		graphics->create_tileset(name, tileset, 32, 32, 4, 4);
		texture = graphics->create_texture(MAP_WIDTH, MAP_HEIGHT);
		background = { name, 255, 255 };
	}

	void initialize(COORDS map[VERTICAL_TILES][HORIZONTAL_TILES]) {
		CustomSpriteName fixed[VERTICAL_TILES][HORIZONTAL_TILES] = { 0 };
		for(size_t i = 0; i < VERTICAL_TILES; i++) {
			//TODO: DEBUG
			std::string log;
			for(size_t j = 0; j < HORIZONTAL_TILES; j++)
				if (map[i][j].x == 255 && map[i][j].y == 255) {
					//TODO: DEBUG
					std::string _name = "[ ]";
					log += _name;

					fixed[i][j] = EMPTY_TILE;
					collision_map[i][j] = 0;
				}
				else {
					std::string _name = "[X]";
					log += _name;
					fixed[i][j] = { name, map[i][j].x, map[i][j].y };
					collision_map[i][j] = 1;
				}

			//TODO: DEBUG
			std::cout << log << std::endl;
		}

		graphics->draw_texture_tileset(tileset, background, texture, fixed);
	}

	void draw()const {
		graphics->draw_custom_sprite(background, 0.f, 0.f);
	}

	bool is_blocked(int8_t x, int8_t y) const {
		if (x < 0 || x >= VERTICAL_TILES || y < 0 || y >= HORIZONTAL_TILES)
			return true;

		Log::log(LogType::INFO, "Collision map at (" + std::to_string(x) + ", " + std::to_string(y) + "): " + std::to_string(collision_map[y][x]));
		//return collision_map[y][x] != 0;
		for (size_t i = 0; i < VERTICAL_TILES; i++) {
			//TODO: DEBUG
			std::string log;
			for (size_t j = 0; j < HORIZONTAL_TILES; j++) {
				std::string name;
				if (i == x && y == j) {
					if(collision_map[x][y] != 1) name = "[x]";
					else name = "[" + std::to_string(collision_map[i][j]) + "]";
				}
				else {
					name = "[" + std::to_string(collision_map[i][j]) + "]";
				}
				log += name;
			}

			//TODO: DEBUG
			std::cout << log << std::endl;
		}

		return collision_map[x][y] == 1;
	}

	Stage(Stage&&) = default;
	Stage& operator=(Stage&&) = default;
};