/*
  Infinitag_Core.cpp - Core Library for the Infinitag System.
  Created by Jani Taxidis & Tobias Stewen & Florian Kleene, January 19, 2017.
  Info: www.infinitag.io

  All files are published under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
  License: https://creativecommons.org/licenses/by-nc-sa/4.0/
*/

// Basic Libs
#include "Arduino.h"
#include <Wire.h>

// Infinitag
#include <Infinitag_Core.h>


Infinitag_Core::Infinitag_Core()
{
}

unsigned long Infinitag_Core::irEncode(bool isSystem, unsigned int gameId, unsigned int teamId, unsigned int playerId, unsigned int cmd, unsigned int cmdValue){
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
  // Still needs to be calculated
  int checkBits = 0;
  for(int i = 0; i < 23;i++){
    if (tmpBits[i] == 1) {
      checkBits++;
    }
  }
  tmpBits[23] = (checkBits % 2);
  
  return bitsToLong(tmpBits);
}

unsigned long Infinitag_Core::bitsToLong(int *tmpBits) {
  unsigned long result = 0;
  
  for(int i = 0; i < 24;i++){
    // + 0.49 due to pow and float
    result += round(tmpBits[24 - i - 1] * (pow(2, i) + 0.49));
  }
  
  return result;
}

void Infinitag_Core::irToBytes(unsigned long code, byte *result) {
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
  
  
  // Checkbit check
  if (checkBits % 2 != tmpBits[23]) {
    return false;
  }
  
  //alternate: r1 | (r2 << 1) | (r3 << 2) | (r4 << 3) | (r5 << 4);
  char byteOne = (tmpBits[0] << 7) | (tmpBits[1] << 6) | (tmpBits[2] << 5) | (tmpBits[3] << 4) | (tmpBits[4] << 3) | (tmpBits[5] << 2) | (tmpBits[6] << 1) | tmpBits[7];
  char byteTwo = (tmpBits[8] << 7) | (tmpBits[9] << 6) | (tmpBits[10] << 5) | (tmpBits[11] << 4) | (tmpBits[12] << 3) | (tmpBits[13] << 2) | (tmpBits[14] << 1) | tmpBits[15];
  char byteThree = (tmpBits[16] << 7) | (tmpBits[17] << 6) | (tmpBits[18] << 5) | (tmpBits[19] << 4) | (tmpBits[20] << 3) | (tmpBits[21] << 2) | (tmpBits[22] << 1) | tmpBits[23];

  result[0] = byteOne;
  result[1] = byteTwo;
  result[2] = byteThree;
}

void Infinitag_Core::irDecode(unsigned long code){
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
   
   // Checkbit check
   if (checkBits % 2 != tmpBits[23]) {
     return false;
   }
   
  // System
  irRecvIsSystem = tmpBits[0];
  
  // GameId
  x = 0;
  for(int i = 0; i < 2;i++){
    bitWrite(x, i, tmpBits[(2 - i)]);
  }
  irRecvGameId = x;
  
  // TeamId
  x = 0;
  for(int i = 0; i < 3;i++){
    bitWrite(x, i, tmpBits[(5 - i)]);
  }
  irRecvTeamId = x;
  
  // PlayerId
  x = 0;
  for(int i = 0; i < 5;i++){
    bitWrite(x, i, tmpBits[(10 - i)]);
  }
  irRecvPlayerId = x;
  
  // Command
  x = 0;
  for(int i = 0; i < 4;i++){
    bitWrite(x, i, tmpBits[(14 - i)]);
  }
  irRecvCmd = x;
  
  // CommandValue
  x = 0;
  for(int i = 0; i < 8;i++){
    bitWrite(x, i, tmpBits[(22 - i)]);
  }
  irRecvCmdValue = x;
  
  // CheckBit
  x = 0;
  for(int i = 0; i < 1;i++){
    bitWrite(x, i, tmpBits[(23 - i)]);
  }
  irRecvCheckBit = x;
}

void Infinitag_Core::irDecode(byte *data){
  int tmpBits[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  
  for (int i = 0; i < 8; i++) {
    tmpBits[7 - i] = bitRead(data[1], i);
  }
  for (int i = 0; i < 8; i++) {
    tmpBits[15 - i] = bitRead(data[2], i);
  }
  for (int i = 0; i < 8; i++) {
    tmpBits[23 - i] = bitRead(data[3], i);
  }
  
  irDecode(bitsToLong(tmpBits));
}


void Infinitag_Core::irDump(unsigned long code){
  for(int i = 0; i < 24;i++){
    Serial.print(bitRead(code, 23 - i));
    if (i == 0 || i == 2 || i == 5 || i == 10 || i == 14 || i == 22) {
      Serial.print("|");
    }
  }
  Serial.println("");
}

/*
 * Internal Commands
 */
void Infinitag_Core::sendCmdToSensors(byte data[], unsigned int byteLength) {
  // ToDo: correct destination
  Wire.beginTransmission(0x22);
  Wire.write(data, byteLength);
  Wire.endTransmission();
}

void Infinitag_Core::sendCmdSetGameId(unsigned int gameId) {
  byte data[2] = {
    0x01, 
    gameId
  };
  sendCmdToSensors(data, 2);
}

void Infinitag_Core::sendCmdSetTeamId(unsigned int teamId) {
  byte data[2] = {
    0x02, 
    teamId
  };
  sendCmdToSensors(data, 2);
}

void Infinitag_Core::sendCmdSetPlayerId(unsigned int playerId) {
  byte data[2] = {
    0x03, 
    playerId
  };
  sendCmdToSensors(data, 2);
}

void Infinitag_Core::sendCmdSetSensorId(unsigned int sensorId) {
  byte data[2] = {
    0x04, 
    sensorId
  };
  sendCmdToSensors(data, 2);
}

void Infinitag_Core::sendCmdSetAnimation(unsigned int animationId, unsigned int duration, unsigned int colorR, unsigned int colorG, unsigned int colorB, unsigned int colorW, unsigned int repeat) {
  byte data[9] = {
    0x05, 
    animationId, 
    duration, 
    duration >> 8, 
    colorR, 
    colorG, 
    colorB, 
    colorW, 
    repeat
  };
  sendCmdToSensors(data, 9);
}

void Infinitag_Core::sendCmdPing(unsigned int senderId) {
  byte data[2] = {
    0x06, 
    senderId
  };
  sendCmdToSensors(data, 2);
}

void Infinitag_Core::sendCmdPingSetAlive(bool alive) {
  byte data[2] = {
    0x07, 
    (alive) ? 0x01 : 0x00
  };
  sendCmdToSensors(data, 2);
}
