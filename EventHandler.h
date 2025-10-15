#pragma once
#include "Utils.h"

class Actor;

class EventHandler {
public:
	EventHandler() = default;
	virtual ~EventHandler() = default;

	virtual void handle(Actor& actor, Event e) = 0;
};