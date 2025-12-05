#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
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
	IsDeath = false;
}


void CWorldPart::Hide() 
{
	BHide = true;
}

bool CWorldPart::NeedHide() 
{
	return BHide;
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
	}
}

void CWorldPart::MoveTo(const int PlayFieldXin,const int PlayFieldYin,bool BackWards)
{
	if(!IsMoving &!NeedToKill() && !NeedHide())
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
	// if (!FirstArriveEventFired)
	// {
	// 	Event_ArrivedOnNewSpot();
	// 	FirstArriveEventFired=true;
	// }
	if(IsMoving &!NeedToKill() && !NeedHide())
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

void CWorldPart::Draw()
{
	if(!BHide)
	{
		if (*Image)
		{
			Event_BeforeDraw();
			SDL_FRect SrcRect,DstRect;
			SrcRect.x = (float)(AnimPhase * TileWidth);
			SrcRect.y = 0.0f;
			SrcRect.w = (float)TileWidth;
			SrcRect.h = (float)TileHeight;
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
			SDL_RenderTexture(Renderer, (*Image),&SrcRect,&DstRect);
		}
		if (Selected)
		{
			if (ParentList)
			{
				SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
				SDL_SetRenderDrawColor(Renderer,0,0,200,50);
				SDL_FRect Rect;
				Rect.x = (float)(X- ParentList->ViewPort->MinScreenX);
				Rect.y = (float)(Y- ParentList->ViewPort->MinScreenY);
				Rect.w = (float)TileWidth;
				Rect.h = (float)TileHeight;
				SDL_RenderFillRect(Renderer, &Rect);
				SDL_SetRenderDrawColor(Renderer,0,0,255,50);
				SDL_RenderRect(Renderer, &Rect);
				SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_NONE);
			}
			else
			{
				SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
				SDL_SetRenderDrawColor(Renderer,0,0,200,50);
				SDL_FRect Rect;
				Rect.x = (float)X;
				Rect.y = (float)Y;
				Rect.w = (float)TileWidth;
				Rect.h = (float)TileHeight;
				SDL_RenderFillRect(Renderer, &Rect);
				SDL_SetRenderDrawColor(Renderer,0,0,255,50);
				SDL_RenderRect(Renderer, &Rect);
				SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_NONE);
			}
		}
	}
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

CWallBreakable::CWallBreakable(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
{
	Image = &IMGWall;
	AnimPhase = 1;
	Type = IDWallBreakable;
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
			{
				if (ParentList->Items[Teller]->NeedToKill() || ParentList->Items[Teller]->NeedHide())
					continue;
				if((ParentList->Items[Teller]->GetType() == IDWall) || (ParentList->Items[Teller]->GetType() == IDWallBreakable) || 
					(ParentList->Items[Teller]->GetType() == IDBox) || (ParentList->Items[Teller]->GetType() == IDDiamond) ||
					(ParentList->Items[Teller]->GetType() == IDPlayer) || (ParentList->Items[Teller]->GetType() == IDPlayer2) ||
					(ParentList->Items[Teller]->GetType() == IDBox1) || (ParentList->Items[Teller]->GetType() == IDBox2) ||
					(ParentList->Items[Teller]->GetType() == IDBoxWall))
					if((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldXin) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldYin))
					{
						Result = false;
						break;
					}
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
	if (ParentList)
	{
		for (Teller=0;Teller< ParentList->ItemCount;Teller++)
		{
			if (ParentList->Items[Teller]->NeedToKill() || ParentList->Items[Teller]->NeedHide())
				continue;
           	if ((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldX) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldY))
		   	{
            	if (ParentList->Items[Teller]->GetType() == IDBomb)
              	{                  
                	ParentList->Items[Teller]->Kill();
                	Kill();
					ParentList->Add(new CExplosion(PlayFieldX,PlayFieldY));
        		}

				if (ParentList->Items[Teller]->GetType() == IDBoxBomb)
				{
					//kill needs to come first, Add sorts the lists skewing indexing					
					ParentList->Items[Teller]->Kill();
					Kill();
					ParentList->Add(new CExplosion(PlayFieldX,PlayFieldY));
					break;
				}
			}
		}
	}
}

CBox1::CBox1(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
{
	Image = &IMGBox;
	Type = IDBox1;
	MoveDelay = 0;
	MoveSpeed = 2;
	AnimPhase = 2;
	Z = ZBox;
}

bool CBox1::CanMoveTo(const int PlayFieldXin,const int PlayFieldYin)
{
	int Teller;
	bool Result = true;
	if ((PlayFieldXin >= 0) && (PlayFieldXin < NrOfCols) && (PlayFieldYin >= 0) && (PlayFieldYin < NrOfRows))
	{
		if (ParentList)
		{
			for (Teller=0;Teller<ParentList->ItemCount;Teller++)
			{
				if (ParentList->Items[Teller]->NeedToKill() || ParentList->Items[Teller]->NeedHide())
					continue;
				if((ParentList->Items[Teller]->GetType() == IDWall) || (ParentList->Items[Teller]->GetType() == IDWallBreakable) || 
					(ParentList->Items[Teller]->GetType() == IDBox) || (ParentList->Items[Teller]->GetType() == IDDiamond) ||
					(ParentList->Items[Teller]->GetType() == IDPlayer) || (ParentList->Items[Teller]->GetType() == IDPlayer2) ||
					(ParentList->Items[Teller]->GetType() == IDBox1) || (ParentList->Items[Teller]->GetType() == IDBox2) ||
					(ParentList->Items[Teller]->GetType() == IDBoxWall))
					if((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldXin) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldYin))
					{
						Result = false;
						break;
					}
			}
		}
	}
	else
		Result = false;
	return Result;
}

void CBox1::Event_ArrivedOnNewSpot()
{
	int Teller;
	AnimPhase = 2;
	if (ParentList)
	{
		for (Teller=0;Teller< ParentList->ItemCount;Teller++)
		{
			if (ParentList->Items[Teller]->NeedToKill() || ParentList->Items[Teller]->NeedHide())
				continue;
        	
			if((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldX) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldY))
		    {
            	if (ParentList->Items[Teller]->GetType() == IDBomb)
              	{                	
                	ParentList->Items[Teller]->Kill();
                	Kill();
					ParentList->Add(new CExplosion(PlayFieldX,PlayFieldY));
              	}

				if (ParentList->Items[Teller]->GetType() == IDBoxBomb)
				{
					//kill needs to come first, Add sorts the lists skewing indexing					
					ParentList->Items[Teller]->Kill();
					Kill();
					ParentList->Add(new CExplosion(PlayFieldX,PlayFieldY));
					break;
				}
			}
		}
	}
}

CBox2::CBox2(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
{
	Image = &IMGBox;
	Type = IDBox2;
	MoveDelay = 0;
	MoveSpeed = 2;
	AnimPhase = 3;
	Z = ZBox;
}

bool CBox2::CanMoveTo(const int PlayFieldXin,const int PlayFieldYin)
{
	int Teller;
	bool Result = true;
	if ((PlayFieldXin >= 0) && (PlayFieldXin < NrOfCols) && (PlayFieldYin >= 0) && (PlayFieldYin < NrOfRows))
	{
		if (ParentList)
		{
			for (Teller=0;Teller<ParentList->ItemCount;Teller++)
			{
				if (ParentList->Items[Teller]->NeedToKill() || ParentList->Items[Teller]->NeedHide())
					continue;
				
				if((ParentList->Items[Teller]->GetType() == IDWall) || (ParentList->Items[Teller]->GetType() == IDWallBreakable) || 
					(ParentList->Items[Teller]->GetType() == IDBox) || (ParentList->Items[Teller]->GetType() == IDDiamond) ||
					(ParentList->Items[Teller]->GetType() == IDPlayer) || (ParentList->Items[Teller]->GetType() == IDPlayer2) ||
					(ParentList->Items[Teller]->GetType() == IDBox1) || (ParentList->Items[Teller]->GetType() == IDBox2) ||
					(ParentList->Items[Teller]->GetType() == IDBoxWall))
					if((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldXin) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldYin))
					{
						Result = false;
						break;
					}
			}
		}
	}
	else
		Result = false;
	return Result;
}

void CBox2::Event_ArrivedOnNewSpot()
{
	int Teller;
	AnimPhase = 3;
	if (ParentList)
	{
		for (Teller=0;Teller< ParentList->ItemCount;Teller++)
		{
			if (ParentList->Items[Teller]->NeedToKill() || ParentList->Items[Teller]->NeedHide())
				continue;

        	if((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldX) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldY))
			{
              	if (ParentList->Items[Teller]->GetType() == IDBomb)
              	{                	
                	ParentList->Items[Teller]->Kill();
                	Kill();
					ParentList->Add(new CExplosion(PlayFieldX,PlayFieldY));
              	}
				
				if (ParentList->Items[Teller]->GetType() == IDBoxBomb)
				{
					//kill needs to come first, Add sorts the lists skewing indexing					
					ParentList->Items[Teller]->Kill();
					Kill();
					ParentList->Add(new CExplosion(PlayFieldX,PlayFieldY));
					break;
				}
			}

		}
	}
}

CBoxWall::CBoxWall(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
{
	Image = &IMGBox;
	Type = IDBoxWall;
	MoveDelay = 0;
	MoveSpeed = 2;
	AnimPhase = 1;
	Z = ZBox;
}

bool CBoxWall::CanMoveTo(const int PlayFieldXin,const int PlayFieldYin)
{
	int Teller;
	bool Result = true;
	if ((PlayFieldXin >= 0) && (PlayFieldXin < NrOfCols) && (PlayFieldYin >= 0) && (PlayFieldYin < NrOfRows))
	{
		if (ParentList)
		{
			for (Teller=0;Teller<ParentList->ItemCount;Teller++)
			{
				if (ParentList->Items[Teller]->NeedToKill() || ParentList->Items[Teller]->NeedHide())
					continue;
				
				if((ParentList->Items[Teller]->GetType() == IDWall) || (ParentList->Items[Teller]->GetType() == IDWallBreakable) || 
					(ParentList->Items[Teller]->GetType() == IDBox) || (ParentList->Items[Teller]->GetType() == IDDiamond) ||
					(ParentList->Items[Teller]->GetType() == IDPlayer) || (ParentList->Items[Teller]->GetType() == IDPlayer2) ||
					(ParentList->Items[Teller]->GetType() == IDBox1) || (ParentList->Items[Teller]->GetType() == IDBox2))
					if((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldXin) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldYin))
					{
						Result = false;
						break;
					}
			}
		}
	}
	else
		Result = false;
	return Result;
}

void CBoxWall::Event_ArrivedOnNewSpot()
{
	int Teller;
	AnimPhase = 1;
	if (ParentList)
	{
		for (Teller=0;Teller< ParentList->ItemCount;Teller++)
		{
			if (ParentList->Items[Teller]->NeedToKill() || ParentList->Items[Teller]->NeedHide())
				continue;
        	
			if ((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldX) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldY))
			{
            	if (ParentList->Items[Teller]->GetType() == IDBomb)
              	{                	
                  	ParentList->Items[Teller]->Kill();
                  	Kill();
					ParentList->Add(new CExplosion(PlayFieldX,PlayFieldY));
              	}

			 	if ((ParentList->Items[Teller]->GetType() == IDBoxWall) && (ParentList->Items[Teller] != this))
				{
					//kill needs to come first, Add sorts the lists skewing indexing
					ParentList->Items[Teller]->Kill();
					Kill();
					ParentList->Add(new CExplosion(PlayFieldX,PlayFieldY));
					ParentList->Add(new CWall(PlayFieldX,PlayFieldY));		
					break;
				}

				if ((ParentList->Items[Teller]->GetType() == IDBoxBomb) && (this != ParentList->Items[Teller]))
				{
					//kill needs to come first, Add sorts the lists skewing indexing					
					ParentList->Items[Teller]->Kill();
					Kill();
					ParentList->Add(new CExplosion(PlayFieldX,PlayFieldY));
					break;
				}
			}
		}
	}
}

CBoxBomb::CBoxBomb(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
{
	Image = &IMGBox;
	Type = IDBoxBomb;
	MoveDelay = 0;
	MoveSpeed = 2;
	AnimPhase = 4;
	Z = ZBox;
}

bool CBoxBomb::CanMoveTo(const int PlayFieldXin,const int PlayFieldYin)
{
	int Teller;
	bool Result = true;
	if ((PlayFieldXin >= 0) && (PlayFieldXin < NrOfCols) && (PlayFieldYin >= 0) && (PlayFieldYin < NrOfRows))
	{
		if (ParentList)
		{
			for (Teller=0;Teller<ParentList->ItemCount;Teller++)
			{
				if (ParentList->Items[Teller]->NeedToKill() || ParentList->Items[Teller]->NeedHide())
					continue;
				
				if((ParentList->Items[Teller]->GetType() == IDWall) || 
					(ParentList->Items[Teller]->GetType() == IDDiamond))
					if((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldXin) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldYin))
					{
						Result = false;
						break;
					}
			}
		}
	}
	else
		Result = false;
	return Result;
}

void CBoxBomb::Event_ArrivedOnNewSpot()
{
	int Teller;
	AnimPhase = 4;
	if (ParentList)
	{
		for (Teller=0;Teller< ParentList->ItemCount;Teller++)
		{
			if (ParentList->Items[Teller]->NeedToKill() || ParentList->Items[Teller]->NeedHide())
				continue;
        	
			if((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldX) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldY))
			{
             	if (ParentList->Items[Teller]->GetType() == IDBomb)
              	{
                  	ParentList->Items[Teller]->Kill();
                  	Kill();
					ParentList->Add(new CExplosion(PlayFieldX,PlayFieldY));
              	}

				if ((ParentList->Items[Teller]->GetType() != IDWall) && (ParentList->Items[Teller]->GetType() != IDDiamond) && 
					(ParentList->Items[Teller]->GetType() != IDFloor) && (ParentList->Items[Teller] != this))
				{
					///kill needs to come first, Add sorts the lists skewing indexing
					if ((ParentList->Items[Teller]->GetType() == IDPlayer) || (ParentList->Items[Teller]->GetType() == IDPlayer2))
					{
						ParentList->Items[Teller]->IsDeath = true;
						ParentList->Items[Teller]->Hide();
					}
					else
						ParentList->Items[Teller]->Kill();
					Kill();
					ParentList->Add(new CExplosion(PlayFieldX,PlayFieldY));										
					break;
				}

				if ((ParentList->Items[Teller]->GetType() == IDBoxBomb) && (this != ParentList->Items[Teller]))
				{
					//kill needs to come first, Add sorts the lists skewing indexing					
					ParentList->Items[Teller]->Kill();
					Kill();
					ParentList->Add(new CExplosion(PlayFieldX,PlayFieldY));
					break;
				}
			}
		}
	}
}

