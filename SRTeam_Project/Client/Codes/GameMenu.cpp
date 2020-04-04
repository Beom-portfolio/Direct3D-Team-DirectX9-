#include "stdafx.h"
#include "..\Headers\GameMenu.h"

#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Input_Device.h"
#include "SceneMgr.h"
#include "Stage1.h"
#include "Client_Menu.h"
#include "Client_Button.h"

CGameMenu::CGameMenu(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{
}

HRESULT CGameMenu::Ready_Scene(void)
{
	if (Ready_Component())
		return E_FAIL;
	
	/*if (Ready_Proj_SetUp())
		return E_FAIL;*/

	if (Ready_Layer_Buttom(L"Layer_Start"))
		return E_FAIL;

	if (Ready_Layer_Menu(L"Layer_Menu"))
		return E_FAIL;

	CSoundMgr::GetInstance()->PlayBGM(L"Logo.mp3", CSoundMgr::Channel_BGM);
	

	return NOERROR;
}

int CGameMenu::Update_Scene(const _float & fTimeDelta)
{
	ChangeScene(SCENE_STAGE1);     // 메뉴 나중에    바로 일단 스테이지로

	return CScene::Update_Scene(fTimeDelta);
}

void CGameMenu::Render_Scene(void)
{
	CScene::Render_Scene();
}

HRESULT CGameMenu::Ready_Component(void)
{
	//Engine::CComponent*			pComponent;

	return NOERROR;
}

HRESULT CGameMenu::Ready_Layer_Menu(const _tchar* pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*	pGameObject = nullptr;

	// For.Object

	pGameObject = CClient_Menu::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_MENU, pLayerTag, pGameObject)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CGameMenu::Ready_Layer_Buttom(const _tchar* pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CClient_Button::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_MENU, pLayerTag, pGameObject)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CGameMenu::Ready_Proj_SetUp()
{
	_matrix		matProj, matView;

	matView.Identity();
	// 뷰스페이스 변환행렬을 만들자.

	// 1. 카메라의 월드변환행렬(상태행렬)을 만들어보자.
	_vec3		vRight, vUp, vLook, vPos, vAt, vEye, vFixdUp;

	vAt = _vec3(0.f, 0.f, 0.f);

	vEye = _vec3(0.f, 0.f, -3.f);

	vFixdUp = _vec3(0.f, 1.f, 0.f);

	// look
	vLook = vAt - vEye;

	// right
	D3DXVec3Cross(&vRight, &vFixdUp, &vLook);

	// up
	D3DXVec3Cross(&vUp, &vLook, &vRight);

	// vPos
	vPos = vEye;

	// right
	memcpy((_vec3*)&matView.m[0][0], D3DXVec3Normalize(&vRight, &vRight), sizeof(_vec3));
	// up
	memcpy((_vec3*)&matView.m[1][0], D3DXVec3Normalize(&vUp, &vUp), sizeof(_vec3));
	// look
	memcpy((_vec3*)&matView.m[2][0], D3DXVec3Normalize(&vLook, &vLook), sizeof(_vec3));
	// pos
	memcpy((_vec3*)&matView.m[3][0], &vPos, sizeof(_vec3));



	D3DXMatrixInverse(&matView, nullptr, &matView);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);



	
	matProj.Identity();

	matProj._11 = (1.f / tanf(D3DXToRadian(60.f) * 0.5f)) / (WINCX / WINCY);
	matProj._22 = 1.f / tanf(D3DXToRadian(60.f) * 0.5f);
	matProj._33 = 1000.f / (1000.f - 0.2f);
	matProj._43 = (1000.f * 0.2f) / (1000.f - 0.2f) * -1.0f;
	matProj._34 = 1.f;
	matProj._44 = 0.f;

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	return NOERROR;
}

HRESULT CGameMenu::ChangeScene(SCENETYPE eSceneID)
{
	if (m_pInput_Device->Get_DIKeyState(DIK_RETURN) & 0x80)
	{
		CSoundMgr::GetInstance()->StopSound(CSoundMgr::Channel_BGM);

		CScene*			pNewScene = nullptr;

		switch (eSceneID)
		{
		case SCENE_STAGE1:
			pNewScene = CStage1::Create(m_pGraphicDev);
			if (nullptr == pNewScene)
				return E_FAIL;
			break;
		}
	
		if (FAILED(CSceneMgr::GetInstance()->SetUp_CurrentScene(pNewScene)))
			return E_FAIL;
	}
	return NOERROR;
}

CGameMenu * CGameMenu::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGameMenu *	pInstance = new CGameMenu(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MessageBox(0, L"CGameMenu Created Failed", nullptr, MB_OK);
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CGameMenu::Free()
{
	if (FAILED(CScene::Release_Object(SCENE_MENU)))
		MSG_BOX("CLogo Release Failed");

	if (FAILED(Engine::CComponent_Manager::GetInstance()->Release_Component(SCENE_MENU)))
		MSG_BOX("Release_Component Release Failed");


	return CScene::Free();
}

