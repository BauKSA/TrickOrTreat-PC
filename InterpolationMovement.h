#pragma once
#include<stdint.h>

#include "Component.h"
#include "Utils.h"

class InterpolationMovement : public Component {
private:
	int8_t lerp(int8_t start, int8_t end, int8_t percentage = 25);

	bool moving = false;
	Directions direction = Directions::NONE;

	uint8_t progress = 0;
	uint8_t target = 0;

	bool target_set = true;

	uint8_t move_tile;
public:
	InterpolationMovement(uint8_t tile) : move_tile(tile) {};
	void update(float delta_time, Actor& owner) override;

	void move(Directions dir) {
		if (moving) return;
		direction = dir;
		moving = true;
		target_set = false;
	};

	void stop() {
		moving = false;
		direction = Directions::NONE;
		progress = 0;
	};
};