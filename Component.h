#pragma once

class Actor;

class Component {
public:
	virtual void update(float delta_time, Actor& owner) = 0;
	virtual ~Component() = default;
};