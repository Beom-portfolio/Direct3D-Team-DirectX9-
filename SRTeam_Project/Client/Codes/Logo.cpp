#include "stdafx.h"
#include "..\Headers\Logo.h"

#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Input_Device.h"
#include "SceneMgr.h"
#include "Dynamic_Camera.h"
#include "GameMenu.h"
#include "Back_Logo.h"


CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
	, m_pLogoOjbect(nullptr)
{
}

HRESULT CLogo::Ready_Scene(void)
{
	if (Ready_LogoComponent())
		return E_FAIL;

	/*if (Ready_Layer_Camera(L"Layer_Camara", &_vec3(0.f, 0.f, -5.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f)))
		return E_FAIL;*/

	if (Ready_Layer_Logo(L"Layer_BackGround"))
		return E_FAIL;

	return NOERROR;
}

int CLogo::Update_Scene(const _float & fTimeDelta)
{
 	ChangeScene(SCENE_MENU);     // 메뉴 나중에    바로 일단 스테이지로

	return CScene::Update_Scene(fTimeDelta);
}

void CLogo::Render_Scene(void)
{
	CScene::Render_Scene();
}

HRESULT CLogo::Ready_LogoComponent(void)
{
	Engine::CComponent*			pComponent = nullptr;

	return NOERROR;
}

HRESULT CLogo::Ready_Layer_Camera(const _tchar * pLayerTag, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	// For.Camera
	pGameObject = CDynamic_Camera::Create(m_pGraphicDev, pEye, pAt, pUp);
	if (nullptr == pGameObject)
		return E_FAIL;
	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_LOGO, pLayerTag, pGameObject)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CLogo::Ready_Layer_Logo(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	

	// For.Object

	m_pLogoOjbect = CBack_Logo::Create(m_pGraphicDev);
	if (nullptr == m_pLogoOjbect)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_LOGO, pLayerTag, m_pLogoOjbect)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CLogo::ChangeScene(SCENETYPE eSceneID)
{
	if (DCAST(CBack_Logo*, m_pLogoOjbect)->Get_FrameEnd())
	{
		CScene*			pNewScene = nullptr;

		switch (eSceneID)
		{
		case SCENE_MENU:
			pNewScene = CGameMenu::Create(m_pGraphicDev);
			if (nullptr == pNewScene)
				return E_FAIL;
			break;
		}

		if (FAILED(CSceneMgr::GetInstance()->SetUp_CurrentScene(pNewScene)))
			return E_FAIL;
	}

	if (m_pInput_Device->Get_DIKeyState(DIK_SPACE) & 0x80)
	{
		CScene*			pNewScene = nullptr;

		switch (eSceneID)
		{
		case SCENE_MENU:
			pNewScene = CGameMenu::Create(m_pGraphicDev);
			if (nullptr == pNewScene)
				return E_FAIL;
			break;
		}

		if (FAILED(CSceneMgr::GetInstance()->SetUp_CurrentScene(pNewScene)))
			return E_FAIL;
	}
	return NOERROR;
}


CLogo * CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo *	pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MessageBox(0, L"CLogo Created Failed", nullptr, MB_OK);
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CLogo::Free(void)
{
	if (FAILED(CScene::Release_Object(SCENE_LOGO)))
		MSG_BOX("CLogo Release Failed");

	if (FAILED(Engine::CComponent_Manager::GetInstance()->Release_Component(SCENE_LOGO)))
		MSG_BOX("Release_Component Release Failed");


	return CScene::Free();
}
