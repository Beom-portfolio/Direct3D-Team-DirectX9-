#pragma once

#include "Engine_Defines.h"


typedef struct tagAmmoInfo
{
	_uint		iAmmo;
	_uint		iBasic;
	_uint		iMaxBasic;
	_uint		iShotGun;
	_uint		iMaxShotGun;
	_uint		iChainGun;
	_uint		iMaxChainGun;
	_uint		iPlasmaGun;
	_uint		iMaxPlasmaGun;
	_uint		iBFG;
	_uint		iMaxBFG;
	

	tagAmmoInfo() {}
	tagAmmoInfo(const _uint& _iAmmo, const _uint& _iBasic, const _uint& _iMaxBasic, const _uint& _iShotGun
		, const _uint& _iMaxShotGun, const _uint& _iChainGun, const _uint& _iMaxChainGun, const _uint& _iPlasmaGun, const _uint& iMaxPlasmaGun, const _uint& _iBFG, const _uint& _iMaxBFG)
	: iAmmo(_iAmmo), iBasic(_iBasic), iMaxBasic(_iMaxBasic), iShotGun(_iShotGun), iMaxShotGun(_iMaxShotGun), iChainGun(_iChainGun), iMaxChainGun(_iMaxChainGun)
	, iPlasmaGun(_iChainGun), iMaxPlasmaGun(_iMaxChainGun), iBFG(_iBFG), iMaxBFG(_iMaxBFG) {}

} AMMOINFO, *pAMMOINFO, *PAMMOINFO;


typedef struct tagStringCompare
{
	const TCHAR* m_pString;

	template<typename T>
	bool operator () (T rData)
	{
		return !lstrcmp(rData.first, m_pString);
	}

	tagStringCompare(const TCHAR* pKey)
		:m_pString(pKey) {}
}STRING_COMPARE;