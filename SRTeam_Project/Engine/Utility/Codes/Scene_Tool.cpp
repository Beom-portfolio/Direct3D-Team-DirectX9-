#include "..\Headers\Scene_Tool.h"


USING(Engine)


CScene_Tool::CScene_Tool(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pObject_Manager(CObject_Manager::GetInstance())
{
	m_pGraphicDev->AddRef();
	m_pObject_Manager->AddRef();
}

HRESULT CScene_Tool::Ready_Scene(void)
{

	return NOERROR;
}

//
_int CScene_Tool::Update_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pObject_Manager)
		return -1;

	_int		iExitCode = 0;

	iExitCode = m_pObject_Manager->Update_Object_Manager(fTimeDelta);

	return iExitCode;
}

void CScene_Tool::Render_Scene(void)
{
	
}

HRESULT CScene_Tool::Release_Object(const _uint & iSceneIdx)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Release_Object(iSceneIdx);	
}

_ulong CScene_Tool::Free(void)
{
	_ulong dwRefCnt = 0;


	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pObject_Manager); //By.Jangwon
	
	return dwRefCnt;
}
