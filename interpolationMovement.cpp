#include "InterpolationMovement.h"
#include "Actor.h"

int8_t InterpolationMovement::lerp(int8_t start, int8_t end, int8_t percentage) {
	return start + (end - start) * percentage / 100;
}

void InterpolationMovement::update(float delta_time, Actor& owner) {
	if (!moving) return;
	if (direction == Directions::NONE) return;

	int8_t x = owner.get_x();
	int8_t y = owner.get_y();

	switch (direction) {
	case Directions::UP:
		y = lerp(owner.get_y(), owner.get_y() - move_tile);
		if (!target_set) target = owner.get_y() - move_tile;
		if (y <= target) {
			y = target;
			stop();
		}

		break;
	case Directions::DOWN:
		y = lerp(owner.get_y(), owner.get_y() + move_tile);
		if (!target_set) target = owner.get_y() + move_tile;
		if (y >= target) {
			y = target;
			stop();
		}

		break;
	case Directions::RIGHT:
		x = lerp(owner.get_x(), owner.get_x() + move_tile);
		if (!target_set) target = owner.get_x() + move_tile;
		if (x >= target) {
			x = target;
			stop();
		}

		break;
	case Directions::LEFT:
		x = lerp(owner.get_x(), owner.get_x() - move_tile);
		if (!target_set) target = owner.get_x() - move_tile;
		if (x <= target) {
			x = target;
			stop();
		}

		break;
	default:
		return;
	}

	if (!target_set) target_set = true;

	owner.set_position(x, y);
	Log::log(LogType::INFO, "pos -> x: " + std::to_string(x) + ", y:" + std::to_string(y));

	return;
}