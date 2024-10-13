#include <SDL.h>
#include "CWorldParts.h"
#include "CWorldPart.h"
#include "stdbool.h"

CWorldParts* CWorldParts_Create()
{
	CWorldParts* Result = (CWorldParts*) malloc(sizeof(CWorldParts));

	Result->ItemCount = 0;
	Result->DisableSorting = false;
	Result->ViewPort = CViewPort_Create(0,0,19,11,0,0,NrOfCols-1,NrOfRows-1);
	return Result;
}

void CWorldParts_CenterVPOnPlayer(CWorldParts* WorldParts)
{
    int Teller=0,PlayerX=-1,PlayerY=-1;
    for (Teller=0;Teller<WorldParts->ItemCount;Teller++)
        if(WorldParts->Items[Teller]->Type == IDPlayer)
        {

            PlayerX = WorldParts->Items[Teller]->PlayFieldX;
            PlayerY = WorldParts->Items[Teller]->PlayFieldY;
            break;
        }
    CViewPort_SetViewPort(WorldParts->ViewPort, PlayerX-10, PlayerY-6,PlayerX+10, PlayerY+6);
}

void CWorldParts_LimitVPLevel(CWorldParts* WorldParts)
{
    int MinX = NrOfCols, MinY = NrOfRows, MaxX = -1, MaxY = -1, Teller = 0;
	for (Teller = 0;Teller<WorldParts->ItemCount;Teller++)
	{
		if (WorldParts->Items[Teller]->PlayFieldX < MinX)
			MinX = WorldParts->Items[Teller]->PlayFieldX;
		if (WorldParts->Items[Teller]->PlayFieldY < MinY)
			MinY = WorldParts->Items[Teller]->PlayFieldY;
		if (WorldParts->Items[Teller]->PlayFieldX > MaxX)
			MaxX = WorldParts->Items[Teller]->PlayFieldX;
		if (WorldParts->Items[Teller]->PlayFieldY > MaxY)
            MaxY = WorldParts->Items[Teller]->PlayFieldY;
    }
    CViewPort_SetVPLimit(WorldParts->ViewPort, MinX,MinY,MaxX,MaxY);
    CWorldParts_CenterVPOnPlayer(WorldParts);
}

void CWorldParts_RemoveAll(CWorldParts* WorldParts)
{
	int Teller;
	for (Teller=0;Teller<WorldParts->ItemCount;Teller++)
	{
		CWorldPart_Destroy(WorldParts->Items[Teller]);
		WorldParts->Items[Teller] = NULL;
	}
	WorldParts->ItemCount=0;
}
void CWorldParts_Remove(CWorldParts* WorldParts, int PlayFieldXin,int PlayFieldYin)
{
	int Teller1,Teller2;
	for (Teller1=0;Teller1<WorldParts->ItemCount;Teller1++)
	{
		if ((WorldParts->Items[Teller1]->PlayFieldX == PlayFieldXin) && (WorldParts->Items[Teller1]->PlayFieldY == PlayFieldYin))
		{
			CWorldPart_Destroy(WorldParts->Items[Teller1]);
			for (Teller2=Teller1;Teller2<WorldParts->ItemCount-1;Teller2++)
				WorldParts->Items[Teller2] = WorldParts->Items[Teller2+1];
			WorldParts->ItemCount--;
			Teller1--;
		}
	}
}

void CWorldParts_Remove_Type(CWorldParts* WorldParts, int PlayFieldXin,int PlayFieldYin,int Type)
{
	int Teller1,Teller2;
	for (Teller1=0;Teller1<WorldParts->ItemCount;Teller1++)
	{
		if ((WorldParts->Items[Teller1]->PlayFieldX == PlayFieldXin) && (WorldParts->Items[Teller1]->PlayFieldY == PlayFieldYin) && (WorldParts->Items[Teller1]->Type == Type))
		{
			CWorldPart_Destroy(WorldParts->Items[Teller1]);
			for (Teller2=Teller1;Teller2<WorldParts->ItemCount-1;Teller2++)
				WorldParts->Items[Teller2] = WorldParts->Items[Teller2+1];
			WorldParts->ItemCount--;
			Teller1--;
		}
	}
}

void CWorldParts_Add(CWorldParts* WorldParts, CWorldPart *WorldPart)
{
	if( WorldParts->ItemCount < NrOfRows*NrOfCols*2 )
	{
		WorldPart->ParentList = WorldParts;
		WorldParts->Items[WorldParts->ItemCount] = WorldPart;
		WorldParts->ItemCount++;
		CWorldParts_Sort(WorldParts);
	}
}


void CWorldParts_Sort(CWorldParts* WorldParts)
{
	int Teller1,Teller2,Index;
	CWorldPart *Part;
	if (!WorldParts->DisableSorting)
	{
		for (Teller1 = 1; Teller1 <WorldParts->ItemCount;Teller1++)
		{
			Index = WorldParts->Items[Teller1]->Z;
			Part = WorldParts->Items[Teller1];
			Teller2 = Teller1;
			while ((Teller2 > 0) && (WorldParts->Items[Teller2-1]->Z > Index))
			{
				WorldParts->Items[Teller2] = WorldParts->Items[Teller2 - 1];
				Teller2--;
			}
			WorldParts->Items[Teller2] = Part;
		}
	}

}

void CWorldParts_Save(CWorldParts* WorldParts, char *Filename)
{
	FILE *Fp;
	int Teller,BufferPosition=0;
	char *Buffer = (char*) malloc(sizeof(char)* 3*WorldParts->ItemCount);
	for (Teller=0;Teller<WorldParts->ItemCount;Teller++)
	{
		Buffer[BufferPosition]= (char) WorldParts->Items[Teller]->Type;
		Buffer[BufferPosition+1] = (char)WorldParts->Items[Teller]->PlayFieldX;
		Buffer[BufferPosition+2]= (char)WorldParts->Items[Teller]->PlayFieldY;
		BufferPosition += 3;
	}
	Fp = fopen(Filename,"wb");
	if (Fp)
	{
		fwrite(Buffer,1,3*WorldParts->ItemCount,Fp);
		fclose(Fp);
	}
	free(Buffer);
}

void CWorldParts_Load(CWorldParts* WorldParts, char *Filename)
{
	int X,Y,Type;
	FILE *Fp;
	int BufferPosition=0;
	long FileSize;
	char *Buffer;
	Fp = fopen(Filename,"rb");
	if(Fp)
	{
		CWorldParts_RemoveAll(WorldParts);
		WorldParts->DisableSorting=true;
		fseek (Fp , 0 , SEEK_END);
  		FileSize = ftell (Fp);
  		rewind (Fp);
		char *Buffer = (char*) malloc(sizeof(char)* FileSize);
		fread(Buffer,1,FileSize,Fp);
		while(BufferPosition < FileSize)
		{
			Type = (int)Buffer[BufferPosition];
			X =(int)Buffer[BufferPosition+1];
			Y = (int)Buffer[BufferPosition+2];
			BufferPosition +=3;
			CWorldParts_Add(WorldParts, CWorldPart_Create(X,Y,Type));
		}
		free(Buffer);
		fclose(Fp);
		WorldParts->DisableSorting=false;
		CWorldParts_Sort(WorldParts);
		CWorldParts_LimitVPLevel(WorldParts);
		CWorldParts_CenterVPOnPlayer(WorldParts);
	}

}

void CWorldParts_Move(CWorldParts* WorldParts)
{
	int Teller;
	for (Teller=0;Teller<WorldParts->ItemCount;Teller++)
	{
		printf("%d %d\n", Teller, WorldParts->ItemCount);
		CWorldPart_Move(WorldParts->Items[Teller]);
	}
}

void CWorldParts_Draw(CWorldParts* WorldParts, SDL_Surface *Surface)
{
	int Teller;
	for (Teller=0;Teller<WorldParts->ItemCount;Teller++)
	{
	    if(WorldParts->Items[Teller]->PNeedToKill)
	     {
	         CWorldParts_Remove_Type(WorldParts, WorldParts->Items[Teller]->PlayFieldX,WorldParts->Items[Teller]->PlayFieldY,WorldParts->Items[Teller]->Type);
	         //go back one item to prevent skips & segaults
	         Teller--;
	     }
	     else
            if((WorldParts->Items[Teller]->PlayFieldX >= WorldParts->ViewPort->VPMinX) && (WorldParts->Items[Teller]->PlayFieldX-1 <= WorldParts->ViewPort->VPMaxX) &&
                (WorldParts->Items[Teller]->PlayFieldY >= WorldParts->ViewPort->VPMinY) && (WorldParts->Items[Teller]->PlayFieldY-1 <= WorldParts->ViewPort->VPMaxY))
                {
                    CWorldPart_Draw(WorldParts->Items[Teller], Surface);
				}
	}
}

void CWorldParts_Destroy(CWorldParts* WorldParts)
{
    CViewPort_Destroy(WorldParts->ViewPort);
	int Teller;
	for (Teller=0;Teller<WorldParts->ItemCount;Teller++)
	{
		CWorldPart_Destroy(WorldParts->Items[Teller]);
		WorldParts->Items[Teller] = NULL;
	}
}
