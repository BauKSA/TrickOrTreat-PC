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

	strncpy_s(txt.first_line, sizeof(txt.first_line), "TRICK OR TREAT TRICK", _TRUNCATE);
	strncpy_s(txt.second_line, sizeof(txt.second_line), "OR TREAT HEHE", _TRUNCATE);

	const Text* texts[1] = { &txt };
	text.start_text(texts, 1);

	while (graphics->is_open()) {
		graphics->clear();
		text.update(0.2f);
		graphics->display();
		game.tick();
	}

	return 0;
}