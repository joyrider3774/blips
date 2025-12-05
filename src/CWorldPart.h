#ifndef CWORLDPART_H
#define CWORLDPART_H

#include <SDL3/SDL.h>
#include <vector>
#include "Common.h"
#include "CWorldParts.h"
#include "SPoint.h"

class CWorldParts;

class CWorldPart
{
	protected:
		int MoveDelayCounter;
		bool FirstArriveEventFired,PNeedToKill;
		int Type,MoveSpeed,MoveDelay,Xi,Yi,X,Y,AnimPhase,PlayFieldX,PlayFieldY,Z,Group;
		bool BHide;
		SDL_Texture ** Image;
		std::vector<SPoint> MoveQue;

	public:
		CWorldParts *ParentList;
		bool IsDeath;
		bool IsMoving;
		bool Selected;
		CWorldPart(const int PlayFieldXin,const int PlayFieldYin);
		void Hide();
		bool NeedHide();
		void AddToMoveQue(int PlayFieldXIn,int PlayFieldYIn);
		void Kill();
		bool NeedToKill();
		bool MovesInQue();
		int GetGroup();
		int GetType();
		int GetX();
		int GetY();
		int GetPlayFieldX();
		int GetPlayFieldY();
		int GetZ();
		int GetAnimPhase();
		void SetAnimPhase(int AnimPhaseIn);
		void SetPosition(const int PlayFieldXin,const int PlayFieldYin);
		virtual void Event_ArrivedOnNewSpot();
		virtual void Event_BeforeDraw();
		virtual void Event_LeaveCurrentSpot();
		virtual void Event_Moving(int ScreenPosX,int ScreenPosY,int ScreenXi, int ScreenYi);
		virtual void MoveTo(const int PlayFieldXin,const int PlayFieldYin,bool BackWards);
		virtual bool CanMoveTo(const int PlayFieldXin,const int PlayFieldYin);
		virtual void Move();
		virtual void Draw();
		virtual ~CWorldPart(); 
};

class CEmpty : public CWorldPart
{
 	public:
		CEmpty(const int PlayFieldXin,const int PlayFieldYin);
};

class CWall : public CWorldPart
{
 	public:
		CWall(const int PlayFieldXin,const int PlayFieldYin);
};

class CWallBreakable : public CWorldPart
{
 	public:
		CWallBreakable(const int PlayFieldXin,const int PlayFieldYin);
};


class CFloor : public CWorldPart
{
 	public:
		CFloor(const int PlayFieldXin,const int PlayFieldYin);
};

class CBomb : public CWorldPart
{
 	public:
		CBomb(const int PlayFieldXin,const int PlayFieldYin);
};

class CDiamond : public CWorldPart
{
	public:
		CDiamond(const int PlayFieldXin,const int PlayFieldYin);
};

class CExplosion : public CWorldPart
{
	private:
  		int AnimCounter,AnimDelay,AnimDelayCounter,AnimPhases;
  		void Event_BeforeDraw();
 	public:
		CExplosion(const int PlayFieldXin,const int PlayFieldYin);
};


class CBox : public CWorldPart
{
 	private:
 		void Event_ArrivedOnNewSpot();
 		bool CanMoveTo(const int PlayFieldXin,const int PlayFieldYin);
 	public:
		CBox(const int PlayFieldXin,const int PlayFieldYin);
};

class CBox1 : public CWorldPart
{
 	private:
 		void Event_ArrivedOnNewSpot();
 		bool CanMoveTo(const int PlayFieldXin,const int PlayFieldYin);
 	public:
		CBox1(const int PlayFieldXin,const int PlayFieldYin);
};

class CBox2 : public CWorldPart
{
 	private:
 		void Event_ArrivedOnNewSpot();
 		bool CanMoveTo(const int PlayFieldXin,const int PlayFieldYin);
 	public:
		CBox2(const int PlayFieldXin,const int PlayFieldYin);
};

class CBoxWall : public CWorldPart
{
 	private:
 		void Event_ArrivedOnNewSpot();
 		bool CanMoveTo(const int PlayFieldXin,const int PlayFieldYin);
 	public:
		CBoxWall(const int PlayFieldXin,const int PlayFieldYin);
};

class CBoxBomb : public CWorldPart
{
 	private:
 		void Event_ArrivedOnNewSpot();
 		bool CanMoveTo(const int PlayFieldXin,const int PlayFieldYin);
 	public:
		CBoxBomb(const int PlayFieldXin,const int PlayFieldYin);
};

#endif