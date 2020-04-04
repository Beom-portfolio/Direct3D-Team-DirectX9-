#include "stdafx.h"
#include "..\Headers\Scene_Logo.h"

#include "Constant.h"

#include "../Default/MainFrm.h"
#include "../Default/MainForm.h"

#include "Management.h"
#include "Component_manager.h"
#include "Object_manager.h"
#include "Layer.h"

#include "Camera_Dynamic.h"
#include "Terrain.h"
#include "Cube.h"
#include "Back_Object.h"
#include "Monster.h"
#include "Back_Item.h"



CScene_Logo::CScene_Logo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene_Tool(pGraphicDev)
	, m_pPickingCom(nullptr)
	, m_pTargetBufferCom(nullptr)
{
	//나중에 고쳐야함
	//m_pManagement->AddRef();
}

HRESULT CScene_Logo::Ready_Scene(void)
{
	if (FAILED(Ready_LogoComponent()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(L"Layer_Camera", &_vec3(0.f, 5.f, -5.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(L"Layer_Terrain")))
		return E_FAIL;

	SetBufferTarget();

	return NOERROR;
}

_int CScene_Logo::Update_Scene(const _float & fTimeDelta)
{
	return Engine::CScene_Tool::Update_Scene(fTimeDelta);
}

void CScene_Logo::Render_Scene(void)
{
	Engine::CScene_Tool::Render_Scene();
}

HRESULT CScene_Logo::Ready_LogoComponent(void)
{
	// For.Stage_Scene Component
	Engine::CComponent*			pComponent = nullptr;


	// For.Texture_Terrain
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/Terrain/Stage1_Terrain.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_LOGO, L"Component_Texture_Terrain", pComponent)))
		return E_FAIL;

	// For.Texture_Cube
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_CUBE, L"../Bin/Resources/Textures/BackLogo/Wall_Cube/Wall_%d.dds", iCubeIndex);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_LOGO, L"Component_Texture_Cube", pComponent)))
		return E_FAIL;

	// For.Texture_Obj
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/BackLogo/Object/Object_%d.png", iObjIndex);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_LOGO, L"Component_Texture_Object", pComponent)))
		return E_FAIL;

	// For.Texture_Mon
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/BackLogo/Monster/Monster_%d.png", iMonIndex);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_LOGO, L"Component_Texture_Monster", pComponent)))
		return E_FAIL;

	// For.Texture_Item
		pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resources/Textures/BackLogo/Item/Item_%d.png", iItemIndex);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_LOGO, L"Component_Texture_Item", pComponent)))
		return E_FAIL;

	// For.Buffer_Terrain
	pComponent = Engine::CTerrain_Buffer::Create(m_pGraphicDev, 10, 10);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_LOGO, L"Component_Buffer_Terrain", pComponent)))
		return E_FAIL;

	// For.Cube_Rect
	pComponent = Engine::CRect_Cube::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_LOGO, L"Component_Buffer_Cube", pComponent)))
		return E_FAIL;

	// For.Obj_Rect
	pComponent = Engine::CRect_Texture::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_LOGO, L"Component_Buffer_Rect", pComponent)))
		return E_FAIL;

	//For.Picking
	pComponent = Engine::CPicking::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_LOGO, L"Component_Picking", pComponent)))
		return E_FAIL;

	// Clone.Picking
	pComponent = m_pPickingCom = (Engine::CPicking*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_LOGO, L"Component_Picking");
	if (nullptr == pComponent)
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Logo::Ready_Layer_Camera(const _tchar * pLayerTag
	, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*	pGameObject = nullptr;

	// For.Camera
	pGameObject = CCamera_Dynamic::Create(m_pGraphicDev, pEye, pAt, pUp);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_LOGO, pLayerTag, pGameObject)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*	pGameObject = nullptr;

	// For.Terrain
	pGameObject = CTerrain::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_LOGO, pLayerTag, pGameObject)))
		return E_FAIL;

	return NOERROR;
}

void CScene_Logo::Add_Cube(const _tchar * pLayerTag, const _float & fX, const _float & fY, const _float & fZ, const _vec3* vPos,
							/*For.Load*/const bool& bLoadCheck, const _int&	iType)
{
	if (nullptr == m_pObject_Manager)
		return;

	_int iCubeType;

	if (true == bLoadCheck)
		/*For Load*/
		iCubeType = iType;
	/*For Add*/
	else
		iCubeType = ((CMainFrame*)AfxGetMainWnd())->GetMainForm()->GetCubeTool()->GetCubeType();

	Engine::CGameObject*	pGameObject = nullptr;

	pGameObject = CCube::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return;

	DCAST(CCube*, pGameObject)->SetCubeType(iCubeType);
	DCAST(CCube*, pGameObject)->Scaling_Cube(fX, fY, fZ);
	DCAST(CCube*, pGameObject)->TransPos_Cube(vPos);

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_LOGO, pLayerTag, pGameObject)))
		return;
}

void CScene_Logo::Del_Cube(const _tchar * pLayerTag)
{
	m_pObject_Manager->Release_Object_Certain(SCENE_LOGO, pLayerTag);
}

void CScene_Logo::Add_Obj(const _tchar * pLayerTag, const _vec3* vPos, const bool & bLoadCheck, const _int & iType)
{
	if (nullptr == m_pObject_Manager)
		return;

	_int iObjType;

	if (true == bLoadCheck)
		/*For Load*/
		iObjType = iType;
	/*For Add*/
	else
		iObjType = ((CMainFrame*)AfxGetMainWnd())->GetMainForm()->GetObjectTool()->GetObjType();
		

	Engine::CGameObject*	pGameObject = nullptr;

	pGameObject = CBack_Object::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return;

	DCAST(CBack_Object*, pGameObject)->SetObjType(iObjType);
	DCAST(CBack_Object*, pGameObject)->TransPos_Obj(vPos);

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_LOGO, pLayerTag, pGameObject)))
		return;
}

void CScene_Logo::Del_Obj(const _tchar * pLayerTag)
{
	m_pObject_Manager->Release_Object_Certain(SCENE_LOGO, pLayerTag);
}

void CScene_Logo::Add_Mon(const _tchar * pLayerTag, const _float & fX, const _float & fY, const _float & fZ, const _vec3 * vPos, const bool & bLoadCheck, const _int & iType)
{
	if (nullptr == m_pObject_Manager)
		return;

	_int iMonType;

	if (true == bLoadCheck)
		/*For Load*/
		iMonType = iType;
	/*For Add*/
	else
		iMonType = ((CMainFrame*)AfxGetMainWnd())->GetMainForm()->GetMonsterTool()->GetMonType();

	Engine::CGameObject*	pGameObject = nullptr;

	pGameObject = CMonster::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return;

	DCAST(CMonster*, pGameObject)->SetMonType(iMonType);
	DCAST(CMonster*, pGameObject)->Scaling_Mon(fX, fY, fZ);
	DCAST(CMonster*, pGameObject)->TransPos_Mon(vPos);

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_LOGO, pLayerTag, pGameObject)))
		return;
}

void CScene_Logo::Del_Mon(const _tchar * pLayerTag)
{
	m_pObject_Manager->Release_Object_Certain(SCENE_LOGO, pLayerTag);
}

void CScene_Logo::Add_Item(const _tchar * pLayerTag, const _float & fX, const _float & fY, const _float & fZ, const _vec3 * vPos, const bool & bLoadCheck, const _int & iType)
{
	if (nullptr == m_pObject_Manager)
		return;

	_int iItemType;

	if (true == bLoadCheck)
		/*For Load*/
		iItemType = iType;
	/*For Add*/
	else
		iItemType = ((CMainFrame*)AfxGetMainWnd())->GetMainForm()->GetItemTool()->GetItemType();

	Engine::CGameObject*	pGameObject = nullptr;

	pGameObject = CBack_Item::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return;

	DCAST(CBack_Item*, pGameObject)->SetItemType(iItemType);
	DCAST(CBack_Item*, pGameObject)->Scaling_Item(fX, fY, fZ);
	DCAST(CBack_Item*, pGameObject)->TransPos_Item(vPos);

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_LOGO, pLayerTag, pGameObject)))
		return;
}

void CScene_Logo::Del_Item(const _tchar * pLayerTag)
{
	m_pObject_Manager->Release_Object_Certain(SCENE_LOGO, pLayerTag);
}

void CScene_Logo::Change_Terrain(const _int & iX, const _int & iZ, const _float & fItv)
{
	CComponent* pComponent = nullptr;

	m_pObject_Manager->Release_Object_Certain(SCENE_LOGO, L"Layer_Terrain");
	CComponent_Manager::GetInstance()->Release_Component_Certain(SCENE_LOGO, L"Component_Buffer_Terrain");

	Engine::Safe_Release(m_pTargetBufferCom);

	pComponent = Engine::CTerrain_Buffer::Create(m_pGraphicDev, iX, iZ, fItv);
	if (nullptr == pComponent)
		return;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_LOGO, L"Component_Buffer_Terrain", pComponent)))
		return;
	if (FAILED(Ready_Layer_BackGround(L"Layer_Terrain")))
		return;

	SetBufferTarget();
}

const _vec3 & CScene_Logo::GetPickingPos(void)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	_vec3* pPickPos = m_pPickingCom->Picking_ToBuffer(g_hWnd, m_pTargetBufferCom);
	
	if (nullptr == pPickPos)
	{
		m_vPos = _vec3(0.f, 0.f, 0.f);
		return m_vPos;
	}

	m_vPos = *pPickPos;
	return m_vPos;
}

void CScene_Logo::SetBufferTarget(void)
{
	if (nullptr == m_pTargetBufferCom)
	{
		m_pTargetBufferCom = (Engine::CVIBuffer*)Engine::CObject_Manager::GetInstance()->Get_Component(SCENE_LOGO, L"Layer_Terrain", 0, L"Com_Buffer");
		m_pTargetBufferCom->AddRef();
	}
}

void CScene_Logo::Picking_Cube(void)
{
	//각 큐브들의 BufferCom을 받아온다
	//검사한다.
	//만약에 없다면
	//Terrain을 받아온다.

	//PickingPos
	_vec3  vPos;

	tuple<_uint, _uint, _float> Tuple = m_pTargetBufferCom->Get_VertexCntXZItv();
	_int iCubeType = ((CMainFrame*)AfxGetMainWnd())->GetMainForm()->GetCubeTool()->GetCubeType();

	_vec3* pPickPos = m_pPickingCom->Picking_ForCubeTool(g_hWnd, m_pTargetBufferCom);

	if (nullptr == pPickPos)
	return;

	vPos = *pPickPos;

	//Height
	((CMainFrame*)AfxGetMainWnd())->GetMainForm()->GetCubeTool()->UpdateData(TRUE);
	const _float& fHeight = ((CMainFrame*)AfxGetMainWnd())->GetMainForm()->GetCubeTool()->GetHeight();

	if (0.f > fHeight)
	{
		return;
	}
	else if (get<2>(Tuple) == fHeight)
	{
		vPos.y += (get<2>(Tuple) / 2.f);
	}
	else
	{
		vPos.y += fHeight - get<2>(Tuple) / 2.f;
	}

	//Cube Search
	//If alreay cube had been added just change cubetype
	const map<const _tchar*, CLayer*>* mapCube = CObject_Manager::GetInstance()->GetMapLayer();

	for (auto& Pair : mapCube[SCENE_LOGO])
	{
		if (L"Layer_Terrain" == Pair.first ||
			L"Layer_Camera" == Pair.first  ||
			!lstrcmp(L"",Pair.first))
			continue;

		///
		//check
		// Obj
		_tchar ObjectCheck[255] = L"";
		size_t NameSize = wcslen(Pair.first) + 1;
		wcscpy_s(ObjectCheck, NameSize, Pair.first);

		for (size_t i = NameSize; i >= wcslen(L"Object"); --i)
		{
			ObjectCheck[i] = 0;
		}
		if (!wcscmp(ObjectCheck, L"Object"))
			continue;
		//
		//Mon
		_tchar MonCheck[255] = L"";
		NameSize = wcslen(Pair.first) + 1;
		wcscpy_s(MonCheck, NameSize, Pair.first);

		for (size_t i = NameSize; i >= wcslen(L"Monster"); --i)
		{
			MonCheck[i] = 0;
		}

		if (!wcscmp(MonCheck, L"Monster"))
			continue;
		//
		// Item
		_tchar ItemCheck[255] = L"";
		NameSize = wcslen(Pair.first) + 1;
		wcscpy_s(ItemCheck, NameSize, Pair.first);

		for (size_t i = NameSize; i >= wcslen(L"Item"); --i)
		{
			ItemCheck[i] = 0;
		}
		if (!wcscmp(ItemCheck, L"Item"))
			continue;
		//
		///

		list<CGameObject*>* listTemp = Pair.second->GetGameObjList();
		const _vec3 &pCubePos = DCAST(CCube*, *listTemp->begin())->GetCubePos();
		
		if (vPos == pCubePos)
		{
			DCAST(CCube*, *listTemp->begin())->SetCubeType(iCubeType);
			return;
		}
	}

	
	((CMainFrame*)AfxGetMainWnd())->GetMainForm()->GetCubeTool()->AddCube_On_CubeTool(&vPos);
}

void CScene_Logo::Picking_Obj(void)
{
	//PickingPos
	_vec3  vPos;

	tuple<_uint, _uint, _float> Tuple = m_pTargetBufferCom->Get_VertexCntXZItv();
	_vec3* pPickPos = m_pPickingCom->Picking_ToBuffer(g_hWnd, m_pTargetBufferCom);

	if (nullptr == pPickPos)
		return;

	vPos = *pPickPos;

	//Height
	((CMainFrame*)AfxGetMainWnd())->GetMainForm()->GetObjectTool()->UpdateData(TRUE);
	const _float& fHeight = ((CMainFrame*)AfxGetMainWnd())->GetMainForm()->GetObjectTool()->GetHeight();

	if (0.f > fHeight)
	{
		return;
	}
	else if (get<2>(Tuple) == fHeight)
	{
		vPos.y += (get<2>(Tuple) / 2.f);
	}
	else
	{
		vPos.y += fHeight - get<2>(Tuple) / 2.f;
	}

	((CMainFrame*)AfxGetMainWnd())->GetMainForm()->GetObjectTool()->AddObj_On_ObjTool(&vPos);
}

void CScene_Logo::Picking_Mon(void)
{
	//PickingPos
	_vec3  vPos;

	tuple<_uint, _uint, _float> Tuple = m_pTargetBufferCom->Get_VertexCntXZItv();
	_vec3* pPickPos = m_pPickingCom->Picking_ToBuffer(g_hWnd, m_pTargetBufferCom);

	if (nullptr == pPickPos)
		return;

	vPos = *pPickPos;

	//Height
	((CMainFrame*)AfxGetMainWnd())->GetMainForm()->GetMonsterTool()->UpdateData(TRUE);
	const _float& fHeight = ((CMainFrame*)AfxGetMainWnd())->GetMainForm()->GetMonsterTool()->GetHeight();

	if (0.f > fHeight)
	{
		return;
	}
	else if (get<2>(Tuple) == fHeight)
	{
		vPos.y += (get<2>(Tuple) / 2.f);
	}
	else
	{
		vPos.y += fHeight - get<2>(Tuple) / 2.f;
	}

	((CMainFrame*)AfxGetMainWnd())->GetMainForm()->GetMonsterTool()->AddMon_On_MonTool(&vPos);
}

void CScene_Logo::Picking_Item(void)
{
	//PickingPos
	_vec3  vPos;

	tuple<_uint, _uint, _float> Tuple = m_pTargetBufferCom->Get_VertexCntXZItv();
	_vec3* pPickPos = m_pPickingCom->Picking_ToBuffer(g_hWnd, m_pTargetBufferCom);

	if (nullptr == pPickPos)
		return;

	vPos = *pPickPos;

	//Height
	((CMainFrame*)AfxGetMainWnd())->GetMainForm()->GetItemTool()->UpdateData(TRUE);
	const _float& fHeight = ((CMainFrame*)AfxGetMainWnd())->GetMainForm()->GetItemTool()->GetHeight();

	if (0.f > fHeight)
	{
		return;
	}
	else if (get<2>(Tuple) == fHeight)
	{
		vPos.y += (get<2>(Tuple) / 2.f);
	}
	else
	{
		vPos.y += fHeight - get<2>(Tuple) / 2.f;
	}

	((CMainFrame*)AfxGetMainWnd())->GetMainForm()->GetItemTool()->AddItem_On_ItemTool(&vPos);
}

HRESULT CScene_Logo::Change_Scene(SCENETYPE eSceneID)
{
	return NOERROR;
}

CScene_Logo * CScene_Logo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_Logo *	pInstance = new CScene_Logo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MessageBox(0, L"CScene_Logo Created Failed", nullptr, MB_OK);
		Engine::Safe_Release(pInstance);
	}
	 
	return pInstance;
}

_ulong CScene_Logo::Free(void)
{
	Engine::Safe_Release(m_pTargetBufferCom);
	Engine::Safe_Release(m_pPickingCom);

	if (FAILED(CScene_Tool::Release_Object(SCENE_LOGO)))
		MSG_BOX("Scene_Logo Release Failed");

	if (FAILED(Engine::CComponent_Manager::GetInstance()->Release_Component(SCENE_LOGO)))
		MSG_BOX("Scene_Logo Release Failed");

	return Engine::CScene_Tool::Free();
}

