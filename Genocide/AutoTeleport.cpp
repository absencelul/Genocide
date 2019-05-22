#include "DllMain.h"

CPathFinder::CPathFinder()
{
	m_pMap = NULL;
	m_nSizeX = 0;
	m_nSizeY = 0;
	memset(&m_ptStart, 0, sizeof(POINT));
	memset(&m_ptEnd, 0, sizeof(POINT));
}

CPathFinder::~CPathFinder()
{
}

BOOL CPathFinder::CreateBestPath(POINT ptStart, POINT ptEnd, LPPOINT lpBuffer, DWORD dwMaxCount)
{
	POINT ptPos;
	INT nFlag, nIterations = 0;
	DWORD dwFound;

	if (lpBuffer == NULL || dwMaxCount == 0 || m_nSizeX <= 0 || m_nSizeY <= 0 || m_pMap == NULL)
		return 0;

	if (ptStart.x > m_nSizeX || ptStart.y > m_nSizeY || ptEnd.x > m_nSizeX || ptEnd.y > m_nSizeY)
		return 0;

	memset(lpBuffer, 0, sizeof(POINT) * dwMaxCount);
	m_ptStart = ptStart;
	m_ptEnd = ptEnd;

	CreateDistanceTable();

	lpBuffer[0] = ptStart;
	dwFound = 1;
	ptPos = ptStart;

	nFlag = GetNextMove(ptPos);

	while (nFlag != PATH_FAILED && dwFound < dwMaxCount)
	{
		if (nIterations > 5000)
			return 0;

		if (nFlag == PATH_REACHED)
		{
			lpBuffer[dwFound] = ptEnd;
			dwFound++;
			return dwFound;
		}

		int nRedundancy = CheckRedundancy(lpBuffer, dwFound, ptPos);
		if (nRedundancy == -1)
		{
			lpBuffer[dwFound] = ptPos;
			dwFound++;
		}
		else
		{
			dwFound = nRedundancy + 1;
			lpBuffer[dwFound] = ptPos;
		}

		nFlag = GetNextMove(ptPos);
		nIterations++;
	}

	return FALSE;
}

VOID CPathFinder::SetPathingMap(LPWORD *pMap, INT nSizeX, INT nSizeY)
{
	m_pMap = pMap;
	m_nSizeX = nSizeX;
	m_nSizeY = nSizeY;
}

PATHRETURN CPathFinder::GetNextMove(POINT& pos, int nAdjust)
{
	if (Math::CalculateDistance(m_ptEnd, pos) <= range)
	{
		Funcs->Print(0, 4, "Range Dist: %d", range);
		pos = m_ptEnd;
		return PATH_REACHED;
	}

	if (!IsValidIndex(pos.x, pos.y))
		return PATH_FAILED;

	BlockLocation(pos, nAdjust);

	POINT p, best;
	int value = RANGE_INVALID;

	for (p.x = pos.x - range; p.x <= pos.x + range; p.x++)
	{
		for (p.y = pos.y - range; p.y <= pos.y + range; p.y++)
		{
			if (!IsValidIndex(p.x, p.y))
				continue;

			if (m_pMap[p.x][p.y] < value && Math::CalculateDistance(p, pos) <= range)
			{
				value = m_pMap[p.x][p.y];
				best = p;
			}
		}
	}

	if (value >= RANGE_INVALID)
		return PATH_FAILED;

	pos = best;
	BlockLocation(pos, nAdjust);
	return PATH_CONTINUE;
}

BOOL CPathFinder::CreateDistanceTable()
{
	if (m_pMap == NULL)
		return FALSE;

	for (int x = 0; x < m_nSizeX; x++)
	{
		for (int y = 0; y < m_nSizeY; y++)
		{
			if ((m_pMap[x][y] % 2) == 0)
				m_pMap[x][y] = (SHORT)Math::CalculateDistance(x, y, m_ptEnd.x, m_ptEnd.y);
			else
				m_pMap[x][y] = RANGE_INVALID;
		}
	}

	m_pMap[m_ptEnd.x][m_ptEnd.y] = 1;
	return TRUE;
}

BOOL CPathFinder::CheckRedundancy(LPPOINT lpPath, INT nCount, POINT ptPos)
{
	if (lpPath == NULL || nCount == 0)
		return -1;

	for (INT i = 0; i < nCount; i++)
	{
		if (Math::CalculateDistance(ptPos, lpPath[i]) <= (range / 3))
			return i;
	}

	return -1;
}

BOOL CPathFinder::IsValidIndex(INT nX, INT nY)
{
	return nX >= 0 && nX < m_nSizeX && nY >= 0 && nY < m_nSizeY;
}

VOID CPathFinder::BlockLocation(POINT pos, INT nRange)
{
	nRange = max(nRange, 1);

	for (INT i = pos.x - nRange; i < pos.x + nRange; i++)
	{
		for (INT j = pos.y - nRange; j < pos.y + nRange; j++)
		{
			if (IsValidIndex(i, j))
				m_pMap[i][j] = RANGE_INVALID;
		}
	}
}