// ------------------------------  header file -----------------------
#pragma once
#ifndef _HEAD_CTime

#define _HEAD_CTime
#include <Windows.h>
class CTime
{
private:
	CTime(void);
	~CTime(void);
public:
	static UINT64 GetMilliS();//∫¡√Î
	static UINT64 GetMicroS();//Œ¢√Î
	static UINT64 GetNanoS();//ƒ…√Î
private:
	static UINT64 m_frequency;
	static CTime m_self;
};

// -------------------------------- CTime.cpp ------------------------------
#include <assert.h>

UINT64    CTime::m_frequency = 0;
CTime    CTime::m_self;

CTime::CTime(void)
{
	LARGE_INTEGER fc;

	if (!QueryPerformanceFrequency(&fc))
	{
		assert(FALSE);
	}
	m_frequency = fc.QuadPart;  //º∆ ±∆˜µƒ∆µ¬ 
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
#endif // !_HEAD_CTime