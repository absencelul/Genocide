class Chicken
{
public:
	Chicken();
	virtual ~Chicken();
	void Rescue();
	bool IsTown(UnitAny* pUnit);
	DWORD LifeMana(bool Life);
	bool Portal();
};
