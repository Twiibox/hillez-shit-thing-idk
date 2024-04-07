#pragma once
#include "../include/globals.hpp"

namespace misc {
	class prediction_ {
		CMoveData movedata = { };
		float old_curtime = .0f, old_frametime = .0f;
		int old_tickcount = 0;
		int get_tickbase( player_t* player, CUserCmd* cmd );
	public:
		void begin( CUserCmd* cmd );
		void finish( );
	};

	inline prediction_* prediction = new prediction_;
}