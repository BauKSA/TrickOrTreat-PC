#pragma once
#include "EventHandler.h"
#include "Utils.h"

class PlayerMapEventHandler : public EventHandler {
using EventHandler::handle;
private:
	void move(Directions dir, Actor& actor);
	bool check_position(int8_t x, int8_t y);
public:
	void handle(Actor& actor, Event e) override;
};