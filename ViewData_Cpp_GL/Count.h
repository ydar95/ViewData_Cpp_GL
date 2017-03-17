// ------------------------------  header file -----------------------
#pragma once
#ifndef _HEAD_CTime

#define _HEAD_CTime
#include <Windows.h>
#include <iostream>
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

#define CTIME_COUNT_MILLIS(func)	\
			auto t1=CTime::GetMicroS();	\
			func			   ;	\
			std::cout<<CTime::GetMicroS()-t1<<endl;

#endif // !_HEAD_CTime