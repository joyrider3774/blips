#include "CViewPort.h"
#include "Common.h"

CViewPort::CViewPort(int MinX,int MinY, int MaxX,int MaxY,int MinX2,int MinY2,int MaxX2,int MaxY2)
{
	if ((MinX < NrOfCols) && (MinX >= 0) && (MaxX + 1 < NrOfCols) && (MaxX >= 0) &&
		(MinY < NrOfRows) && (MinY >= 0) && (MaxY + 1 < NrOfRows) && (MaxY >= 0))
	{
		VPMinX = MinX;
		VPMinY = MinY;
		VPMaxX = MaxX + 1;
		VPMaxY = MaxY + 1;
		MinScreenX = VPMinX * TileWidth;
		MinScreenY = VPMinY * TileHeight;
		MaxScreenX = VPMaxX * TileHeight;
		MaxScreenY = VPMaxY * TileWidth;
	}
	else
	{
		VPMinX = 0;
		VPMinY = 0;
		VPMaxX = NrOfColsVisible;
		VPMaxY = NrOfRowsVisible;
		MinScreenX = 0;
		MinScreenY = 0;
		MaxScreenX = WINDOW_WIDTH;
		MaxScreenY = WINDOW_HEIGHT;
	}
	if ((MinX2 < NrOfCols) && (MinX2 >= 0) && (MaxX2 +1< NrOfCols) && (MaxX2 >= 0) &&
		(MinY2 < NrOfRows) && (MinY2 >= 0) && (MaxY2 +1< NrOfRows) && (MaxY2 >= 0))
	{
		VPLimitMinX = MinX2;
		VPLimitMinY = MinY2;
		VPLimitMaxX = MaxX2+1; //needs to be one more, need to get max element + 1 as we need to get the ending of the previous tile
		VPLimitMaxY = MaxY2+1; //needs to be one more, need to get max element + 1 as we need to get the ending of the previous tile
	}
	else
	{
		VPLimitMinX = 0;
		VPLimitMinY = 0;
		VPLimitMaxX = NrOfCols; //needs to be one more, need to get max element + 1 as we need to get the ending of the previous tile
		VPLimitMaxY = NrOfRows; //needs to be one more, need to get max element + 1 as we need to get the ending of the previous tile
	}
	Width = VPMaxX - VPMinX;
	Height = VPMaxY - VPMinY;

}

void CViewPort::SetVPLimit(int MinX,int MinY, int MaxX,int MaxY)
{
	if ((MinX < NrOfCols) && (MinX >= 0) && (MaxX + 1 < NrOfCols) && (MaxX + 1 >= 0) &&
		(MinY < NrOfRows) && (MinY >= 0) && (MaxY + 1 < NrOfRows) && (MaxY + 1 >= 0))
	{
		VPLimitMinX = MinX;
		VPLimitMinY = MinY;
		VPLimitMaxX = MaxX + 1; //needs to be one more, need to get max element + 1 as we need to get the ending of the previous tile
		VPLimitMaxY = MaxY + 1; //needs to be one more, need to get max element + 1 as we need to get the ending of the previous tile
	}
	else
	{
		VPLimitMinX = 0;
		VPLimitMinY = 0;
		VPLimitMaxX = NrOfCols; //needs to be one more, need to get max element + 1 as we need to get the ending of the previous tile
		VPLimitMaxY = NrOfRows; //needs to be one more, need to get max element + 1 as we need to get the ending of the previous tile
	}
	if (VPLimitMaxX - VPLimitMinX < Width)
	{
		if (VPLimitMaxX - Width >= 0)
			VPLimitMinX = VPLimitMaxX - (Width);
		else
		{
			VPLimitMinX = 0;
			VPLimitMaxX = VPLimitMinX + (Width);
		}
	}
	if (VPLimitMaxY - VPLimitMinY < Height)
	{
		if (VPLimitMaxY - Height >= 0)
			VPLimitMinY = VPLimitMaxY - (Height);
		else
		{
			VPLimitMinY = 0;
			VPLimitMaxY = VPLimitMinY + (Height);
		}
	}
}

void CViewPort::Move(int Xi,int Yi)
{
	if ((MinScreenX + Xi <= TileWidth*(VPLimitMaxX)) && (MinScreenX + Xi >=VPLimitMinX*TileWidth) && (MaxScreenX + Xi <= TileWidth*(VPLimitMaxX)) && (MaxScreenX + Xi >=VPLimitMinX *TileWidth) &&
		(MinScreenY + Yi <= TileHeight*(VPLimitMaxY)) && (MinScreenY + Yi >=VPLimitMinY*TileHeight) && (MaxScreenY + Yi <= TileHeight*(VPLimitMaxY)) && (MaxScreenY + Yi >=VPLimitMinY*TileHeight))
		{

			MinScreenX +=Xi;
			MaxScreenX +=Xi;
			MinScreenY +=Yi;
			MaxScreenY +=Yi;
			VPMinX = MinScreenX / TileWidth;
			VPMinY = MinScreenY  / TileHeight;
			VPMaxX = MaxScreenX  / TileHeight;
			VPMaxY = MaxScreenY  / TileWidth;
		}
}

void CViewPort::SetViewPort(int MinX,int MinY, int MaxX,int MaxY)
{
	if ((MinX <= VPLimitMaxX) && (MinX >= VPLimitMinX) && (MaxX + 1 <= VPLimitMaxX) && (MaxX + 1 >= VPLimitMinX) &&
		(MinY <= VPLimitMaxY) && (MinY >= VPLimitMinY) && (MaxY + 1 <= VPLimitMaxY) && (MaxY + 1 >= VPLimitMinY))
	{
		VPMinX = MinX;
		VPMinY = MinY;
		VPMaxX = MaxX + 1; //needs to be one more, need to get max element + 1 as we need to get the ending of the previous tile
		VPMaxY = MaxY + 1; //needs to be one more, need to get max element + 1 as we need to get the ending of the previous tile
		MinScreenX = VPMinX * TileWidth;
		MinScreenY = VPMinY * TileHeight;
		MaxScreenX = VPMaxX * TileHeight;
		MaxScreenY = VPMaxY * TileWidth;
	}
	else
	{
		VPMinX = MinX;
		VPMinY = MinY;
		VPMaxX = MaxX + 1; //needs to be one more, need to get max element + 1 as we need to get the ending of the previous tile
		VPMaxY = MaxY + 1; //needs to be one more, need to get max element + 1 as we need to get the ending of the previous tile
		if (VPMinX < VPLimitMinX)
		{
			VPMinX = VPLimitMinX;
			VPMaxX = VPLimitMinX + NrOfColsVisible;

		}
		if (VPMaxX > VPLimitMaxX)
		{
			VPMinX = VPLimitMaxX - NrOfColsVisible;
			VPMaxX = VPLimitMaxX;
		}
		if (VPMinY < VPLimitMinY)
		{
			VPMaxY = VPLimitMinY + NrOfRowsVisible;
			VPMinY = VPLimitMinY;
		}
		if (VPMaxY > VPLimitMaxY)
		{
			VPMinY = VPLimitMaxY - NrOfRowsVisible;
			VPMaxY = VPLimitMaxY;
		}
		MinScreenX = VPMinX * TileWidth;
		MinScreenY = VPMinY * TileHeight;
		MaxScreenX = VPMaxX * TileHeight;
		MaxScreenY = VPMaxY * TileWidth;
	}
	Width = VPMaxX - VPMinX;
	Height = VPMaxY - VPMinY;
}

CViewPort::~CViewPort()
{
}



