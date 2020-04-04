#pragma once

#include "Engine_Defines.h"
#include "Base.h"

const int KEYMAX = 256;

class CKeyMgr : public CBase
{
	DECLARE_SINGLETON(CKeyMgr)
private:
	explicit CKeyMgr();
	virtual ~CKeyMgr();
private:
	bool	m_bKeyDown[KEYMAX];
	bool	m_bKeyUp[KEYMAX];
public:
	bool	StayKeyDown(INT nKey);
	bool	OnceKeyDown(INT nKey);
	bool	OnceKeyUp(INT nKey);
	bool	IsToggleKey(INT nKey);
public:
	_ulong Free(void);
};

