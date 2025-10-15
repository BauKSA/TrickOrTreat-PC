#pragma once
#include<string>
#include<memory>
#include<vector>
#include<algorithm>
#include<functional>

#include "Utils.h"
#include "Log.h"
#include "Component.h"
#include "ResourceIDs.h"
#include "Graphics.h"

class Actor {
private:
	uint8_t id;

	int8_t x;
	int8_t y;

	Resources::TextureID texture;
	uint8_t current_sprite;

	CustomTextureID render_texture;            
	TextureSize texture_size;

	std::vector<std::unique_ptr<Component>> components;

	uint8_t index;
public:
	Actor() = default;
	Actor(Actor&&) noexcept = default;              // ✅ movimiento permitido
	Actor& operator=(Actor&&) noexcept = default;   // ✅ movimiento permitido

	Actor(const Actor&) = delete;                  // ❌ prohibir copia
	Actor& operator=(const Actor&) = delete;       // ❌ prohibir copia

	Actor(uint8_t _id, int8_t _x, int8_t _y, uint8_t z = 0)
		: id(_id), x(_x), y(_y), render_texture(0), texture_size{ 0, 0 }, texture(Resources::TextureID::NONE),
		index(z), current_sprite(NONE_SPRITE) {
	}

	~Actor();

	void initialize(Resources::TextureID txt, uint8_t width, uint8_t height, uint8_t offset_x = 0, uint8_t offset_y = 0);
	virtual void update(float delta_time);
	void reverse();

	void draw();

	//Adders
	void add_component(std::unique_ptr<Component> component) {
		if (!component) return;
		components.push_back(std::move(component));
	}

	//Setters
	void set_position(float _x, float _y);
	void set_z_index(uint8_t z) { index = z; }
	void set_sprite(uint8_t sprite) {
		if (current_sprite == sprite) return;
		current_sprite = sprite;
	}

	//Getters
	/*float width()const { return graphics->get_texture_size(current_sprite).width; }
	float height()const { return graphics->get_texture_size(current_sprite).height; }*/
	int8_t get_x()const { return x; }
	int8_t get_y()const { return y; }
	uint8_t z_index()const { return index; }
	uint8_t get_id()const { return id; }
	uint8_t get_sprite()const { return current_sprite; }

	template<typename T>
	T* get_component() {
		for (std::unique_ptr<Component>& c : components) {
			if (T* ptr = dynamic_cast<T*>(c.get())) {
				return ptr;
			}
		}

		return nullptr;
	}

	template<typename T>
	std::vector<T*> get_components() {
		std::vector<T*> comps;
		for (std::unique_ptr<Component>& c : components) {
			if (T* ptr = dynamic_cast<T*>(c.get())) {
				comps.push_back(ptr);
			}
		}

		return comps;
	}
};