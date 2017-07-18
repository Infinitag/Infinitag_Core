/*
  Infinitag_Core.cpp - Core Library for the Infinitag System.
  Created by Jani Taxidis & Tobias Stewen & Florian Kleene, January 19, 2017.
  Info: www.infinitag.io

  All files are published under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
  License: https://creativecommons.org/licenses/by-nc-sa/4.0/
*/

#include "Arduino.h"

// Infinitag
#include <Infinitag_Core.h>


Infinitag_Core::Infinitag_Core()
{
}

unsigned long Infinitag_Core::ir_encode(bool isSystem, unsigned int gameId, unsigned int teamId, unsigned int playerId, unsigned int cmd, unsigned int cmdValue){
	int tmpBits[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	unsigned long finalVal = 0;
	byte x;
  
	// System
	tmpBits[0] = (isSystem == true)?1:0;

	// GameId	
	x = gameId; 
	for(int i = 0; i < 2;i++){
		tmpBits[(2 - i)] = bitRead(x, i);
	}
  
	// TeamId
	x = teamId; 
	for(int i = 0; i < 3;i++){
		tmpBits[2 + (3 - i)] = bitRead(x, i);
	}

	// PlayerId
	x = playerId; 
	for(int i = 0; i < 5;i++){
		tmpBits[5 + (5 - i)] = bitRead(x, i);
	}
  
	// Command
	x = cmd; 
	for(int i = 0; i < 4;i++){
		tmpBits[10 + (4 - i)] = bitRead(x, i);
	}
  
	// Command value
	x = cmdValue; 
	for(int i = 0; i < 8;i++){
		tmpBits[14 + (8 - i)] = bitRead(x, i);
	}
	
	// CheckBit
	// Muss noch errechnet werden
	int checkBits = 0;
	for(int i = 0; i < 23;i++){
		if (tmpBits[i] == 1) {
			checkBits++;
		}
	}
	tmpBits[23] = (checkBits % 2);
  
	for(int i = 0; i < 24;i++){
		// + 0.49 wegen pow und float
		finalVal += round(tmpBits[24 - i - 1] * (pow(2, i) + 0.49));
	}
  
	return finalVal;
}


void Infinitag_Core::ir_to_bytes(unsigned long code, byte *result) {
	if(code > 16777215) {
		return false;
	}
	
	int tmpBits[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int checkBits = 0;
	
	for(int i = 0; i < 24;i++){
		tmpBits[i] =  bitRead(code, 23 - i);
		if (i < 23 && tmpBits[i] == 1) {
			checkBits++;
		}
	}
	
	
	// Checkbit prüfen
	if (checkBits % 2 != tmpBits[23]) {
		return false;
	}
	
	//alternate: r1 | (r2 << 1) | (r3 << 2) | (r4 << 3) | (r5 << 4);
	char byte_one = (tmpBits[0] << 7) | (tmpBits[1] << 6) | (tmpBits[2] << 5) | (tmpBits[3] << 4) | (tmpBits[4] << 3) | (tmpBits[5] << 2) | (tmpBits[6] << 1) | tmpBits[7];
	char byte_two = (tmpBits[8] << 7) | (tmpBits[9] << 6) | (tmpBits[10] << 5) | (tmpBits[11] << 4) | (tmpBits[12] << 3) | (tmpBits[13] << 2) | (tmpBits[14] << 1) | tmpBits[15];
	char byte_three = (tmpBits[16] << 7) | (tmpBits[17] << 6) | (tmpBits[18] << 5) | (tmpBits[19] << 4) | (tmpBits[20] << 3) | (tmpBits[21] << 2) | (tmpBits[22] << 1) | tmpBits[23];
	
	byte data[3] = {
		byte_one,
		byte_two,
		byte_three
	};
	
	result = data;
}

boolean Infinitag_Core::ir_decode(unsigned long code){
	if(code > 16777215) {
		return false;
	}
	
	int tmpBits[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	byte x;
	
	int checkBits = 0;
	for(int i = 0; i < 24;i++){
		tmpBits[i] =  bitRead(code, 23 - i);
		if (i < 23 && tmpBits[i] == 1) {
			checkBits++;
		}
	}
   
   // Checkbit prüfen
   if (checkBits % 2 != tmpBits[23]) {
	   return false;
   }
   
	// System
	ir_recv_is_system = tmpBits[0];
  
	// GameId
	x = 0;
	for(int i = 0; i < 2;i++){
		bitWrite(x, i, tmpBits[(2 - i)]);
	}
	ir_recv_game_id = x;
  
	// TeamId
	x = 0;
	for(int i = 0; i < 3;i++){
		bitWrite(x, i, tmpBits[(5 - i)]);
	}
	ir_recv_team_id = x;
  
	// PlayerId
	x = 0;
	for(int i = 0; i < 5;i++){
		bitWrite(x, i, tmpBits[(10 - i)]);
	}
	ir_recv_player_id = x;
  
	// Command
	x = 0;
	for(int i = 0; i < 4;i++){
		bitWrite(x, i, tmpBits[(14 - i)]);
	}
	ir_recv_cmd = x;
  
	// CommandValue
	x = 0;
	for(int i = 0; i < 8;i++){
		bitWrite(x, i, tmpBits[(22 - i)]);
	}
	ir_recv_cmd_value = x;
	
	// CheckBit
	x = 0;
	for(int i = 0; i < 1;i++){
		bitWrite(x, i, tmpBits[(23 - i)]);
	}
	ir_recv_check_bit = x;
  
	return true;
}


void Infinitag_Core::ir_dump(unsigned long code){
	for(int i = 0; i < 24;i++){
		Serial.print(bitRead(code, 23 - i));
		if (i == 0 || i == 2 || i == 5 || i == 10 || i == 14 || i == 22) {
			Serial.print("|");
		}
	}
	Serial.println("");
}