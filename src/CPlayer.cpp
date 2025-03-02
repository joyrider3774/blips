#include "CPlayer.h"
#include <SDL3_mixer/SDL_mixer.h>

CPlayer::CPlayer(const int PlayFieldXin,const int PlayFieldYin) : CWorldPart(PlayFieldXin,PlayFieldYin)
{
	Image=&IMGPlayer;
	AnimBase=4;
	AnimPhase=4;
	AnimPhases=4;
	AnimCounter = 0;
	AnimDelay = 12;
	MoveDelay = 0;
	MoveSpeed = 2;
	AnimDelayCounter =0;
	Type = IDPlayer;
	Z = ZPlayer;
	IsDeath=false;
}


bool CPlayer::CanMoveTo(const int PlayFieldXin,const int PlayFieldYin)
{
	int Teller;
	bool Result = true;
	if ((PlayFieldXin >= 0) && (PlayFieldXin < NrOfCols) && (PlayFieldYin >= 0) && (PlayFieldYin < NrOfRows))
	{
		if (ParentList)
		{
			for (Teller=0;Teller<ParentList->ItemCount;Teller++)
				if((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldXin) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldYin))
				{
					if((ParentList->Items[Teller]->GetType() == IDWall))
					{
						Result = false;
						break;
					}
					if((ParentList->Items[Teller]->GetType() == IDBox))
					{
						if (PlayFieldX > PlayFieldXin)
						{
							Result = ParentList->Items[Teller]->CanMoveTo(PlayFieldXin-1,PlayFieldYin);
						}
						if (PlayFieldX < PlayFieldXin)
						{
							Result = ParentList->Items[Teller]->CanMoveTo(PlayFieldXin+1,PlayFieldYin);
						}
						if (PlayFieldY > PlayFieldYin)
						{
							Result = ParentList->Items[Teller]->CanMoveTo(PlayFieldXin,PlayFieldYin-1);
						}
						if (PlayFieldY < PlayFieldYin)
						{
							Result = ParentList->Items[Teller]->CanMoveTo(PlayFieldXin,PlayFieldYin+1);
						}
						break;
					}

				}
		}
	}
	else
		Result = false;
	return Result;
}

void CPlayer::Event_ArrivedOnNewSpot()
{
    int Teller=0;
    if (ParentList)
    {
        for (Teller=0;Teller<ParentList->ItemCount;Teller++)
            if((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldX) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldY))
            {
                if((ParentList->Items[Teller]->GetType() == IDBomb))
                {
                    ParentList->Add(new CExplosion(PlayFieldX,PlayFieldY));
                    ParentList->Items[Teller]->Kill();
                    IsDeath = true;
                    Hide();

                    break;
                }

                if((ParentList->Items[Teller]->GetType() == IDDiamond))
                {
                    ParentList->Items[Teller]->Kill();
                    if(GlobalSoundEnabled)
                        Mix_PlayChannel(-1,Sounds[SND_COLLECT],0);
                }

            }
    }
}

void CPlayer::Event_BeforeDraw()
{
	if (IsMoving)
	{
		AnimPhase = AnimBase + AnimCounter;
		AnimDelayCounter++;
		if (AnimDelayCounter ==AnimDelay)
		{
			AnimDelayCounter = 0;
			AnimCounter++;
			if (AnimCounter == AnimPhases)
				AnimCounter = 0;
		}
	}
/*	else
		AnimPhase = AnimBase;*/
}

void CPlayer::Event_Moving(int ScreenPosX,int ScreenPosY,int ScreenXi,int ScreenYi)
{
    if((ScreenPosX > (ParentList->ViewPort->MaxScreenX) - ORIG_WINDOW_WIDTH/2) && (Xi > 0))
        ParentList->ViewPort->Move(Xi,Yi);
    if((ScreenPosX < (ParentList->ViewPort->MaxScreenX) - ORIG_WINDOW_WIDTH/2) && (Xi < 0))
        ParentList->ViewPort->Move(Xi,Yi);
    if((ScreenPosY > (ParentList->ViewPort->MaxScreenY) - ORIG_WINDOW_HEIGHT/2) && (Yi > 0))
        ParentList->ViewPort->Move(Xi,Yi);
    if((ScreenPosY < (ParentList->ViewPort->MaxScreenY) - ORIG_WINDOW_HEIGHT/2) && (Yi < 0))
        ParentList->ViewPort->Move(Xi,Yi);

}


void CPlayer::MoveTo(const int PlayFieldXin,const int PlayFieldYin,bool BackWards)
{
 	int Teller;
 	if(!IsMoving)
	{
		if(this->CanMoveTo(PlayFieldXin,PlayFieldYin) || BackWards)
		{
			PlayFieldX = PlayFieldXin;
			PlayFieldY = PlayFieldYin;
			if(X < PlayFieldX*TileWidth)
			{
				Xi = MoveSpeed;
				if (ParentList)
				{
					for(Teller=0;Teller<ParentList->ItemCount;Teller++)
					{
						if(((ParentList->Items[Teller]->GetType() == IDBox) || (ParentList->Items[Teller]->GetType() == IDWall)) && ((ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldX) && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldY)))
						{
							ParentList->Items[Teller]->MoveTo(PlayFieldX+1,PlayFieldY,false);
							break;
						}
					}
				}
				AnimBase = 4;

			}
			if(X > PlayFieldX*TileWidth)
			{
				Xi = -MoveSpeed;
				if (ParentList)
				{
					for(Teller=0;Teller<ParentList->ItemCount;Teller++)
					{
						if(((ParentList->Items[Teller]->GetType() == IDBox) || (ParentList->Items[Teller]->GetType() == IDWall)) && ((PlayFieldX == ParentList->Items[Teller]->GetPlayFieldX() )  && (ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldY)))
						{
							ParentList->Items[Teller]->MoveTo(PlayFieldX-1,PlayFieldY,false);
							break;
						}
					}
				}
				AnimBase = 0;
			}
			if(Y > PlayFieldY*TileHeight)
			{
				Yi = -MoveSpeed;
				if (ParentList)
				{
					for(Teller=0;Teller<ParentList->ItemCount;Teller++)
					{
						if(((ParentList->Items[Teller]->GetType() == IDBox) || (ParentList->Items[Teller]->GetType() == IDWall)) && ((PlayFieldY == ParentList->Items[Teller]->GetPlayFieldY())  && (ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldX)))
						{
							ParentList->Items[Teller]->MoveTo(PlayFieldX,PlayFieldY-1,false);
							break;
						}
					}
				}
				AnimBase = 8;
			}
			if(Y < PlayFieldY*TileHeight)
			{
				Yi = MoveSpeed;
				if(ParentList)
				{
					for(Teller=0;Teller<ParentList->ItemCount;Teller++)
					{
						if(((ParentList->Items[Teller]->GetType() == IDBox) || (ParentList->Items[Teller]->GetType() == IDWall)) && ((ParentList->Items[Teller]->GetPlayFieldY() == PlayFieldY)  && (ParentList->Items[Teller]->GetPlayFieldX() == PlayFieldX )))
						{
							ParentList->Items[Teller]->MoveTo(PlayFieldX,PlayFieldY+1,false);
							break;
						}
					}
				}
				AnimBase = 12;
			}
			IsMoving = true;
		}
		else
		{
			if (PlayFieldXin > PlayFieldX)
			{
				AnimBase= 4;
			}
			if (PlayFieldXin < PlayFieldX)
			{
				AnimBase = 0;
			}
			if (PlayFieldYin > PlayFieldY)
			{
				AnimBase = 12;
			}
			if (PlayFieldYin < PlayFieldY)
			{
				AnimBase = 8;
			}
			AnimPhase = AnimBase + AnimCounter;
			AnimDelayCounter++;
			if (AnimDelayCounter ==AnimDelay)
			{
				AnimDelayCounter = 0;
				AnimCounter++;
				if (AnimCounter == AnimPhases)
					AnimCounter = 0;
			}
		}

 	}
 }
