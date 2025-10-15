#pragma once
#include "Stage.h"
#include "Actor.h"
#include "Player.h"
#include "PlayerMapEventHandler.h"

enum class GameState {
	MAP,
	BOSS_FIGHT,
	DIALOGUE
};

class Game {
private:
	GameState state;
	Stage stages[STAGES];
	Actor map_player;
	//Actor fight_player;

	COORDS global_position;
	int8_t current_stage = -1;

	PlayerMapEventHandler map_handler;
public:
	Game() :
		state(GameState::MAP),
		global_position({ 0,0 }),
		map_player(Player()),
		map_handler() {
		global_position.x = map_player.get_x();
		global_position.y = map_player.get_y();

		current_stage = 0;
		initialize();
	};

	static Game& instance() {
		static Game instance;
		return instance;
	}

	void initialize();
	void tick(uint16_t delta_time = 0);
	void handle_event(Event e);

	bool tile_block(int8_t x, int8_t y);

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
};