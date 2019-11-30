#define PI 3.1415926535897932384626433832795

long CalculateDistance(const POINT & pt1, const POINT & pt2);
long CalculateDistance(long x1, long y1, long x2, long y2);
BYTE CalcPercent(DWORD dwVal, DWORD dwMaxVal, BYTE iMin = 0);
POINT PickFarcastLocation(POINT Target);
long CalculateAngle(const POINT& pt1, const POINT& pt2);
long CalculateAngle(long x1, long y1, long x2, long y2);
void NormalizeAngle(int& rAngle);
POINT CalculatePointOnTrack(const POINT& ptOrigin, int nRadius, int nAngle);