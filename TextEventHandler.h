#pragma once
#include "EventHandler.h"
#include "TextManager.h"

class TextEventHandler : public EventHandler {
using EventHandler::handle;
public:
	void handle(Event e) {
		if (e.type != Type::KeyPressed) return;
		if (e.key != Key::Keyboard::ACTION) return;

		return TextManager::instance().next_text();
	}
};