#include<iostream>
#include<memory>
#include<vector>
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>

#include "tileset.h"

#include "Graphics.h"
#include "Stage.h"
#include "ResourceIDs.h"
#include "Utils.h"
#include "TextManager.h"

#include "Game.h"

int main() {
	std::cout << "Game started" << std::endl;

	Game& game = Game::instance();

	TextManager& text = TextManager::instance();

	Text txt{};

	strncpy_s(txt.first_line, sizeof(txt.first_line), "WELCOME TO TRICK OR", _TRUNCATE);
	strncpy_s(txt.second_line, sizeof(txt.second_line), "TREAT. USE K TO ACTION", _TRUNCATE);

	Text txt_2{};

	strncpy_s(txt_2.first_line, sizeof(txt.first_line), "AND WASD TO MOVE.", _TRUNCATE);
	strncpy_s(txt_2.second_line, sizeof(txt.second_line), "GOOD LUCK!", _TRUNCATE);

	const Text* welcome_text[2] = { &txt, &txt_2 };
	text.start_text(welcome_text, 2, [](void* ctx) {
		Game* g = (Game*)ctx;

		g->change_state(GameState::MAP);
		}, &game);

	while (graphics->is_open()) {
		graphics->clear();
		text.update(0.2f);
		game.tick();
		graphics->display();
	}

	return 0;
}