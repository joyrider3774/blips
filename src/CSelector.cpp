
#include <SDL3/SDL.h>
#include "CSelector.h"
#include "CWorldParts.h"
#include "CWorldPart.h"
#include "CPlayer.h"

CSelector::CSelector(CWorldParts *List)
{
    PList = List;
	Selection = IDEmpty;
	Part = new CEmpty(0,0);
	Part->ParentList = PList;
	Part->Selected = true;
}

void CSelector::SetPosition(const int PlayFieldXin,const int PlayFieldYin)
{
    Part->SetPosition(PlayFieldXin,PlayFieldYin);
}

void CSelector::MoveUp()
{

	Part->SetPosition(Part->GetPlayFieldX(),Part->GetPlayFieldY()-1);
}

void CSelector::MoveDown()
{
	Part->SetPosition(Part->GetPlayFieldX(),Part->GetPlayFieldY()+1);
}

void CSelector::MoveRight()
{
	Part->SetPosition(Part->GetPlayFieldX() +1,Part->GetPlayFieldY());
}

void CSelector::MoveLeft()
{
	Part->SetPosition(Part->GetPlayFieldX() -1,Part->GetPlayFieldY());
}

void CSelector::IncSelection()
{
	int X,Y;
	Selection++;
	if (Selection > 13)
		Selection = 1;
	X = Part->GetPlayFieldX();
	Y = Part->GetPlayFieldY();
	delete Part;
	switch (Selection)
	{
		case IDEmpty:
			Part = new CEmpty(X,Y);
			break;
		case IDBox:
			Part = new CBox(X,Y);
			break;
		case IDPlayer:
			Part = new CPlayer(X,Y);
			break;
		case IDBomb:
			Part = new CBomb(X,Y);
			break;
		case IDWall:
			Part = new CWall(X,Y);
			break;
		case IDFloor:
			Part = new CFloor(X,Y);
			break;
        case IDDiamond:
			Part = new CDiamond(X,Y);
			break;
		case IDPlayer2:
			Part = new CPlayer2(X,Y);
			break;
		case IDBox1:
			Part = new CBox1(X,Y);
			break;
		case IDBox2:
			Part = new CBox2(X,Y);
			break;
		case IDBoxBomb:
			Part = new CBoxBomb(X,Y);
			break;
		case IDBoxWall:
			Part = new CBoxWall(X,Y);
			break;
		case IDWallBreakable:
			Part = new CWallBreakable(X,Y);
			break;
	}
	Part->ParentList = PList;
	Part->Selected = true;
}

void CSelector::DecSelection()
{
	int X,Y;
	Selection--;
	if (Selection < 1)
		Selection = 13;
	X = Part->GetPlayFieldX();
	Y = Part->GetPlayFieldY();
	delete Part;
    switch (Selection)
	{
		case IDEmpty:
			Part = new CEmpty(X,Y);
			break;
		case IDBox:
			Part = new CBox(X,Y);
			break;
		case IDPlayer:
			Part = new CPlayer(X,Y);
			break;
		case IDBomb:
			Part = new CBomb(X,Y);
			break;
		case IDWall:
			Part = new CWall(X,Y);
			break;
		case IDFloor:
			Part = new CFloor(X,Y);
			break;
         case IDDiamond:
			Part = new CDiamond(X,Y);
			break;
		case IDPlayer2:
			Part = new CPlayer2(X,Y);
			break;
		case IDBox1:
			Part = new CBox1(X,Y);
			break;
		case IDBox2:
			Part = new CBox2(X,Y);
			break;
		case IDBoxBomb:
			Part = new CBoxBomb(X,Y);
			break;
		case IDBoxWall:
			Part = new CBoxWall(X,Y);
			break;
		case IDWallBreakable:
			Part = new CWallBreakable(X,Y);
			break;
	}

	Part->ParentList = PList;
	Part->Selected = true;
}

void CSelector::Draw()
{
	Part->Draw();
}

int CSelector::GetSelection()
{
	return Selection;
}

int CSelector::GetPlayFieldX()
{
	return Part->GetPlayFieldX();
}

int CSelector::GetPlayFieldY()
{
	return Part->GetPlayFieldY();
}

CSelector::~CSelector()
{
	delete Part;
}
