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

	text.start_text("TRICK OR TREAT TRICK OR TREAT TRICK OR TREAT");

	while (graphics->is_open()) {
		graphics->clear();
		text.update(0.2f);
		graphics->display();
		game.tick();
	}

	return 0;
}