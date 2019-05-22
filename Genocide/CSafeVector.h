#ifndef _CSAFEVECTOR_H
#define _CSAFEVECTOR_H

template<class _Ty>
class CSafeVector : public std::vector<_Ty>
{
public:

	bool IsLocked;

	CSafeVector()
	{
		InitializeCriticalSection(&m_CriticalSection);
		IsLocked = false;
	}

	~CSafeVector()
	{
		DeleteCriticalSection(&m_CriticalSection);
	}

	void Lock(void)
	{
		EnterCriticalSection(&m_CriticalSection);
		IsLocked = true;
	}

	void Unlock(void)
	{
		LeaveCriticalSection(&m_CriticalSection);
		IsLocked = false;
	}

private:

	CRITICAL_SECTION m_CriticalSection;
};

template <typename T>
void CopyVector(std::vector<T> & dest, std::vector<T> source)
{
	for (unsigned int i = 0; i < source.size(); ++i)
		dest.push_back(source[i]);
}

#endif