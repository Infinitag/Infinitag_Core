/*
  Infinitag_Core.h - Core Library for the Infinitag System.
  Created by Jani Taxidis & Tobias Stewen & Florian Kleene, January 19, 2017.
  Info: www.infinitag.io

  All files are published under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
  License: https://creativecommons.org/licenses/by-nc-sa/4.0/
*/

#ifndef Infinitag_Core_h
  #define Infinitag_Core_h

  #include "Arduino.h"

  class Infinitag_Core
  {
    public:
      Infinitag_Core();
  
      unsigned long irEncode(bool isSystem, unsigned int gameId, unsigned int teamId, unsigned int playerId, unsigned int cmd, unsigned int cmdValue);
      void irDecode(unsigned long code);
      void irDecode(byte *data);
      void irToBytes(unsigned long code, byte *result);
      unsigned long bitsToLong(int *tmpBits);
      void irDump(unsigned long code);
      
      void sendCmdToSensors(byte data[], unsigned int byteLength);
      void sendCmdSetGameId(unsigned int gameId);
      void sendCmdSetTeamId(unsigned int teamId);
      void sendCmdSetPlayerId(unsigned int playerId);
      void sendCmdSetSensorId(unsigned int sensorId);
      void sendCmdSetAnimation(unsigned int animationId, unsigned int duration, unsigned int colorR, unsigned int colorG, unsigned int colorB, unsigned int colorW, unsigned int repeat);
      void sendCmdPing(unsigned int senderId);
      void sendCmdPingSetAlive(bool alive);
      
      unsigned int irRecvIsSystem;
      unsigned int irRecvGameId;
      unsigned int irRecvTeamId;
      unsigned int irRecvPlayerId;
      unsigned int irRecvCmd;
      unsigned int irRecvCmdValue;
      unsigned int irRecvCheckBit;
      
    private:
  };

#endif