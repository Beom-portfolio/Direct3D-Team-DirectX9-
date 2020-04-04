#include "stdafx.h"
#include "..\Headers\SceneMgr.h"

#include "Component_Manager.h"
#include "Object_Manager.h"
#include "SceneMgr.h"
#include "Scene.h"

USING(Engine)
IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr()
	: m_pCurrent_Scene(nullptr)
	, m_pNew_Scene(nullptr)
{
}

CSceneMgr::~CSceneMgr()
{
}

HRESULT CSceneMgr::SetUp_CurrentScene(CScene* pNewScene)
{
	if (nullptr == pNewScene)
		return E_FAIL;

	_ulong dwRefCnt = 0;

	if (nullptr != m_pCurrent_Scene)
		m_pNew_Scene = pNewScene;
	else
		m_pCurrent_Scene = pNewScene;

	return NOERROR;
}

HRESULT CSceneMgr::SetUp_Renderer(CRenderer * pRenderer)
{
	if (nullptr != m_pRenderer)
		return E_FAIL;

	m_pRenderer = pRenderer;
	m_pRenderer->AddRef();

	return NOERROR;
}

HRESULT CSceneMgr::Ready_SceneMgr(const _uint & iMaxSize)
{
	if (FAILED(CComponent_Manager::GetInstance()->Reserve_ComponentContainer(iMaxSize)))
		return E_FAIL;

	if (FAILED(CObject_Manager::GetInstance()->Reserve_ObjectContainer(iMaxSize)))
		return E_FAIL;

	return NOERROR;
}


_int CSceneMgr::Update_SceneMgr(const _float & fTimeDelta)
{
	if (nullptr == m_pCurrent_Scene)
		return -1;

	_ulong			dwRefCnt = 0;
	
	if (nullptr != m_pNew_Scene)
	{
		
		if (dwRefCnt = Engine::Safe_Release(m_pCurrent_Scene))
			return -1;
		m_pCurrent_Scene = m_pNew_Scene;
		m_pNew_Scene = nullptr;
	}

	_int		iExitCode = 0;
	iExitCode = m_pCurrent_Scene->Update_Scene(fTimeDelta);

	if (2 == iExitCode)
	{
		return iExitCode;
	}


	if (iExitCode & 0x80000000)
		return iExitCode;

	iExitCode = m_pCurrent_Scene->LastUpdate_Scene(fTimeDelta);
	if (iExitCode & 0x80000000)
		return iExitCode;

	return iExitCode;
}

void CSceneMgr::Render_SceneMgr(void)
{
	if (nullptr != m_pRenderer)
		m_pRenderer->Render_RenderGroup();

	if (nullptr != m_pCurrent_Scene)
		m_pCurrent_Scene->Render_Scene();
}

_ulong CSceneMgr::Free(void)
{
	_ulong dwRefCnt = 0;

	Engine::Safe_Release(m_pRenderer);

	if (dwRefCnt = Engine::Safe_Release(m_pCurrent_Scene))
		MSG_BOX("Current_Scene Release Failed");


	return dwRefCnt;
}
