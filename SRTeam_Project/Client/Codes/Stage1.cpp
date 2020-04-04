#include "stdafx.h"
#include "..\Headers\Stage1.h"


#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Input_Device.h"
#include "SceneMgr.h"
#include "Dynamic_Camera.h"
#include "Static_Camera.h"
#include "Stage2.h"
#include "Client_Terrain.h"
#include "Sky.h"
#include "Client_Player.h"
#include "Monster_Imp.h"
#include "Monster_Demon.h"
#include "Monster_Afrit.h"
#include "Monster_Baron.h"
#include "Monster_Sergeant.h"
#include "Monster_Elemental.h"
#include "Monster_NormalSordier.h"
#include "Layer.h"
#include "NomalFont.h"
#include "MiniFont.h"
#include "Face.h"
#include "Hit.h"
#include "Client_UI.h"
#include "Stage1_Boss.h"
#include "Yellow_UI.h"
#include "Monster_Deco.h"

//Mapping
#include "Wall_Cube.h"
#include "Map_Object.h"
#include "Item.h"

CStage1::CStage1(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
	, m_Dynamic_Camera(nullptr)
	, m_Static_Camera(nullptr)
	, m_pCamera(nullptr)
	, m_pPlayer(nullptr)
	, m_eSceneType(SCENE_STAGE1)
	, m_eNewSceneType(SCENE_END)
{
}

HRESULT CStage1::Ready_Scene(void)
{

	if (Ready_Stage1Component())
		return E_FAIL;

	LoadMap();

	if (Ready_Layer_Static_Camera(L"Layer_Static_Camara", &_vec3(0.f, 5.f, -5.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(90.f)))
		return E_FAIL;

	if (Ready_Layer_Player(L"Layer_Player", &_vec3(1.5f, 0.f, 2.5f), &_vec3(0.f, 0.f, 0.f)))
		return E_FAIL;

	/*if (Ready_Layer_Player(L"Layer_Player", &_vec3(63.5f, 0.f, 52.f), &_vec3(0.f, 0.f, 0.f)))
		return E_FAIL;*/



	if (FAILED(Ready_UIInfo(L"Layer_UI")))
		return E_FAIL;

	/*if (Ready_Layer_Imp(L"Layer_Monster"))
		return E_FAIL;*/

	/*if (Ready_Layer_Demon(L"Layer_Monster"))
		return E_FAIL;

	if (Ready_Layer_Afrit(L"Layer_Monster"))
		return E_FAIL;

	if (Ready_Layer_Baron(L"Layer_Monster"))
		return E_FAIL;*/

	/*if (Ready_Layer_Sergeant(L"Layer_Monster"))
		return E_FAIL;

	if (Ready_Layer_Elemental(L"Layer_Monster"))
		return E_FAIL;*/

	if (Ready_Layer_Stage1_Boss(L"Layer_Boss"))
		return E_FAIL;


	m_pObject_Manager->Set_pCamera(dynamic_cast<CCamera*>(m_pCamera));

	CSoundMgr::GetInstance()->PlayBGM(L"The_Constellarium.mp3", CSoundMgr::Channel_BGM);

	return NOERROR;
}

int CStage1::Update_Scene(const _float & fTimeDelta)
{
	//CameraChange();


	list<CGameObject*>* pMonsterList = nullptr;
	list<CGameObject*>* pBossList				= nullptr;
	list<CGameObject*>* pPlayerList				= nullptr;
	list<CGameObject*>* pBulletList				= nullptr;
	list<CGameObject*>* pMonsterBulletList		= nullptr;
	list<CGameObject*>* pWallCubeList			= nullptr;
	list<CGameObject*>* pItemList				= nullptr;

	const map<const _tchar*, CLayer*>* pMapLayer = m_pObject_Manager->GetMapLayer();

	for (auto& iter : pMapLayer[SCENE_STAGE1])
	{
		if (!lstrcmp(L"Layer_Monster", iter.first))
			pMonsterList = iter.second->GetGameObjList();
		if (!lstrcmp(L"Layer_Boss", iter.first))
			pBossList = iter.second->GetGameObjList();
		if (!lstrcmp(L"Monster_Bullet", iter.first))
			pMonsterBulletList = iter.second->GetGameObjList();
		if (!lstrcmp(L"WallCube", iter.first))
			pWallCubeList = iter.second->GetGameObjList();
		if (!lstrcmp(L"Item", iter.first))
			pItemList = iter.second->GetGameObjList();
	}

	for (auto& iter : pMapLayer[SCENE_STATIC])
	{
		if (!lstrcmp(L"Layer_Player", iter.first))
			pPlayerList = iter.second->GetGameObjList();
		if (!lstrcmp(L"Bullet", iter.first))
			pBulletList = iter.second->GetGameObjList();
	}

	if (nullptr == pPlayerList)
		return E_FAIL;

	if(nullptr != pBulletList && nullptr != pMonsterList)
		CCollisionMgr::CollisionMonsterHit_Circle(pMonsterList, pBulletList);
	if (nullptr != pBulletList && nullptr != pBossList)
		CCollisionMgr::CollisionMonsterHit_Circle(pBossList, pBulletList);

	if (nullptr != pPlayerList && nullptr != pMonsterList)
		CCollisionMgr::CollisionMonsterSearch_Circle(pPlayerList, pMonsterList);
	if (nullptr != pPlayerList && nullptr != pBossList)
		CCollisionMgr::CollisionMonsterSearch_Circle(pPlayerList, pBossList);

	if (nullptr != pPlayerList && nullptr != pBossList)
		CCollisionMgr::CollisionMonsterAtkSearch_Circle(pPlayerList, pBossList);
	if (nullptr != pPlayerList && nullptr != pMonsterList)
		CCollisionMgr::CollisionMonsterAtkSearch_Circle(pPlayerList, pMonsterList);

	if (nullptr != pPlayerList && nullptr != pMonsterBulletList)
		CCollisionMgr::CollisionPlayerHit_Circle(pPlayerList, pMonsterBulletList);

	//Cube Coll
	if(nullptr != pWallCubeList && nullptr != pPlayerList)
		CCollisionMgr::CollisionWall_Rect(pWallCubeList, pPlayerList, fTimeDelta);

	////Monster Coll
	if(nullptr != pMonsterList)
		CCollisionMgr::Collision_WithMonster_Monster_Circle(pMonsterList, pMonsterList);

	//Bullet Coll
	if(nullptr != pBulletList)
		CCollisionMgr::Collision_WithWall_Bullet_Circle(pWallCubeList, pBulletList);
	
	//Item
	if (nullptr != pItemList)
		CCollisionMgr::CollisionHitItem_Circle(pPlayerList, pItemList);

	/*if (pBossList->empty())
	{*/
		int iScene_Count = ChangeScene(SCENE_STAGE2);

		if (2 == iScene_Count)
			return 2;
	//}

	return CScene::Update_Scene(fTimeDelta);
}

void CStage1::Render_Scene(void)
{
	CScene::Render_Scene();
}

HRESULT CStage1::Ready_Stage1Component(void)
{
	Engine::CComponent*			pComponent = nullptr;

	// For.Texture_Terrain
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Stage1_Terrain/Stage3_Terrain.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STATIC, L"Component_Texture_Terrain", pComponent)))
		return E_FAIL;

	// For.Texture_Sky
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_CUBE, L"../Bin/Resource/Sky/Sky.dds");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STATIC, L"Component_Texture_Sky", pComponent)))
		return E_FAIL;

	// For.Buffer_SkyCube_Texture
	pComponent = Engine::CSky_Cube_Texture::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STATIC, L"Component_Buffer_Sky", pComponent)))
		return E_FAIL;

	//Monster_Component
	if(FAILED(Ready_Monster_Component()))
		return E_FAIL;


	return NOERROR;
}

HRESULT CStage1::Ready_LightInfo(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ((ºû.d * Àç.d * ÅØ½ºÃÄ»ö) * ¸í¾Ï) + (ºû.a * Àç.a) + (ºû.s * Àç.s) * ½ºÆåÅ§·¯ÀÇ ¼¼±â
	D3DLIGHT9			LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.f, 1.f);
	LightInfo.Ambient = LightInfo.Diffuse;
	LightInfo.Direction = _vec3(1.f, -1.f, 1.f);


	m_pGraphicDev->SetLight(0, &LightInfo);
	m_pGraphicDev->LightEnable(0, TRUE);

	return NOERROR;

}

HRESULT CStage1::Ready_UIInfo(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CClient_UI::Create(m_pGraphicDev, &_vec3(WINCX * 0.5f, 550.f, 0.f), _float(WINCX), _float(100));
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	//m_pPlayer->get
	pGameObject = CYellow_UI::Create(m_pGraphicDev, &_vec3(WINCX * 0.5f, WINCY * 0.415f, 0.f), _float(WINCX), _float(500.f), nullptr, 0);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, L"Player_Earn_UI", pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	//m_pPlayer->hit
	pGameObject = CHit::Create(m_pGraphicDev, &_vec3(WINCX * 0.5f, WINCY * 0.415f, 0.f), _float(WINCX), _float(500.f), nullptr, 0);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, L"Player_Hit_UI", pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	// AMMO ---------------------------------------------------------------------------------------------------------------------------------------------------------------
	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::AMMO_THREE);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::AMMO_TWO);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::AMMO_ONE);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	// Health ---------------------------------------------------------------------------------------------------------------------------------------------------------------

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::HEALTH_FOUR);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::HEALTH_THREE);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::HEALTH_TWO);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::HEALTH_ONE);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	// Face ---------------------------------------------------------------------------------------------------------------------------------------------------------------

	pGameObject = CFace::Create(m_pGraphicDev, &_vec3(402.f, 554.f, 0.f), _float(225), _float(250), m_pPlayer);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;
	
	// ARMOR ---------------------------------------------------------------------------------------------------------------------------------------------------------------

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::ARMOR_FOUR);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::ARMOR_THREE);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::ARMOR_TWO);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::ARMOR_ONE);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	// Shotgun ---------------------------------------------------------------------------------------------------------------------------------------------------------------

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::SHOTGUN_THREE);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::SHOTGUN_TWO);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::SHOTGUN_ONE);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;



	// MAXShotgun ---------------------------------------------------------------------------------------------------------------------------------------------------------------

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::MAXSHOTGUN_THREE);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::MAXSHOTGUN_TWO);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::MAXSHOTGUN_ONE);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	// ChainGun ---------------------------------------------------------------------------------------------------------------------------------------------------------------

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::CHAINGUN_THREE);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::CHAINGUN_TWO);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::CHAINGUN_ONE);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	// MaxChainGun ---------------------------------------------------------------------------------------------------------------------------------------------------------------

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::MAXCHAINGUN_THREE);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::MAXCHAINGUN_TWO);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::MAXCHAINGUN_ONE);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	// PlasmaGun ---------------------------------------------------------------------------------------------------------------------------------------------------------------

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::PLASMAGUN_THREE);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::PLASMAGUN_TWO);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::PLASMAGUN_ONE);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	// MaxPlasmaGun ---------------------------------------------------------------------------------------------------------------------------------------------------------------

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::MAXPLASMAGUN_THREE);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::MAXPLASMAGUN_TWO);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::MAXPLASMAGUN_ONE);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	// BFG ---------------------------------------------------------------------------------------------------------------------------------------------------------------

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::BFG_THREE);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::BFG_TWO);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::BFG_ONE);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	// MAXBFG ---------------------------------------------------------------------------------------------------------------------------------------------------------------

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::MAXBFG_THREE);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::MAXBFG_TWO);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;

	pGameObject = CNomalFont::Create(m_pGraphicDev, m_pPlayer, CNomalFont::MAXBFG_ONE);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;
	pGameObject = nullptr;


	return NOERROR;
}

HRESULT CStage1::Ready_Layer_Dynamic_Camera(const _tchar * pLayerTag, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*		m_Dinamic_Camera = nullptr;

	// For.Camera
	m_Dinamic_Camera = CDynamic_Camera::Create(m_pGraphicDev, pEye, pAt, pUp);
	if (nullptr == m_Dinamic_Camera)
	return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE1, pLayerTag, m_Dinamic_Camera)))
	return E_FAIL;

	return NOERROR;
}

HRESULT CStage1::Ready_Layer_Static_Camera(const _tchar * pLayerTag, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float& fFovY)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*		m_Static_Camera = nullptr;

	// For.Camera
	m_Static_Camera = CStatic_Camera::Create(m_pGraphicDev, pEye, pAt, pUp, fFovY);
	if (nullptr == m_Static_Camera)
		return E_FAIL;

	m_pCamera = m_Static_Camera;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, m_Static_Camera)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CStage1::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*	pGameObject = nullptr;

	// For.Terrain
	pGameObject = CClient_Terrain::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;

	// For.Sky
	pGameObject = CSky::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CStage1::Ready_Layer_Player(const _tchar * pLayertag, const _vec3* vPosition, const _vec3* vLook)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	// Player
	pGameObject = Client_Player::Create(m_pGraphicDev, vPosition, vLook);
	m_pPlayer = pGameObject;
	m_pPlayer->AddRef();
	
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayertag, pGameObject)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CStage1::Ready_Layer_Imp(const _tchar * pLayertag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMonster_Imp::Create(m_pGraphicDev, &_vec3(10.f, 0.f, 15.f), SCENE_STAGE1);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE1, pLayertag, pGameObject)))
		return E_FAIL;


	pGameObject = CMonster_Imp::Create(m_pGraphicDev, &_vec3(15.f, 0.f, 15.f), SCENE_STAGE1);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE1, pLayertag, pGameObject)))
		return E_FAIL;


	pGameObject = CMonster_Imp::Create(m_pGraphicDev, &_vec3(20.f, 0.f, 15.f), SCENE_STAGE1);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE1, pLayertag, pGameObject)))
		return E_FAIL;



	return NOERROR;
}

HRESULT CStage1::Ready_Layer_Demon(const _tchar * pLayertag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMonster_Demon::Create(m_pGraphicDev, &_vec3(5.f, 0.f, 15.f), SCENE_STAGE1);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE1, pLayertag, pGameObject)))
		return E_FAIL;


	return NOERROR;
}

HRESULT CStage1::Ready_Layer_Afrit(const _tchar * pLayertag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	//for (size_t i = 0; i < 5; i++)
	//{
		/*pGameObject = CMonster_Afrit::Create(m_pGraphicDev, &_vec3(_float(rand() % 50), 0.f, _float(rand() % 50)));
		if (nullptr == pGameObject)
			return E_FAIL;*/

	pGameObject = CMonster_Afrit::Create(m_pGraphicDev, &_vec3(7.f, 0.f, 15.f), SCENE_STAGE1);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE1, pLayertag, pGameObject)))
			return E_FAIL;
	

	return NOERROR;
}

HRESULT CStage1::Ready_Layer_Baron(const _tchar * pLayertag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMonster_Baron::Create(m_pGraphicDev, &_vec3(10.f, 0.f, 15.f), SCENE_STAGE1);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE1, pLayertag, pGameObject)))
		return E_FAIL;


	return NOERROR;
}

HRESULT CStage1::Ready_Layer_Sergeant(const _tchar * pLayertag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMonster_Sergeant::Create(m_pGraphicDev, &_vec3(10.f, 0.f, 15.f), SCENE_STAGE1);
	if (nullptr == pGameObject)
		return E_FAIL;

	pGameObject->Set_Scale(&_vec3(0.4f, 0.4f, 0.4f));

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE1, pLayertag, pGameObject)))
		return E_FAIL;


	return NOERROR;
}

HRESULT CStage1::Ready_Layer_Elemental(const _tchar * pLayertag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMonster_NormalSordier::Create(m_pGraphicDev, &_vec3(8.f, 0.f, 14.f), SCENE_STAGE1);
	if (nullptr == pGameObject)
		return E_FAIL;

	pGameObject->Set_Scale(&_vec3(0.4f, 0.4f, 0.4f));

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE1, pLayertag, pGameObject)))
		return E_FAIL;


	return NOERROR;
}

HRESULT CStage1::Ready_Layer_NormalSordier(const _tchar * pLayertag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMonster_Elemental::Create(m_pGraphicDev, &_vec3(9.f, 0.f, 16.f), SCENE_STAGE1);
	if (nullptr == pGameObject)
		return E_FAIL;

	pGameObject->Set_Scale(&_vec3(0.4f, 0.4f, 0.4f));

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE1, pLayertag, pGameObject)))
		return E_FAIL;
	


	return NOERROR;
}

HRESULT CStage1::Ready_Layer_Stage1_Boss(const _tchar * pLayertag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CStage1_Boss::Create(m_pGraphicDev, &_vec3(63.f, 0.f, 62.f), SCENE_STAGE1);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE1, pLayertag, pGameObject)))
		return E_FAIL;

	return NOERROR;
}

int CStage1::ChangeScene(SCENETYPE eSceneID)
{
	if (m_pInput_Device->Get_DIKeyState(DIK_RETURN) & 0x80)
	{
		if (FAILED(CScene::Release_Object(SCENE_STAGE1)))
			MSG_BOX("CStage1 Release Failed");

		if (FAILED(Engine::CComponent_Manager::GetInstance()->Release_Component(SCENE_STAGE1)))
			MSG_BOX("CStage1_Component Release Failed");


		CScene*			pNewScene = nullptr;

		switch (eSceneID)
		{
		case SCENE_STAGE2:
			pNewScene = CStage2::Create(m_pGraphicDev, m_pPlayer);
			
			if (nullptr == pNewScene)
				return E_FAIL;

			m_eNewSceneType = SCENE_STAGE2;
		 	break;
		}
		if (FAILED(CSceneMgr::GetInstance()->SetUp_CurrentScene(pNewScene)))
			return E_FAIL;


		return 2;
	}

	return 0;
}

void CStage1::CameraChange(void)
{

	if (m_pInput_Device->Get_DIKeyState(DIK_1) & 0x80)
	{
		dynamic_cast<CStatic_Camera*>(m_Static_Camera)->m_CameraOn = true;
		dynamic_cast<CDynamic_Camera*>(m_Dynamic_Camera)->m_CameraOn = false;
	}
	if (m_pInput_Device->Get_DIKeyState(DIK_2) & 0x80)
	{
		dynamic_cast<CStatic_Camera*>(m_Static_Camera)->m_CameraOn = false;
		dynamic_cast<CDynamic_Camera*>(m_Dynamic_Camera)->m_CameraOn = true;
	}
}

HRESULT CStage1::Ready_Monster_Component(void)
{
	Engine::CComponent*			pComponent = nullptr;

	// Monster_Imp ----------------------------------------------------------------------------------------------------------------------------------------------------------

	// for.Monster_Imp_Walk_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Imp/Walk/Walk_Imp%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Imp_Walk", pComponent)))
		return E_FAIL;
	// for.Monster_Imp_Walk_Left_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Imp/Walk_Left/Walk_Left_Imp%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Imp_Walk_Left", pComponent)))
		return E_FAIL;
	// for.Monster_Imp_Walk_Right_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Imp/Walk_Right/Walk_Right_Imp%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Imp_Walk_Right", pComponent)))
		return E_FAIL;

	// for.Monster_Imp_Hit_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Imp/Hit/Hit_Imp%d.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Imp_Hit", pComponent)))
		return E_FAIL;

	// for.Monster_Imp_Atk_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Imp/Attack/Attack_Imp%d.png", 3);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Imp_Atk", pComponent)))
		return E_FAIL;

	// for.Monster_Imp_Death_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Imp/Death/Death_Imp%d.png", 7);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Imp_Die", pComponent)))
		return E_FAIL;

	// for.Monster_Imp_Bullet_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster_Bullet/Monster_Imp_Bullet/Monster_Imp_Bullet%d.png", 7);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Imp_Bullet", pComponent)))
		return E_FAIL;

	// Monster_Demon ----------------------------------------------------------------------------------------------------------------------------------------------------------


	// for.Monster_Demon_Walk_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Demon/Walk/Walk_Demon%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Demon_Walk", pComponent)))
		return E_FAIL;

	// for.Monster_Demon_Hit_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Demon/Hit/Hit_Demon%d.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Demon_Hit", pComponent)))
		return E_FAIL;

	// for.Monster_Demon_Atk_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Demon/Attack/Attack_Demon%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Demon_Atk", pComponent)))
		return E_FAIL;

	// for.Monster_Demon_Death_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Demon/Death/Death_Demon%d.png", 5);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Demon_Die", pComponent)))
		return E_FAIL;

	//// for.Monster_Demon_Bullet_Texture
	//pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster_Bullet/Monster_Demon/Monster_Demon_Bullet%d.png", 7);
	//if (nullptr == pComponent)
	//	return E_FAIL;
	//if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Demon_Bullet", pComponent)))
	//	return E_FAIL;

	// Monster_Afrit ----------------------------------------------------------------------------------------------------------------------------------------------------------

	// for.Monster_Afrit_Texture_Walk
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Afrit/idle/Afrit_idle%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Afrit_Walk", pComponent)))
		return E_FAIL;

	// for.Monster_Afrit_Texture_Atk1
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Afrit/Attact/Afrit_Almost_Attact%d.png", 3);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Afrit_Almost_Atk", pComponent)))
		return E_FAIL;

	// for.Monster_Afrit_Texture_Atk2
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Afrit/Attact/Afrit_Distance_Attact%d.png", 3);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Afrit_Distance_Atk", pComponent)))
		return E_FAIL;

	// for.Monster_Afrit_Texture_Die
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Afrit/Death/Afrit_Death%d.png", 5);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Afrit_Die", pComponent)))
		return E_FAIL;

	// for.Monster_Afrit_Texture_Hit
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Afrit/Hit/Afrit_Hit.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Afrit_Hit", pComponent)))
		return E_FAIL;

	// for.Monster_Afrit_Bullet
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Afrit/Bullet/Afrit_Bullet%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Afrit_Bullet", pComponent)))
		return E_FAIL;

	// Monster_Baron ----------------------------------------------------------------------------------------------------------------------------------------------------------


	// for.Monster_Baron_Texture_Walk
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Baron/Walk/Walk_Baron%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Baron_Walk", pComponent)))
		return E_FAIL;

	// for.Monster_Baron_Texture_Atk
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Baron/Attack/Attack_Baron%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Baron_Atk", pComponent)))
		return E_FAIL;

	// for.Monster_Baron_Texture_Hit
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Baron/Hit/Hit_Baron0.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Baron_Hit", pComponent)))
		return E_FAIL;

	// for.Monster_Baron_Texture_Die
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Baron/Death/Death_Baron%d.png", 6);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Baron_Die", pComponent)))
		return E_FAIL;

	// for.Monster_Baron_Texture_BUllet
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Baron/Bullet/Baron_Bullet0.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Baron_Bullet", pComponent)))
		return E_FAIL;

	// Monster_Sergeant ----------------------------------------------------------------------------------------------------------------------------------------------------------

	// for.Monster_Sergeamt_Walk
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Sergeant/Walk/Sergeant_Walk%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Sergeant_Walk", pComponent)))
		return E_FAIL;

	// for.Monster_Sergeamt_Attack
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Sergeant/Attack/Sergeant_Attack%d.png", 2);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Sergeant_Attack", pComponent)))
		return E_FAIL;

	// for.Monster_Sergeamt_Death
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Sergeant/Death/Sergeant_Dead%d.png", 9);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Sergeant_Death", pComponent)))
		return E_FAIL;

	// for.Monster_Sergeamt_hit
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Sergeant/Hit/Sergeant_Hit.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Sergeant_Hit", pComponent)))
		return E_FAIL;

	// Monster_Elemental ----------------------------------------------------------------------------------------------------------------------------------------------------------
	
	// for.Monster_Elemental_Walk
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Elemental/Walk/Elemental_Walk%d.png", 3);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Elemental_Walk", pComponent)))
		return E_FAIL;

	// for.Monster_Elemental_Attack
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Elemental/Attack/Elemental_Attack%d.png", 3);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Elemental_Attack", pComponent)))
		return E_FAIL;

	// for.Monster_Elemental_Death
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Elemental/Death/Elemental_Death%d.png", 6);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Elemental_Death", pComponent)))
		return E_FAIL;

	// for.Monster_Elemental_Hit
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Elemental/Hit/Elemental_Hit.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Elemental_Hit", pComponent)))
		return E_FAIL;

	// for.Monster_Elemental_Bullet
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster_Bullet/Monster_Elemental_Bullet/Elemental_Bullet%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Elemental_Bullet", pComponent)))
		return E_FAIL;
	
	// Monster_NormalSordier ----------------------------------------------------------------------------------------------------------------------------------------------------------

	// for.Monster_NormalSordier_Walk
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Normal_Sordier/Walk/Walk_NormalArmy%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Walk_NormalArmy", pComponent)))
		return E_FAIL;

	// for.Monster_NormalArmy_Attack
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Normal_Sordier/Attack/NormalArmy_Attack_%d.png", 3);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_NormalArmy_Attack", pComponent)))
		return E_FAIL;

	// for.Monster_NormalSordier_Death
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Normal_Sordier/Death/NormalArmy_Death_%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_NormalArmy_Death", pComponent)))
		return E_FAIL;

	// for.Monster_NormalSordier_Hit
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Normal_Sordier/Hit/NormalArmy_Hit.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_NormalArmy_Hit", pComponent)))
		return E_FAIL;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// for.Monster_Deco_Texture_Walk
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Cacodemon/idle/Walk_Cacodemon%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Deco_Walk", pComponent)))
		return E_FAIL;

	// for.Monster_Deco_Texture_Atk
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Cacodemon/Attack/Cacodemon_Attact%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Deco_Atk", pComponent)))
		return E_FAIL;

	// for.Monster_Deco_Texture_Die
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Cacodemon/Death/Cacodemon_Death%d.png", 6);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Deco_Die", pComponent)))
		return E_FAIL;

	// for.Monster_Deco_Texture_Die
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Cacodemon/Bullet/BlueFire%d.png", 12);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Deco_Bullet", pComponent)))
		return E_FAIL;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// for.Stage1_Boss_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Boss/Stage1_Boss/Walk/Walk_Boss%d.png", 4);
	if (nullptr == pComponent) 
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Stage1Boss_Walk", pComponent)))
		return E_FAIL;

	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Boss/Stage1_Boss/Attack/Attack_Boss%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Stage1Boss_Atk", pComponent)))
		return E_FAIL;

	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Boss/Stage1_Boss/Death/Death_Boss%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Stage1Boss_Die", pComponent)))
		return E_FAIL;

	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Boss/Stage1_Boss/Bullet/Bullet%d.png", 8);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Stage1Boss_Bullet", pComponent)))
		return E_FAIL;

	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Effect/Boss_Hit/Blood/Blood%d.png", 9);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Stage1Boss_Blood", pComponent)))
		return E_FAIL;

	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Boss_Bullet/Boss_Bullet1/Boss_Bullet1_%d.png", 16);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Stage1Boss_Bullet1", pComponent)))
		return E_FAIL;

	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Boss_Bullet/Boss_Bullet2/Boss_Bullet2_%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Stage1Boss_Bullet2", pComponent)))
		return E_FAIL;

	return NOERROR;
}

void CStage1::LoadMap(void)
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	//Map
	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(
		L"../Bin/DataFiles/Stage1/Stage1.dat",
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	//Terrain

	TERRAIN_DATA* tempData_Terrain = new TERRAIN_DATA;

	ReadFile(hFile, tempData_Terrain, sizeof(TERRAIN_DATA), &dwByte, NULL);

	Change_Terrain(tempData_Terrain->iX, tempData_Terrain->iZ, tempData_Terrain->iItv);

	//Cube
	while (true)
	{
		CUBE_DATA*	tempData_Cube = new CUBE_DATA;

		ReadFile(hFile, tempData_Cube, sizeof(CUBE_DATA), &dwByte, NULL);

		if (dwByte == 0)
		{
			Engine::Safe_Delete(tempData_Cube);
			break;
		}

		Add_WallCube(&tempData_Cube->vPos, &tempData_Cube->vScale, tempData_Cube->iType, tempData_Cube->bDoorCheck);
	}

	CloseHandle(hFile);
	/////////////////////////////////////////////////////////////////////////////////////////////
	//Object
	dwByte = 0;

	hFile = CreateFile(
		L"../Bin/DataFiles/Stage1/Obj.dat",
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	while (true)
	{
		OBJ_DATA*	tempData_Obj = new OBJ_DATA;

		ReadFile(hFile, tempData_Obj, sizeof(OBJ_DATA), &dwByte, NULL);

		if (dwByte == 0)
		{
			Engine::Safe_Delete(tempData_Obj);
			break;
		}

		Add_Obj(&tempData_Obj->vPos, &tempData_Obj->vScale, tempData_Obj->iType);
	}

	CloseHandle(hFile);

	/////////////////////////////////////////////////////////////////////////////////////////////
	//Item
	dwByte = 0;

	hFile = CreateFile(
		L"../Bin/DataFiles/Stage1/Item.dat",
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	while (true)
	{
		ITEM_DATA*	tempData_Item = new ITEM_DATA;

		ReadFile(hFile, tempData_Item, sizeof(ITEM_DATA), &dwByte, NULL);

		if (dwByte == 0)
		{
			Engine::Safe_Delete(tempData_Item);
			break;
		}

		Add_Item(&tempData_Item->vPos, &tempData_Item->vScale, tempData_Item->iType);
	}

	CloseHandle(hFile);

	/////////////////////////////////////////////////////////////////////////////////////////////
	//Monster
	dwByte = 0;

	hFile = CreateFile(
		L"../Bin/DataFiles/Stage1/Mon.dat",
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	while (true)
	{
		MON_DATA*	tempData_Mon = new MON_DATA;

		ReadFile(hFile, tempData_Mon, sizeof(MON_DATA), &dwByte, NULL);

		if (dwByte == 0)
		{
			Engine::Safe_Delete(tempData_Mon);
			break;
		}

		Add_Monster(L"Layer_Monster", &tempData_Mon->vPos, &tempData_Mon->vScale, tempData_Mon->iType);
	}

	CloseHandle(hFile);
}

void CStage1::Change_Terrain(const _int & iX, const _int & iZ, const _float & fItv)
{
	CComponent* pComponent = nullptr;

	pComponent = Engine::CTerrain_Buffer::Create(m_pGraphicDev, iX, iZ, fItv);
	if (nullptr == pComponent)
		return;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STATIC, L"Component_Buffer_Terrain", pComponent)))
		return;
	if (FAILED(Ready_Layer_Terrain(L"Layer_Terrain")))
		return;
}

void CStage1::Add_WallCube(const _vec3 * vPos, const _vec3 * vScale, const _int& iType, const _bool& iDoorCheck)
{
	if (nullptr == m_pObject_Manager)
		return;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CWall_Cube::Create(m_pGraphicDev);

	if (nullptr == pGameObject)
		return;

	DCAST(CWall_Cube*, pGameObject)->SetCubeType(iType);
	DCAST(CWall_Cube*, pGameObject)->SetDoorCheck(iDoorCheck);

	if (true == DCAST(CWall_Cube*, pGameObject)->GetDoorCheck())
	{
		if (true == DCAST(CWall_Cube*, pGameObject)->GetDoorXZCheck())
		{
			//Z
			DCAST(CWall_Cube*, pGameObject)->Scaling_Cube(vScale->x  * 0.6f, vScale->y, vScale->z);
		}
		else
		{
			//X
			DCAST(CWall_Cube*, pGameObject)->Scaling_Cube(vScale->x, vScale->y, vScale->z * 0.6f);
		}
	}
	else
	{
		DCAST(CWall_Cube*, pGameObject)->Scaling_Cube(vScale->x, vScale->y, vScale->z);
	}
	DCAST(CWall_Cube*, pGameObject)->TransPos_Cube(vPos);

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE1, L"WallCube", pGameObject)))
		return;
}

void CStage1::Add_Obj(const _vec3 * vPos, const _vec3 * vScale, const _int & iType)
{
	if (nullptr == m_pObject_Manager)
		return;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMap_Object::Create(m_pGraphicDev);

	if (nullptr == pGameObject)
		return;

	DCAST(CMap_Object*, pGameObject)->SetObjType(iType);
	DCAST(CMap_Object*, pGameObject)->Scaling_Obj(vScale->x, vScale->y, vScale->z);
	DCAST(CMap_Object*, pGameObject)->TransPos_Obj(vPos);

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE1, L"Object", pGameObject)))
		return;
}

void CStage1::Add_Item(const _vec3 * vPos, const _vec3 * vScale, const _int & iType)
{
	if (nullptr == m_pObject_Manager)
		return;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CItem::Create(m_pGraphicDev);

	if (nullptr == pGameObject)
		return;

	DCAST(CItem*, pGameObject)->SetItemType(iType);
	DCAST(CItem*, pGameObject)->Scaling_Item(vScale->x, vScale->y, vScale->z);
	DCAST(CItem*, pGameObject)->TransPos_Item(vPos);

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE1, L"Item", pGameObject)))
		return;
}

void CStage1::Add_Monster(const _tchar * tagLayer, const _vec3 * vPos, const _vec3 * vScale, const _int & iType)
{
	switch (iType)
	{
	case MONSTER_IMP:
		Add_Imp(tagLayer, vPos, vScale);
		break;
	case MONSTER_DEMON:
		Add_Demon(tagLayer, vPos, vScale);
		break;
	case MONSTER_DECO:
		Add_Deco(tagLayer, vPos, vScale);
		break;
	case MONSTER_AFRIT:
		Add_Afrit(tagLayer, vPos, vScale);
		break;
	case MONSTER_BARON:
		Add_Baron(tagLayer, vPos, vScale);
		break;
	case MONSTER_SEGERENT:
		Add_Segerent(tagLayer, vPos, vScale);
		break;
	case MONSTER_ELEMENT:
		Add_Element(tagLayer, vPos, vScale);
		break;
	case MONSTER_NORM_SOLDIER:
		Add_NormSoldier(tagLayer, vPos, vScale);
		break;
	}
}

void CStage1::Add_Imp(const _tchar * tagLayer, const _vec3 * vPos, const _vec3 * vScale)
{
	if (nullptr == m_pObject_Manager)
		return;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMonster_Imp::Create(m_pGraphicDev, vPos, SCENE_STAGE1);

	if (nullptr == pGameObject)
		return;

	pGameObject->Set_Scale(vScale);

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE1, tagLayer, pGameObject)))
		return;
}

void CStage1::Add_Demon(const _tchar * tagLayer, const _vec3 * vPos, const _vec3 * vScale)
{
	if (nullptr == m_pObject_Manager)
		return;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMonster_Demon::Create(m_pGraphicDev, vPos, SCENE_STAGE1);

	if (nullptr == pGameObject)
		return;

	pGameObject->Set_Scale(vScale);

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE1, tagLayer, pGameObject)))
		return;
}

void CStage1::Add_Deco(const _tchar * tagLayer, const _vec3 * vPos, const _vec3 * vScale)
{
	if (nullptr == m_pObject_Manager)
		return;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMonster_Deco::Create(m_pGraphicDev, vPos, SCENE_STAGE1);

	if (nullptr == pGameObject)
		return;

	pGameObject->Set_Scale(vScale);

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE1, tagLayer, pGameObject)))
		return;
}

void CStage1::Add_Afrit(const _tchar * tagLayer, const _vec3 * vPos, const _vec3 * vScale)
{
	if (nullptr == m_pObject_Manager)
		return;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMonster_Afrit::Create(m_pGraphicDev, vPos, SCENE_STAGE1);

	if (nullptr == pGameObject)
		return;

	pGameObject->Set_Scale(vScale);

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE1, tagLayer, pGameObject)))
		return;
}

void CStage1::Add_Baron(const _tchar * tagLayer, const _vec3 * vPos, const _vec3 * vScale)
{
	if (nullptr == m_pObject_Manager)
		return;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMonster_Baron::Create(m_pGraphicDev, vPos, SCENE_STAGE1);

	if (nullptr == pGameObject)
		return;

	pGameObject->Set_Scale(vScale);

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE1, tagLayer, pGameObject)))
		return;
}

void CStage1::Add_Segerent(const _tchar * tagLayer, const _vec3 * vPos, const _vec3 * vScale)
{
	if (nullptr == m_pObject_Manager)
		return;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMonster_Sergeant::Create(m_pGraphicDev, vPos, SCENE_STAGE1);

	if (nullptr == pGameObject)
		return;

	pGameObject->Set_Scale(vScale);

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE1, tagLayer, pGameObject)))
		return;
}

void CStage1::Add_Element(const _tchar * tagLayer, const _vec3 * vPos, const _vec3 * vScale)
{
	if (nullptr == m_pObject_Manager)
		return;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMonster_Elemental::Create(m_pGraphicDev, vPos, SCENE_STAGE1);

	if (nullptr == pGameObject)
		return;

	pGameObject->Set_Scale(vScale);

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE1, tagLayer, pGameObject)))
		return;
}

void CStage1::Add_NormSoldier(const _tchar * tagLayer, const _vec3 * vPos, const _vec3 * vScale)
{
	if (nullptr == m_pObject_Manager)
		return;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMonster_NormalSordier::Create(m_pGraphicDev, vPos, SCENE_STAGE1);

	if (nullptr == pGameObject)
		return;

	pGameObject->Set_Scale(vScale);

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE1, tagLayer, pGameObject)))
		return;
}



CStage1* CStage1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage1 *	pInstance = new CStage1(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MessageBox(0, L"CStage1 Created Failed", nullptr, MB_OK);
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CStage1::Free(void)
{
	/*if (SCENE_STAGE2 != m_eNewSceneType)
	{
		DCAST(Client_Player*, CObject_Manager::GetInstance()->Get_GameObject(SCENE_STATIC, L"Layer_Player", 0))->Free();
		if(FAILED(CScene::Release_Object(SCENE_STATIC)))
			MSG_BOX("STATIC Release Failed");
	}*/

	Engine::Safe_Release(m_pPlayer);

	if (FAILED(CScene::Release_Object(SCENE_STAGE1)))
		MSG_BOX("CStage1 Release Failed");

	if (FAILED(Engine::CComponent_Manager::GetInstance()->Release_Component(SCENE_STAGE1)))
		MSG_BOX("CStage1_Component Release Failed");


	return CScene::Free();
}
