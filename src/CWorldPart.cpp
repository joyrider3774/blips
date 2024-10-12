#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_mixer.h>
#include "Common.h"
#include "CWorldPart.h"
#include "SPoint.h"

CWorldPart::CWorldPart(const int PlayFieldXin,const int PlayFieldYin)
{
	PlayFieldX=PlayFieldXin;
	PlayFieldY=PlayFieldYin;
	Xi=0;
	Yi=0;
	X=PlayFieldXin*TileWidth;
	Y=PlayFieldYin*TileHeight;
	Type=0;
	MoveDelay=0;
	MoveDelayCounter=0;
	IsMoving = false;
	MoveSpeed=0;
	Image = NULL;
	ParentList = 0;
	AnimPhase=0;
	Selected = false;
	FirstArriveEventFired = false;
	Z=0;
	Group=0;
	PNeedToKill = false;
	BHide = false;
}


void CWorldPart::Hide() 
{
	BHide = true;
}

void CWorldPart::AddToMoveQue(int PlayFieldXIn,int PlayFieldYIn)
{
	SPoint Temp;
	Temp.X = PlayFieldXIn;
	Temp.Y = PlayFieldYIn;
	if (MoveQue.size() == 0)
		MoveQue.push_back(Temp);
	else
		MoveQue.insert(MoveQue.begin(),Temp);
}

void CWorldPart::Kill()
{
	PNeedToKill = true;
}

bool CWorldPart::NeedToKill() 
{
	return PNeedToKill;
}

bool CWorldPart::MovesInQue() 
{ 
	return (MoveQue.size() > 0);
}

int CWorldPart::GetGroup() 
{
	return Group;
}

int CWorldPart::GetType() 
{
	return Type;
}

int CWorldPart::GetX() 
{
	return X;
}

int CWorldPart::GetY() 
{
	return Y;
}

int CWorldPart::GetPlayFieldX() 
{
	return PlayFieldX;
}

int CWorldPart::GetPlayFieldY() 
{
	return PlayFieldY;
}

int CWorldPart::GetZ() 
{
	return Z;
}

int CWorldPart::GetAnimPhase() 
{
	return AnimPhase;
}

void CWorldPart::SetAnimPhase(int AnimPhaseIn) 
{ 
	AnimPhase = AnimPhaseIn;
}

void CWorldPart::Event_ArrivedOnNewSpot()
{

}

void CWorldPart::Event_BeforeDraw() 
{

}

void CWorldPart::Event_LeaveCurrentSpot() 
{

}

void CWorldPart::Event_Moving(int ScreenPosX,int ScreenPosY,int ScreenXi, int ScreenYi) 
{

}

void CWorldPart::SetPosition(const int PlayFieldXin,const int PlayFieldYin)
{
	if ((PlayFieldXin >= 0) && (PlayFieldXin < NrOfCols) && (PlayFieldYin >= 0) && (PlayFieldYin < NrOfRows))
	{
		PlayFieldX=PlayFieldXin;
		PlayFieldY=PlayFieldYin;
		X=PlayFieldXin*TileWidth;
		Y=PlayFieldYin*TileHeight;
		//Event_ArrivedOnNewSpot();
	}
}

void CWorldPart::MoveTo(const int PlayFieldXin,const int PlayFieldYin,bool BackWards)
{
	if(!IsMoving)
	{
		if((PlayFieldXin != PlayFieldX) || (PlayFieldYin != PlayFieldY))
			if(this->CanMoveTo(PlayFieldXin,PlayFieldYin) || BackWards)
			{
				PlayFieldX = PlayFieldXin;
				PlayFieldY = PlayFieldYin;
				if(X < PlayFieldX*TileWidth)
					Xi = MoveSpeed;
				if(X > PlayFieldX*TileWidth)
					Xi = -MoveSpeed;
				if(Y > PlayFieldY*TileHeight)
					Yi = -MoveSpeed;
				if(Y < PlayFieldY*TileHeight)
					Yi = MoveSpeed;
				IsMoving = true;
				Event_LeaveCurrentSpot();
			}
	}
}

bool CWorldPart::CanMoveTo(const int PlayFieldXin,const int PlayFieldYin) 
{
	return false;
}

void CWorldPart::Move()
{
	if (!FirstArriveEventFired)
	{
		Event_ArrivedOnNewSpot();
		FirstArriveEventFired=true;
	}
	if (IsMoving)
	{
		if (MoveDelayCounter == MoveDelay)
		{
			X += Xi;
			Y += Yi;
			Event_Moving(X,Y,Xi,Yi);
			if ((X == PlayFieldX * TileWidth) && (Y == PlayFieldY * TileHeight))
			{
				IsMoving = false;
				Xi = 0;
				Yi = 0;
				Event_ArrivedOnNewSpot();
			}
			MoveDelayCounter = -1;
		}
		MoveDelayCounter++;
	}
	else
		if (MoveQue.size() > 0)
		{
			if (CanMoveTo(MoveQue.back().X,MoveQue.back().Y))
			{
				MoveTo(MoveQue.back().X,MoveQue.back().Y,false);
				MoveQue.pop_back();
			}
			else
				MoveQue.clear();
		}
}

void CWorldPart::Draw(SDL_Surface* Surface)
{
	if(!BHide)
	{
	//printf("Start draw type:%d\n",Type);
	if (*Image)
	{
		Event_BeforeDraw();
		SDL_Rect SrcRect,DstRect;
		SrcRect.x = AnimPhase * TileWidth;
		SrcRect.y = 0;
		SrcRect.w = TileWidth;
		SrcRect.h = TileHeight;
		if(ParentList)
		{
			DstRect.x = X- ParentList->ViewPort->MinScreenX;
			DstRect.y = Y- ParentList->ViewPort->MinScreenY;
		}
		else
		{
			DstRect.x = X;
			DstRect.y = Y;
		}
		DstRect.w = TileWidth;
		DstRect.h = TileHeight;
		SDL_BlitSurface((*Image),&SrcRect,Surface,&DstRect);
	}
	if (Selected)
	{
		if (ParentList)
		{
			boxRGBA(Surface,X- ParentList->ViewPort->MinScreenX,Y- ParentList->ViewPort->MinScreenY,X- ParentList->ViewPort->MinScreenX+TileWidth-1,Y- ParentList->ViewPort->MinScreenY+TileHeight-1,0,0,200,15);
			rectangleRGBA(Surface,X- ParentList->ViewPort->MinScreenX,Y- ParentList->ViewPort->MinScreenY,X- ParentList->ViewPort->MinScreenX+TileWidth-1,Y- ParentList->ViewPort->MinScreenY+TileHeight-1,0,0,255,50);
		}
		else
		{
			boxRGBA(Surface,X,Y,X+TileWidth-1,Y+TileHeight-1,0,0,200,15);
			rectangleRGBA(Surface,X,Y,X+TileWidth-1,Y+TileHeight-1,0,0,255,50);
		}
	}
	}
	//printf("End draw type:%d\n",Type);
}

CWorldPart::~CWorldPart()
{

}

CEmpty::CEmpty(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
{
	Image = &IMGEmpty;
	Type = IDEmpty;
};

CWall::CWall(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
{
	Image = &IMGWall;
	Type = IDWall;
	Z = ZWall;
}

CFloor::CFloor(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
{
	Image = &IMGFloor;
	Type = IDFloor;
	Z = ZFloor;
}

CBomb::CBomb(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
{
	Image = &IMGBomb;
	Type = IDBomb;
	Z = ZBomb;
}

CDiamond::CDiamond(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
{
	Image = &IMGDiamond;
	Type = IDDiamond;
	Z = ZDiamond;
}

CExplosion::CExplosion(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
{
    if(GlobalSoundEnabled)
	{
        Mix_PlayChannel(-1,Sounds[SND_EXPLODE],0);
	}
	Image = &IMGExplosion;
	Type = IDExplosion;
	Z = ZExplosion;
	AnimPhase=0;
	AnimPhases=8;
	AnimCounter = 1;
	AnimDelay = 2;
	MoveDelay = 0;
	MoveSpeed = 4;
	AnimDelayCounter =0;
}


void CExplosion::Event_BeforeDraw()
{
	AnimDelayCounter++;
	if (AnimDelayCounter ==AnimDelay)
	{
		AnimDelayCounter = 0;
		AnimPhase++;
		if (AnimPhase == AnimPhases)
			Kill();
	}
}


CBox::CBox(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
{
	Image = &IMGBox;
	Type = IDBox;
	MoveDelay = 0;
	MoveSpeed = 2;
	Z = ZBox;
}



bool CBox::CanMoveTo(const int PlayFieldXin,const int PlayFieldYin)
{
	int Teller;
	bool Result = true;
	if ((PlayFieldXin >= 0) && (PlayFieldXin < NrOfCols) && (PlayFieldYin >= 0) && (PlayFieldYin < NrOfRows))
	{
		if (ParentList)
		{
			for (Teller=0;Teller<ParentList->ItemCount;Teller++)
				if((ParentList->Items[Teller]->GetType() == IDWall) || (ParentList->Items[Teller]->GetType() == IDBox) || (ParentList->Items[Teller]->GetType() == IDDiamond))
					if((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldXin) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldYin))
					{
						Result = false;
						break;
					}
		}
	}
	else
		Result = false;
	return Result;
}

void CBox::Event_ArrivedOnNewSpot()
{
	int Teller;
	AnimPhase = 0;
	//printf("Arrive Event fired\n");
	if (ParentList)
	{
		//printf("Parent List Set\n");
		for (Teller=0;Teller< ParentList->ItemCount;Teller++)
		{
           if((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldX) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldY) &&
              (ParentList->Items[Teller]->GetType() == IDBomb))
              {
                  ParentList->Add(new CExplosion(PlayFieldX,PlayFieldY));
                  ParentList->Items[Teller]->Kill();
                  Kill();
              }

		}
	}
}
