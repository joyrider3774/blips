#ifndef CWORLDPARTS_H
#define CWORLDPARTS_H

#include "Defines.h"
#include "CViewPort.h"

class CWorldPart;

class CWorldParts
{
	private:
		void Sort();
		bool DisableSorting;

	public:
		CViewPort *ViewPort;
		CWorldPart *Items[NrOfRows*NrOfCols*2];
		CWorldPart *MoveAbleItems[NrOfRows*NrOfCols*2];
		CWorldPart *Player,*Player1,*Player2;
		int ActivePlayer;
		int ItemCount, MoveAbleItemCount, ActivePlayerFlicker;
		CWorldParts();
		void Add(CWorldPart *WorldPart);
		void CenterVPOnPlayer();
		void LimitVPLevel();
		void Move();
		void Draw();
		void Remove(int PlayFieldXin,int PlayFieldYin);
		void Remove(int PlayFieldXin,int PlayFieldYin,int Type);
		void RemoveAll();
		void SwitchPlayers();
		void Save(const char *Filename);
		void Load(const char *Filename);
		~CWorldParts();
};

#endif