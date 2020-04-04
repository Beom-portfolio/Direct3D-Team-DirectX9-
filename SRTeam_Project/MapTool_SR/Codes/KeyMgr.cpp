#include "stdafx.h"
#include "..\Headers\KeyMgr.h"


CKeyMgr::CKeyMgr()
{
	ZeroMemory(m_bKeyDown, KEYMAX);
	ZeroMemory(m_bKeyUp, KEYMAX);
}


CKeyMgr::~CKeyMgr()
{
}

bool CKeyMgr::StayKeyDown(INT nKey)
{
	if (GetAsyncKeyState(nKey) & 0x8000)
	{
		return TRUE;
	}
	return FALSE;
}

bool CKeyMgr::OnceKeyDown(INT nKey)
{
	if (GetAsyncKeyState(nKey) & 0x8000)
	{
		if (false == m_bKeyDown[nKey])
		{
			m_bKeyDown[nKey] = true;
			return TRUE;
		}
	}
	else
	{
		m_bKeyDown[nKey] = false;
	}

	return FALSE;

}

bool CKeyMgr::OnceKeyUp(INT nKey)
{

	if (GetAsyncKeyState(nKey) & 0x8000)
	{
		m_bKeyUp[nKey] = true;
	}
	else
	{
		if (m_bKeyUp[nKey] == true)
		{
			m_bKeyUp[nKey] = false;

			return TRUE;
		}
	}

	return FALSE;
}

bool CKeyMgr::IsToggleKey(INT nKey)
{
	if (GetAsyncKeyState(nKey) & 0x8000)
	{
		m_bKeyUp[nKey] = true;
	}

	else
	{
		// 눌려진 순간 true가 되므로 true일때만 진입
		if (m_bKeyUp[nKey] == true)
		{
			//  키상태를 false로 만들어 재진입 불가
			m_bKeyUp[nKey] = false;

			return TRUE;
		}
	}

	return FALSE;
}

_ulong CKeyMgr::Free(void)
{
	ZeroMemory(m_bKeyDown, KEYMAX);
	ZeroMemory(m_bKeyUp, KEYMAX);

	return 0;
}
