#ifndef CSELECTOR_H
#define CSELECTOR_H

#include <SDL.h>
#include "CWorldParts.h"
#include "CWorldPart.h"

class CSelector
{
 private:
    CWorldParts *PList;
 	CWorldPart *Part;
 	int Selection;
 public:
	CSelector(CWorldParts *List);
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void IncSelection();
	void DecSelection();
	void SetPosition(const int PlayFieldXin,const int PlayFieldYin);
	void Draw(SDL_Surface *Surface);
	int GetSelection();
	int GetPlayFieldX();
	int GetPlayFieldY();
	~CSelector();
};

#endif