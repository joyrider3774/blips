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
		int ItemCount;
		CWorldParts();
		void Add(CWorldPart *WorldPart);
		void CenterVPOnPlayer();
		void LimitVPLevel();
		void Move();
		void Draw(SDL_Surface *Surface);
		void Remove(int PlayFieldXin,int PlayFieldYin);
		void Remove(int PlayFieldXin,int PlayFieldYin,int Type);
		void RemoveAll();
		void Save(char *Filename);
		void Load(char *Filename);
		~CWorldParts();
};

#endif