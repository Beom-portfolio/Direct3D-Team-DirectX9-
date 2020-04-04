#include "stdafx.h"
#include "..\Headers\Scene.h"

#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Input_Device.h"

USING(Engine)

CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev)
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pInput_Device(CInput_Device::GetInstance())
{
	m_pGraphicDev->AddRef();
	m_pObject_Manager->AddRef();
	m_pComponent_Manager->AddRef();
	m_pInput_Device->AddRef();

}

HRESULT CScene::Ready_Scene(void)
{
	return NOERROR;
}

_int CScene::Update_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pObject_Manager)
		return -1;

	_int		iExitCode = 0;


	iExitCode = m_pObject_Manager->Update_Object_Manager(fTimeDelta);

	return iExitCode;
}

_int CScene::LastUpdate_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pObject_Manager)
		return -1;

	_int		iExitCode = 0;

	iExitCode = m_pObject_Manager->LastUpdate_Object_Manager(fTimeDelta);

	return iExitCode;
}

void CScene::Render_Scene(void)
{
}

HRESULT CScene::Release_Object(const _uint & iSceneIdx)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Release_Object(iSceneIdx);
}

_ulong CScene::Free(void)
{
	_ulong dwRefCnt = 0;

	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pGraphicDev);

	return dwRefCnt;
}

