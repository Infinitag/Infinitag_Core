/*
  Infinitag_Core.h - Library for the Infinitag System.
  Created by Jani Taxidis & Tobias Stewen, August 22, 2016.
  Info: www.infinitag.io
*/

#ifndef Infinitag_Core_h
	#define Infinitag_Core_h

	#include "Arduino.h"

	class Infinitag_Core
	{
		public:
			Infinitag_Core();
	
			unsigned long ir_encode(bool isSystem, unsigned int gameId, unsigned int teamId, unsigned int playerId, unsigned int cmd, unsigned int cmdValue);
			bool ir_decode(unsigned long code);
			void ir_to_bytes(unsigned long code, byte *result);
			void ir_dump(unsigned long code);
			
			unsigned int ir_recv_is_system;
			unsigned int ir_recv_game_id;
			unsigned int ir_recv_team_id;
			unsigned int ir_recv_player_id;
			unsigned int ir_recv_cmd;
			unsigned int ir_recv_cmd_value;
			unsigned int ir_recv_check_bit;
			
		private:
	};

#endif