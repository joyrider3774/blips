#ifndef CPLAYER_H
#define CPLAYER_H

#include "CWorldPart.h"

class CPlayer : public CWorldPart
{
 private:
 	int AnimCounter,AnimBase,AnimDelay,AnimDelayCounter,AnimPhases;
 	void Event_BeforeDraw();
 public:
    CPlayer(const int PlayFieldXin,const int PlayFieldYin);
	bool IsDeath;
 	bool CanMoveTo(const int PlayFieldXin,const int PlayFieldYin);
 	void Event_Moving(int ScreenPosX,int ScreenPosY,int ScreenXi,int ScreenYi);
 	void MoveTo(const int PlayFieldXin,const int PlayFieldYin,bool BackWards);
 	void Event_ArrivedOnNewSpot();
 	void MoveTo(const int PlayFieldXin,const int PlayFieldYin);
};

#endif