#define PI 3.1415926535897932384626433832795

class Math
{
public:
	Math();
	~Math();
	static long CalculateDistance(const POINT& pt1, const POINT& pt2);
	static long CalculateDistance(long x1, long y1, long x2, long y2);
	static BYTE CalcPercent(DWORD dwVal, DWORD dwMaxVal, BYTE iMin = 0);
	static POINT PickFarcastLocation(POINT Target);
	static long CalculateAngle(const POINT& pt1, const POINT& pt2);
	static long CalculateAngle(long x1, long y1, long x2, long y2);
	static void NormalizeAngle(int& rAngle);
	static POINT CalculatePointOnTrack(const POINT& ptOrigin, int nRadius, int nAngle);
};
