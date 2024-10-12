#ifndef CVIEWPORT_H
#define CVIEWPORT_H

class CViewPort
{
    public:
        int VPMinX,VPMinY,VPMaxX,VPMaxY,MinScreenX,MinScreenY,MaxScreenX,MaxScreenY,Width,Height,VPLimitMinX,VPLimitMaxX,VPLimitMinY,VPLimitMaxY;
        CViewPort(int MinX,int MinY, int MaxX,int MaxY,int MinX2,int MinY2,int MaxX2,int MaxY2);
        void SetVPLimit(int MinX,int MinY, int MaxX,int MaxY);
        void Move(int Xi,int Yi);
        void SetViewPort(int MinX,int MinY, int MaxX,int MaxY);
        ~CViewPort();
};

#endif