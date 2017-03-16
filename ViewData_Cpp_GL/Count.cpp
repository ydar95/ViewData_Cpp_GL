// -------------------------------- CTime.cpp ------------------------------
#include <assert.h>
#include "Count.h"
UINT64    CTime::m_frequency = 0;
CTime    CTime::m_self;

CTime::CTime(void)
{
	LARGE_INTEGER fc;

	if (!QueryPerformanceFrequency(&fc))
	{
		assert(FALSE);
	}
	m_frequency = fc.QuadPart;  //计时器的频率
}

CTime::~CTime(void)
{
}

UINT64 CTime::GetMilliS()
{
	assert(m_frequency>1000);
	LARGE_INTEGER fc;
	if (!QueryPerformanceCounter(&fc))
	{
		assert(FALSE);
	}

	UINT64 c = fc.QuadPart;
	UINT64 s = (c) / (m_frequency / 1000);
	return s;
}

UINT64 CTime::GetMicroS()
{
	assert(m_frequency>1000 * 1000);

	LARGE_INTEGER fc;

	if (!QueryPerformanceCounter(&fc))
	{
		assert(FALSE);
	}

	UINT64 c = fc.QuadPart;
	UINT64 s = (c) / (m_frequency / 1000 / 1000);
	return s;
}

UINT64 CTime::GetNanoS()
{
	assert(m_frequency>1000 * 1000 * 1000);

	LARGE_INTEGER fc;

	if (!QueryPerformanceCounter(&fc))
	{
		assert(FALSE);
	}
	UINT64 c = fc.QuadPart;
	UINT64 s = (c) / (m_frequency / 1000 / 1000 / 1000);
	return s;
}