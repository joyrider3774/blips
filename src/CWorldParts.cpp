#include <SDL.h>
#include "CWorldParts.h"
#include "CPlayer.h"

CWorldParts::CWorldParts()
{
	ItemCount = 0;
	DisableSorting = false;
	ViewPort = new CViewPort(0,0,19,11,0,0,NrOfCols-1,NrOfRows-1);
}

void CWorldParts::CenterVPOnPlayer()
{
    int Teller=0,PlayerX=-1,PlayerY=-1;
    for (Teller=0;Teller<ItemCount;Teller++)
        if(Items[Teller]->GetType() == IDPlayer)
        {

            PlayerX = Items[Teller]->GetPlayFieldX();
            PlayerY = Items[Teller]->GetPlayFieldY();
            break;
        }
    ViewPort->SetViewPort(PlayerX-10, PlayerY-6,PlayerX+10, PlayerY+6);
}

void CWorldParts::LimitVPLevel()
{
    int MinX = NrOfCols, MinY = NrOfRows, MaxX = -1, MaxY = -1, Teller = 0;
	for (Teller = 0;Teller<ItemCount;Teller++)
	{
		if (Items[Teller]->GetPlayFieldX() < MinX)
			MinX = Items[Teller]->GetPlayFieldX();
		if (Items[Teller]->GetPlayFieldY() < MinY)
			MinY = Items[Teller]->GetPlayFieldY();
		if (Items[Teller]->GetPlayFieldX() > MaxX)
			MaxX = Items[Teller]->GetPlayFieldX();
		if (Items[Teller]->GetPlayFieldY() > MaxY)
            MaxY = Items[Teller]->GetPlayFieldY();
    }
    ViewPort->SetVPLimit(MinX,MinY,MaxX,MaxY);
    CenterVPOnPlayer();
}

void CWorldParts::RemoveAll()
{
	int Teller;
	for (Teller=0;Teller<ItemCount;Teller++)
	{
		delete Items[Teller];
		Items[Teller] = NULL;
	}
	ItemCount=0;
}
void CWorldParts::Remove(int PlayFieldXin,int PlayFieldYin)
{
	int Teller1,Teller2;
	for (Teller1=0;Teller1<ItemCount;Teller1++)
	{
		if ((Items[Teller1]->GetPlayFieldX() == PlayFieldXin) && (Items[Teller1]->GetPlayFieldY() == PlayFieldYin))
		{
			delete Items[Teller1];
			for (Teller2=Teller1;Teller2<ItemCount-1;Teller2++)
				Items[Teller2] = Items[Teller2+1];
			ItemCount--;
			Teller1--;
		}
	}
}

void CWorldParts::Remove(int PlayFieldXin,int PlayFieldYin,int Type)
{
	int Teller1,Teller2;
	for (Teller1=0;Teller1<ItemCount;Teller1++)
	{
		if ((Items[Teller1]->GetPlayFieldX() == PlayFieldXin) && (Items[Teller1]->GetPlayFieldY() == PlayFieldYin) && (Items[Teller1]->GetType() == Type))
		{
			delete Items[Teller1];
			for (Teller2=Teller1;Teller2<ItemCount-1;Teller2++)
				Items[Teller2] = Items[Teller2+1];
			ItemCount--;
			Teller1--;
		}
	}
}

void CWorldParts::Add(CWorldPart *WorldPart)
{
	if( ItemCount < NrOfRows*NrOfCols*3 )
	{
		WorldPart->ParentList = this;
		Items[ItemCount] = WorldPart;
		ItemCount++;
		Sort();
	}
}


void CWorldParts::Sort()
{
	int Teller1,Teller2,Index;
	CWorldPart *Part;
	if (!DisableSorting)
	{
		for (Teller1 = 1; Teller1 <ItemCount;Teller1++)
		{
			Index = Items[Teller1]->GetZ();
			Part = Items[Teller1];
			Teller2 = Teller1;
			while ((Teller2 > 0) && (Items[Teller2-1]->GetZ() > Index))
			{
				Items[Teller2] = Items[Teller2 - 1];
				Teller2--;
			}
			Items[Teller2] = Part;
		}
	}

}

void CWorldParts::Save(char *Filename)
{
	FILE *Fp;
	int Teller,BufferPosition=0;
	char *Buffer;
	Buffer = new char[3*ItemCount];
	for (Teller=0;Teller<ItemCount;Teller++)
	{
		Buffer[BufferPosition]= (char) Items[Teller]->GetType();
		Buffer[BufferPosition+1] = (char)Items[Teller]->GetPlayFieldX();
		Buffer[BufferPosition+2]= (char)Items[Teller]->GetPlayFieldY();
		BufferPosition += 3;
	}
	Fp = fopen(Filename,"wb");
	if (Fp)
	{
		fwrite(Buffer,1,3*ItemCount,Fp);
		fclose(Fp);
	}
	delete[] Buffer;
}

void CWorldParts::Load(char *Filename)
{
	int X,Y,Type;
	FILE *Fp;
	int BufferPosition=0;
	long FileSize;
	char *Buffer;
	Fp = fopen(Filename,"rb");
	if(Fp)
	{
		RemoveAll();
		DisableSorting=true;
		fseek (Fp , 0 , SEEK_END);
  		FileSize = ftell (Fp);
  		rewind (Fp);
		Buffer = new char[FileSize];
		fread(Buffer,1,FileSize,Fp);
		while(BufferPosition < FileSize)
		{
			Type = (int)Buffer[BufferPosition];
			X =(int)Buffer[BufferPosition+1];
			Y = (int)Buffer[BufferPosition+2];
			BufferPosition +=3;
			switch(Type)
			{
                case IDEmpty:
                    Add( new CEmpty(X,Y));
                    break;
                case IDBox:
                     Add(new CBox(X,Y));
                    break;
                case IDPlayer:
                     Add(new CPlayer(X,Y));
                    break;
                case IDFloor:
                     Add(new CFloor(X,Y));
                    break;
                case IDWall:
                     Add( new CWall(X,Y));
                    break;
                case IDBomb:
                     Add(new CBomb(X,Y));
                    break;
                case IDDiamond:
                     Add( new CDiamond(X,Y));
                    break;

			}
		}
		delete[] Buffer;
		fclose(Fp);
		DisableSorting=false;
		Sort();
		LimitVPLevel();
		CenterVPOnPlayer();
	}

}

void CWorldParts::Move()
{
	int Teller;
	for (Teller=0;Teller<ItemCount;Teller++)
	{
		Items[Teller]->Move();
	}
}

void CWorldParts::Draw(SDL_Surface *Surface)
{
	int Teller;
	for (Teller=0;Teller<ItemCount;Teller++)
	{
	    if(Items[Teller]->NeedToKill())
	     {
	         Remove(Items[Teller]->GetPlayFieldX(),Items[Teller]->GetPlayFieldY(),Items[Teller]->GetType());
	         //go back one item to prevent skips & segaults
	         Teller--;
	     }
	     else
            if((Items[Teller]->GetPlayFieldX() >= ViewPort->VPMinX) && (Items[Teller]->GetPlayFieldX()-1 <= ViewPort->VPMaxX) &&
                (Items[Teller]->GetPlayFieldY() >= ViewPort->VPMinY) && (Items[Teller]->GetPlayFieldY()-1 <= ViewPort->VPMaxY))
                {
                    Items[Teller]->Draw(Surface);
                }


	}
}

CWorldParts::~CWorldParts()
{
    delete ViewPort;
	int Teller;
	for (Teller=0;Teller<ItemCount;Teller++)
	{
		delete Items[Teller];
		Items[Teller] = NULL;
	}
}
