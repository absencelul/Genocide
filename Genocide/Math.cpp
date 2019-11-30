#include "Hack.h"

//Calculates distance
long CalculateDistance(const POINT& pt1, const POINT& pt2)
{
	return CalculateDistance(pt1.x, pt1.y, pt2.x, pt2.y);
}

//Calculates distance
long CalculateDistance(long x1, long y1, long x2, long y2)
{
	return (long)::sqrt((double)((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}

//Get Percentage
BYTE CalcPercent(DWORD dwVal, DWORD dwMaxVal, BYTE iMin) {
	if (dwVal == 0 || dwMaxVal == 0)
		return NULL;

	BYTE iRes = (BYTE)((double)dwVal / (double)dwMaxVal * 100.0);

	if (iRes > 100)
		iRes = 100;

	return max(iRes, iMin);
}

POINT PickFarcastLocation(POINT Target)
{
	POINT Origin = { (Me->pPath->xPos), (Me->pPath->yPos) };
	ScreenToAutoMap(&Target);

	INT Angle = CalculateAngle(Origin, Target);
	INT Radius = FcDistance * 2;
	POINT Cast = { NULL };

	while (TRUE)
	{
		Cast = CalculatePointOnTrack(Origin, Radius--, Angle);

		if (!D2COMMON_CheckCollision(Me->pPath->pRoom1, Cast.x, Cast.y, 2))
			if (::abs(Me->pPath->xPos - Cast.x) <= FcDistance && ::abs(Me->pPath->yPos - Cast.y) <= FcDistance)
				break;
	}

	return Cast;
}

void NormalizeAngle(int& rAngle)
{
	if (::abs(rAngle) >= 360)
		rAngle %= 360;

	if (rAngle < 0)
		rAngle += 360;
}

POINT CalculatePointOnTrack(const POINT& ptOrigin, int nRadius, int nAngle)
{
	if (nRadius == 0)
		return ptOrigin;

	NormalizeAngle(nAngle);

	POINT pt;
	pt.x = LONG(DOUBLE(ptOrigin.x) + ::cos((DOUBLE)nAngle * PI / 180.0) * (DOUBLE)nRadius);
	pt.y = LONG(DOUBLE(ptOrigin.y) - ::sin((DOUBLE)nAngle * PI / 180.0) * (DOUBLE)nRadius);
	return pt;
}

long CalculateAngle(const POINT& pt1, const POINT& pt2)
{
	return CalculateAngle(pt1.x, pt1.y, pt2.x, pt2.y);
}

long CalculateAngle(long x1, long y1, long x2, long y2)
{
	// mathematic stuff
	if (x1 == x2 && y1 == y2)
		return 0;

	double fAngle = 0.0;

	if (x1 == x2)
	{
		// vertical special case
		fAngle = y2 > y1 ? 270.0 : 90.0;
	}
	else if (y1 == y2)
	{
		// horizontal special case
		fAngle = x2 > x1 ? 0.0 : 180.0;
	}
	else
	{
		// common case
		fAngle = ::atan(((double)y2 - (double)y1) / ((double)x2 - (double)x1)) * 180.0 / PI;

		// determine the phases (1-4)
		// Phases allocation figure:
		/*
		y
		|
		P2     |    P1
		|
		-----------+----------->x
		|
		P3	   |    P4
		|
		*/
		int nPhase = 0;
		if (y2 < y1)
			nPhase = x2 > x1 ? 1 : 2;
		else
			nPhase = x2 > x1 ? 4 : 3;

		// adjust the angle according to phases
		switch (nPhase)
		{
		case 1:
			fAngle = -fAngle;
			break;

		case 2:
		case 3:
			fAngle = 180.0 - fAngle;
			break;

		case 4:
			fAngle = 360.0 - fAngle;
			break;

		default:
			fAngle = 0.0;
			break;
		}
	}

	return (long)fAngle;
}