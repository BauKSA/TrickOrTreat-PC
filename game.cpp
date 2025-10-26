#include "Game.h"
#include "stage_include.h"

void Game::initialize() {
	stages[0] = new Stage1_1();
	current_stage = 0;
}

void Game::tick(uint16_t delta_time) {
	Event events[MAX_EVENTS];
	size_t event_count = graphics->poll_events(events);

	for (size_t i = 0; i < event_count; i++) {
		handle_event(events[i]);
	}

	if (state == GameState::MAP) {
		stages[current_stage]->update();
		map_player.update(delta_time);
	}
}

void Game::handle_event(Event e) {
	if (state == GameState::MAP) return map_handler.handle(map_player, e);
	else if (state == GameState::DIALOGUE) return text_handler.handle(e);
}

bool Game::tile_block(int8_t x, int8_t y) {
	Log::log(LogType::INFO, "Checking tile at pixel position (" + std::to_string(x) + ", " + std::to_string(y) + ")");
	int8_t tile_x = x / TILE_SIZE;
	int8_t tile_y = y / TILE_SIZE;
	Log::log(LogType::INFO, "Corresponding to tile position (" + std::to_string(tile_x) + ", " + std::to_string(tile_y) + ")");
	return stages[current_stage]->is_blocked(tile_y, tile_x);
}

COORDS Game::get_tile()const {
	uint8_t tile_x = global_position.x / TILE_SIZE;
	uint8_t tile_y = global_position.y / TILE_SIZE;

	return COORDS{ tile_x, tile_y };
}

void Game::update_position(COORDS coords) {
	global_position.x = coords.x - REGULAR_TILE_OFFSET_X;
	global_position.y = coords.y - REGULAR_TILE_OFFSET_Y;

	COORDS tile = get_tile();

	Log::log(LogType::INFO, "tile: " + std::to_string(tile.x) + ", " + std::to_string(tile.y));
}