#pragma once
#include "Actor.h"
#include "ResourceIDs.h"
#include "InterpolationMovement.h"

class Player : public Actor {
	public:
	Player()
		: Actor(1, 10, 28, 0) {
		initialize(Resources::TextureID::PLAYER_MAP, 4, 4, 0, 0);

		add_component(std::make_unique<InterpolationMovement>(4));
	}

	~Player() {};
};