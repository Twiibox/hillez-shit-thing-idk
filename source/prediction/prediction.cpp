#include "prediction.hpp"

int misc::prediction_::get_tickbase( player_t* player, CUserCmd* cmd ) {
	static int tick = 0;
	static CUserCmd* last_cmd = nullptr;

	if ( cmd ) {
		if ( !last_cmd || last_cmd->hasbeenpredicted )
			tick = player->tickbase( );

		else tick++;
		last_cmd = cmd;
	}

	return tick;
}

void misc::prediction_::begin( CUserCmd* cmd ) {
	if ( !globals::local_player || globals::local_player->deadflag( ) )
		return;

	globals::local_player->current_cmd( ) = cmd;
	*offsets::seed = MD5_PseudoRandom( cmd->command_number ) & 0x7FFFFFFF;

	old_curtime = offsets::globals->curtime;
	old_frametime = offsets::globals->frametime;
	old_tickcount = offsets::globals->tickcount;

	auto old_tickbase = globals::local_player->tickbase( );
	bool old_first_pred = offsets::prediction->m_bFirstTimePredicted;
	bool old_in_pred = offsets::prediction->m_bInPrediction;

	offsets::globals->tickcount = get_tickbase( globals::local_player, cmd );
	offsets::globals->curtime = TICKS_TO_TIME( offsets::globals->tickcount );
	offsets::globals->frametime = ( offsets::prediction->m_bEnginePaused ? 0.0f : TICK_INTERVAL );

	offsets::prediction->m_bFirstTimePredicted = false;
	offsets::prediction->m_bInPrediction = true;

	offsets::prediction->SetLocalViewAngles( cmd->viewangles );

	offsets::prediction->SetupMove( globals::local_player, cmd, offsets::movehelper, &movedata );
	offsets::movement->ProcessMovement( globals::local_player, &movedata );
	offsets::prediction->FinishMove( globals::local_player, cmd, &movedata );

	globals::local_player->tickbase( ) = old_tickbase;

	offsets::prediction->m_bInPrediction = old_in_pred;
	offsets::prediction->m_bFirstTimePredicted = old_first_pred;
}

void misc::prediction_::finish( ) {
	if ( !globals::local_player || globals::local_player->deadflag( ) )
		return;

	offsets::globals->curtime = old_curtime;
	offsets::globals->frametime = old_frametime;
	offsets::globals->tickcount = old_tickcount;

	globals::local_player->current_cmd( ) = nullptr;
	*offsets::seed = -1;
}