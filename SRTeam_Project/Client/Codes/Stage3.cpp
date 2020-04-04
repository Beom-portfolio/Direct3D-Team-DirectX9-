#include "stdafx.h"
#include "..\Headers\Stage3.h"

#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Input_Device.h"
#include "SceneMgr.h"
#include "Dynamic_Camera.h"

CStage3::CStage3(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{
}

HRESULT CStage3::Ready_Scene(void)
{
	if (Ready_Stage3Component())
		return E_FAIL;

	if (Ready_Layer_Camera(L"Layer_Camara", &_vec3(0.f, 0.f, -5.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f)))
		return E_FAIL;

	if (Ready_Layer_Terrain(L"Layer_Terrain"))
		return E_FAIL;

	return NOERROR;
}

int CStage3::Update_Scene(const _float & fTimeDelta)
{
	return CScene::Update_Scene(fTimeDelta);
}

void CStage3::Render_Scene(void)
{
	CScene::Render_Scene();
}

HRESULT CStage3::Ready_Stage3Component(void)
{
	return NOERROR;
}

HRESULT CStage3::Ready_Layer_Camera(const _tchar * pLayerTag, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp)
{
	return NOERROR;
}

HRESULT CStage3::Ready_Layer_Terrain(const _tchar * pLayertag)
{
	return NOERROR;
}

HRESULT CStage3::ChangeScene(SCENETYPE eSceneID)
{
	return NOERROR;
}

CStage3 * CStage3::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage3 *	pInstance = new CStage3(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MessageBox(0, L"CStage2 Created Failed", nullptr, MB_OK);
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CStage3::Free(void)
{
	if (FAILED(CScene::Release_Object(SCENE_STAGE3)))
		MSG_BOX("CStage1 Release Failed");

	if (FAILED(Engine::CComponent_Manager::GetInstance()->Release_Component(SCENE_STAGE3)))
		MSG_BOX("CStage1 Release Failed");


	return CScene::Free();
}
