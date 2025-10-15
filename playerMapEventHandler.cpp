#include "PlayerMapEventHandler.h"
#include "Actor.h"
#include "InterpolationMovement.h"
#include "Game.h"

void PlayerMapEventHandler::move(Directions dir, Actor& actor) {
	InterpolationMovement* movement = actor.get_component<InterpolationMovement>();
	if (!movement) return;

	movement->move(dir);
}

void PlayerMapEventHandler::handle(Actor& actor, Event e) {
	if (e.type == Type::KeyPressed) {
		switch (e.key) {
		case Key::Keyboard::UP:
			if (check_position(actor.get_x() - REGULAR_TILE_OFFSET_X, actor.get_y() - REGULAR_TILE_OFFSET_Y - TILE_SIZE)) move(Directions::UP, actor);
			break;
		case Key::Keyboard::DOWN:
			if (check_position(actor.get_x() - REGULAR_TILE_OFFSET_X, actor.get_y() - REGULAR_TILE_OFFSET_Y + TILE_SIZE)) move(Directions::DOWN, actor);
			break;
		case Key::Keyboard::RIGHT:
			if (check_position(actor.get_x() - REGULAR_TILE_OFFSET_X + TILE_SIZE, actor.get_y() - REGULAR_TILE_OFFSET_Y)) move(Directions::RIGHT, actor);
			break;
		case Key::Keyboard::LEFT:
			if (check_position(actor.get_x() - REGULAR_TILE_OFFSET_X - TILE_SIZE, actor.get_y() - REGULAR_TILE_OFFSET_Y)) move(Directions::LEFT, actor);
			break;
		default:
			break;
		}
	}
}

bool PlayerMapEventHandler::check_position(int8_t x, int8_t y) {
	return !Game::instance().tile_block(x, y);
}