#include "create1_1.h"

void Stage1_1::handle_events() {
	Game& game = Game::instance();
	for (size_t i = 0; i < event_count(); i++) {
		if (get_events()[i].x == game.get_tile().x
			&& get_events()[i].y == game.get_tile().y) {
			game.change_state(GameState::DIALOGUE);

			Text txt{};

			strncpy_s(txt.first_line, sizeof(txt.first_line), "FIRST EVENT TEXT!", _TRUNCATE);
			strncpy_s(txt.second_line, sizeof(txt.second_line), "IT IS OKAY TO YOU?", _TRUNCATE);

			first_event_txt = txt;

			first_event[0] = &first_event_txt;

			TextManager::instance().start_text(first_event, 1, [](void* ctx) {
				Game* g = (Game*)ctx;

				g->update_position({ 0, 0 });
				g->change_state(GameState::MAP);
				}, &game);
		}
	}
}