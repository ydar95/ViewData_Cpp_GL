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
	static UINT64 GetMilliS();//����
	static UINT64 GetMicroS();//΢��
	static UINT64 GetNanoS();//����
private:
	static UINT64 m_frequency;
	static CTime m_self;
};


#endif // !_HEAD_CTime