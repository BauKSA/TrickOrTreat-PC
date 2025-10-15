#include "Actor.h"
#include "Graphics.h"

Actor::~Actor() {};

void Actor::draw() {
	graphics->draw_sprite(current_sprite, x, y);
}

void Actor::initialize(Resources::TextureID txt, uint8_t width, uint8_t height, uint8_t offset_x, uint8_t offset_y) {
	graphics->load_texture(txt, width, height);
	texture = txt;

	graphics->create_sprite(texture, id, offset_x, offset_y);
	current_sprite = id;
}

void Actor::Actor::update(float delta_time) {
	for (std::unique_ptr<Component>& c : components)
		c->update(delta_time, *this);
	
	draw();
}

void Actor::set_position(float _x, float _y) {
	if (_x == x && _y == y) return;
	x = _x; y = _y;
}