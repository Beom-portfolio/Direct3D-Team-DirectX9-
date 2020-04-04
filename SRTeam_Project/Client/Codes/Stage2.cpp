#include "stdafx.h"
#include "..\Headers\Stage2.h"

#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Input_Device.h"
#include "SceneMgr.h"
#include "Static_Camera.h"
#include "Stage3.h"
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
#include "Stage2_Boss.h"

//Mapping
#include "Wall_Cube.h"
#include "Map_Object.h"
#include "Item.h"

CStage2::CStage2(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pPlayer)
	:CScene(pGraphicDev)
	, m_Static_Camera(nullptr)
	, m_eSceneType(SCENE_STAGE2)
	, m_bSetup_StartPos(true)
{
	m_pPlayer = pPlayer;
}

HRESULT CStage2::Ready_Scene(void)
{
	if (Ready_Stage2Component())
		return E_FAIL;

	LoadMap();

	dynamic_cast<CTransform*>(m_pPlayer->Get_Component(L"Transform_com"))->Set_Infomation(CTransform::INFO_POSITION, &_vec3(64.5f, 0.f, 1.5f));
	//dynamic_cast<CTransform*>(m_pPlayer->Get_Component(L"Transform_com"))->Set_Infomation(CTransform::INFO_POSITION, &_vec3(74.4f, 0.f, 43.f));
	
	/*if (FAILED(Ready_Layer_Player(&_vec3(64.5f, 0.f, 1.5f), &_vec3(0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Static_Camera(L"Layer_Static_Camara", &_vec3(0.f, 5.f, -5.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(90.f))))
		return E_FAIL;*/

	/*if (FAILED(Ready_Layer_Imp(L"Layer_Monster")))
		return E_FAIL;*/

	if (Ready_Layer_Stage2_Boss(L"Layer_Boss"))
		return E_FAIL;

	CSoundMgr::GetInstance()->StopAll();
	CSoundMgr::GetInstance()->PlayBGM(L"Stage2.mp3", CSoundMgr::Channel_BGM);
	return NOERROR;
}

int CStage2::Update_Scene(const _float & fTimeDelta)
{
	list<CGameObject*>* pMonsterList = nullptr;
	list<CGameObject*>* pPlayerList = nullptr;
	list<CGameObject*>* pBulletList = nullptr;
	list<CGameObject*>* pMonsterBulletList = nullptr;
	list<CGameObject*>* pWallCubeList = nullptr; 
	list<CGameObject*>* pItemList = nullptr;

	list<CGameObject*>* pBossList = nullptr;

	const map<const _tchar*, CLayer*>* pMapLayer = m_pObject_Manager->GetMapLayer();

	for (auto& iter : pMapLayer[SCENE_STAGE2])
	{
		if (!lstrcmp(L"Layer_Monster", iter.first))
			pMonsterList = iter.second->GetGameObjList();

		if (!lstrcmp(L"Item", iter.first))
			pItemList = iter.second->GetGameObjList();

		if (!lstrcmp(L"Bullet", iter.first))
			pBulletList = iter.second->GetGameObjList();

		if (!lstrcmp(L"WallCube", iter.first))
			pWallCubeList = iter.second->GetGameObjList();

		if (!lstrcmp(L"Layer_Boss", iter.first))
			pBossList = iter.second->GetGameObjList();

		if (!lstrcmp(L"Monster_Bullet", iter.first))
			pMonsterBulletList = iter.second->GetGameObjList();
	}

	for (auto& iter : pMapLayer[SCENE_STATIC])
	{
		if (!lstrcmp(L"Bullet", iter.first))
			pBulletList = iter.second->GetGameObjList();

		if (!lstrcmp(L"Layer_Player", iter.first))
		{
			pPlayerList = iter.second->GetGameObjList();
			break;
		}
	}

	if (nullptr == pPlayerList)
		return E_FAIL;

	if (nullptr != pPlayerList && nullptr != pBossList)
		CCollisionMgr::CollisionMonsterSearch_Circle(pPlayerList, pBossList);
	if (nullptr != pPlayerList && nullptr != pBossList)
		CCollisionMgr::CollisionMonsterAtkSearch_Circle(pPlayerList, pBossList);
	if (nullptr != pBulletList && nullptr != pBossList)
		CCollisionMgr::CollisionMonsterHit_Circle(pBossList, pBulletList);
	if (nullptr != pPlayerList && nullptr != pMonsterBulletList)
		CCollisionMgr::CollisionPlayerHit_Circle(pPlayerList, pMonsterBulletList);


	if (nullptr != pBulletList && nullptr != pMonsterList)
		CCollisionMgr::CollisionMonsterHit_Circle(pMonsterList, pBulletList);


	if (nullptr != pPlayerList && nullptr != pMonsterList)
		CCollisionMgr::CollisionMonsterSearch_Circle(pPlayerList, pMonsterList);
	if (nullptr != pPlayerList && nullptr != pMonsterList)
		CCollisionMgr::CollisionMonsterAtkSearch_Circle(pPlayerList, pMonsterList);

	//Cube Coll
	if (nullptr != pWallCubeList && nullptr != pPlayerList)
		CCollisionMgr::CollisionWall_Rect(pWallCubeList, pPlayerList, fTimeDelta);

	////Monster Coll
	if(nullptr != pMonsterList)
		CCollisionMgr::Collision_WithMonster_Monster_Circle(pMonsterList, pMonsterList);

	//Bullet Coll
	if (nullptr != pBulletList)
		CCollisionMgr::Collision_WithWall_Bullet_Circle(pWallCubeList, pBulletList);

	//Item
	if (nullptr != pItemList)
		CCollisionMgr::CollisionHitItem_Circle(pPlayerList, pItemList);


	if (CInput_Device::GetInstance()->Get_DIKeyState(DIK_P) & 0x80)
	{
		CSoundMgr::GetInstance()->StopAll();
		CSoundMgr::GetInstance()->PlayBGM(L"Stage2.mp3", CSoundMgr::Channel_BGM);
	}


	return CScene::Update_Scene(fTimeDelta);


}

void CStage2::Render_Scene(void)
{
	CScene::Render_Scene();
}

HRESULT CStage2::Ready_Stage2Component(void)
{
	Engine::CComponent*			pComponent = nullptr;

	// For.Texture_Terrain


	//Monster_Component
	if (FAILED(Ready_MonsterComponent()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CStage2::Ready_MonsterComponent(void)
{
	Engine::CComponent*			pComponent = nullptr;


	// for.Monster_Imp_Walk_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Imp/Walk/Walk_Imp%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Imp_Walk", pComponent)))
		return E_FAIL;
	// for.Monster_Imp_Walk_Left_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Imp/Walk_Left/Walk_Left_Imp%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Imp_Walk_Left", pComponent)))
		return E_FAIL;
	// for.Monster_Imp_Walk_Right_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Imp/Walk_Right/Walk_Right_Imp%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Imp_Walk_Right", pComponent)))
		return E_FAIL;

	// for.Monster_Imp_Hit_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Imp/Hit/Hit_Imp%d.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Imp_Hit", pComponent)))
		return E_FAIL;

	// for.Monster_Imp_Atk_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Imp/Attack/Attack_Imp%d.png", 3);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Imp_Atk", pComponent)))
		return E_FAIL;

	// for.Monster_Imp_Death_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Imp/Death/Death_Imp%d.png", 7);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Imp_Die", pComponent)))
		return E_FAIL;

	// for.Monster_Imp_Bullet_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster_Bullet/Monster_Imp_Bullet/Monster_Imp_Bullet%d.png", 7);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Imp_Bullet", pComponent)))
		return E_FAIL;



	// for.Monster_Demon_Walk_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Demon/Walk/Walk_Demon%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Demon_Walk", pComponent)))
		return E_FAIL;

	// for.Monster_Demon_Hit_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Demon/Hit/Hit_Demon%d.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Demon_Hit", pComponent)))
		return E_FAIL;

	// for.Monster_Demon_Atk_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Demon/Attack/Attack_Demon%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Demon_Atk", pComponent)))
		return E_FAIL;

	// for.Monster_Demon_Death_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Demon/Death/Death_Demon%d.png", 5);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Demon_Die", pComponent)))
		return E_FAIL;

	//// for.Monster_Demon_Bullet_Texture
	//pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster_Bullet/Monster_Demon/Monster_Demon_Bullet%d.png", 7);
	//if (nullptr == pComponent)
	//	return E_FAIL;
	//if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE1, L"Component_Texture_Demon_Bullet", pComponent)))
	//	return E_FAIL;


	// for.Monster_Afrit_Texture_Walk
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Afrit/idle/Afrit_idle%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Afrit_Walk", pComponent)))
		return E_FAIL;

	// for.Monster_Afrit_Texture_Atk1
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Afrit/Attact/Afrit_Almost_Attact%d.png", 3);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Afrit_Almost_Atk", pComponent)))
		return E_FAIL;

	// for.Monster_Afrit_Texture_Atk2
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Afrit/Attact/Afrit_Distance_Attact%d.png", 3);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Afrit_Distance_Atk", pComponent)))
		return E_FAIL;

	// for.Monster_Afrit_Texture_Hit
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Afrit/Hit/Afrit_Hit.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Afrit_Hit", pComponent)))
		return E_FAIL;

	// for.Monster_Afrit_Texture_Die
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Afrit/Death/Afrit_Death%d.png", 5);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Afrit_Die", pComponent)))
		return E_FAIL;

	// for.Monster_Afrit_Bullet
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Afrit/Bullet/Afrit_Bullet%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Afrit_Bullet", pComponent)))
		return E_FAIL;

	///
	// for.Monster_Baron_Texture_Walk
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Baron/Walk/Walk_Baron%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Baron_Walk", pComponent)))
		return E_FAIL;

	// for.Monster_Baron_Texture_Atk
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Baron/Attack/Attack_Baron%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Baron_Atk", pComponent)))
		return E_FAIL;

	// for.Monster_Baron_Texture_Hit
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Baron/Hit/Hit_Baron0.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Baron_Hit", pComponent)))
		return E_FAIL;

	// for.Monster_Baron_Texture_Die
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Baron/Death/Death_Baron%d.png", 6);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Baron_Die", pComponent)))
		return E_FAIL;

	// for.Monster_Baron_Texture_BUllet
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Baron/Bullet/Baron_Bullet0.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Baron_Bullet", pComponent)))
		return E_FAIL;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Monster_Sergeant ----------------------------------------------------------------------------------------------------------------------------------------------------------

	// for.Monster_Sergeamt_Walk
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Sergeant/Walk/Sergeant_Walk%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Sergeant_Walk", pComponent)))
		return E_FAIL;

	// for.Monster_Sergeamt_Attack
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Sergeant/Attack/Sergeant_Attack%d.png", 2);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Sergeant_Attack", pComponent)))
		return E_FAIL;

	// for.Monster_Sergeamt_Death
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Sergeant/Death/Sergeant_Dead%d.png", 9);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Sergeant_Death", pComponent)))
		return E_FAIL;

	// for.Monster_Sergeamt_hit
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Sergeant/Hit/Sergeant_Hit.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Sergeant_Hit", pComponent)))
		return E_FAIL;

	// Monster_Elemental ----------------------------------------------------------------------------------------------------------------------------------------------------------

	// for.Monster_Elemental_Walk
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Elemental/Walk/Elemental_Walk%d.png", 3);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Elemental_Walk", pComponent)))
		return E_FAIL;

	// for.Monster_Elemental_Attack
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Elemental/Attack/Elemental_Attack%d.png", 3);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Elemental_Attack", pComponent)))
		return E_FAIL;

	// for.Monster_Elemental_Death
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Elemental/Death/Elemental_Death%d.png", 6);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Elemental_Death", pComponent)))
		return E_FAIL;

	// for.Monster_Elemental_Hit
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Elemental/Hit/Elemental_Hit.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Elemental_Hit", pComponent)))
		return E_FAIL;

	// for.Monster_Elemental_Bullet
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster_Bullet/Monster_Elemental_Bullet/Elemental_Bullet%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Elemental_Bullet", pComponent)))
		return E_FAIL;

	// Monster_NormalSordier ----------------------------------------------------------------------------------------------------------------------------------------------------------

	// for.Monster_NormalSordier_Walk
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Normal_Sordier/Walk/Walk_NormalArmy%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Walk_NormalArmy", pComponent)))
		return E_FAIL;

	// for.Monster_NormalArmy_Attack
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Normal_Sordier/Attack/NormalArmy_Attack_%d.png", 3);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_NormalArmy_Attack", pComponent)))
		return E_FAIL;

	// for.Monster_NormalSordier_Death
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Normal_Sordier/Death/NormalArmy_Death_%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_NormalArmy_Death", pComponent)))
		return E_FAIL;

	// for.Monster_NormalSordier_Hit
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Normal_Sordier/Hit/NormalArmy_Hit.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_NormalArmy_Hit", pComponent)))
		return E_FAIL;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// for.Monster_Deco_Texture_Walk
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Cacodemon/idle/Walk_Cacodemon%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Deco_Walk", pComponent)))
		return E_FAIL;

	// for.Monster_Deco_Texture_Atk
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Cacodemon/Attack/Cacodemon_Attact%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Deco_Atk", pComponent)))
		return E_FAIL;

	// for.Monster_Deco_Texture_Die
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Cacodemon/Death/Cacodemon_Death%d.png", 6);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Deco_Die", pComponent)))
		return E_FAIL;

	// for.Monster_Deco_Texture_Die
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Monster/Monster_Cacodemon/Bullet/BlueFire%d.png", 12);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Deco_Bullet", pComponent)))
		return E_FAIL;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// for.Stage2_Boss_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Boss/Stage2_Boss/Walk/Walk_Demon_Boss%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Stage2Boss_Walk", pComponent)))
		return E_FAIL;

	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Boss/Stage2_Boss/Attack/Attack_Demon_Boss%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Stage2Boss_Atk", pComponent)))
		return E_FAIL;

	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Boss/Stage2_Boss/Death/Death_Demon_Boss%d.png", 10);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Stage2Boss_Die", pComponent)))
		return E_FAIL;


	// Boss Bullet
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Boss_Bullet/Boss_Bullet1/Boss_Bullet1_%d.png", 16);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Stage1Boss_Bullet1", pComponent)))
		return E_FAIL;

	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Boss_Bullet/Boss_Bullet3/Boss_Bullet3_%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Stage2Boss_Bullet3", pComponent)))
		return E_FAIL;


	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Boss_Bullet/Boss_Bullet4/Boss_Bullet4_%d.png", 2);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Stage2Boss_Bullet4", pComponent)))
		return E_FAIL;


	//Effect
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Boss_Bullet/Boss_Bullet_Blue_Effect/Boss_Bullet_Blue_Effect%d.png", 6);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Stage2Boss_Bullet_Blue_Effect", pComponent)))
		return E_FAIL;

	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Boss_Bullet/Boss_Bullet3_Effect/Boss_Bullet3_Effect%d.png", 6);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STAGE2, L"Component_Texture_Stage2Boss_Bullet_Green_Effect", pComponent)))
		return E_FAIL;



	return NOERROR;
}

HRESULT CStage2::Ready_Layer_Player(const _vec3 * vPosition, const _vec3 * vLook)
{
	//DCAST(Client_Player*, CObject_Manager::GetInstance()->Get_GameObject(SCENE_STATIC, L"Layer_Player", 0))->Free();
	//CObject_Manager::GetInstance()->Release_Object_Certain(SCENE_STATIC, L"Layer_Player");

	//Engine::CGameObject*		pGameObject = nullptr;

	//// Player
	//pGameObject = Client_Player::Create(m_pGraphicDev, vPosition, vLook);
	//if (nullptr == pGameObject)
	//	return E_FAIL;

	//if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, L"Layer_Player", pGameObject)))
	//	return E_FAIL;

	//return NOERROR;





	/*if (nullptr == m_pObject_Manager)
		return E_FAIL;

	list<CGameObject*>* pPlayerList;

	const map<const _tchar*, CLayer*>* pMapLayer = m_pObject_Manager->GetMapLayer();

	for (auto& iter : pMapLayer[SCENE_STATIC])
	{
		if (!lstrcmp(L"Layer_Player", iter.first))
		{
			pPlayerList = iter.second->GetGameObjList();
			break;
		}
	}

	if(pPlayerList->empty())
		return E_FAIL;


	auto& iter = pPlayerList->begin();
	CComponent* pPlayer_Com = (*iter)->Get_Component(L"Transform_com");
	dynamic_cast<CTransform*>(pPlayer_Com)->Set_Infomation(CTransform::INFO_POSITION, vPosition);
	dynamic_cast<CTransform*>(pPlayer_Com)->Set_Infomation(CTransform::INFO_LOOK, vLook);*/

	return NOERROR;
}

HRESULT CStage2::Ready_Layer_Static_Camera(const _tchar * pLayerTag, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float& fFovY)
{
	/*CObject_Manager::GetInstance()->Release_Object_Certain(SCENE_STATIC, L"Layer_Static_Camera");


	Engine::CGameObject*			m_Static_Camera = nullptr;

	m_Static_Camera = CStatic_Camera::Create(m_pGraphicDev, pEye, pAt, pUp, fFovY);
	if (nullptr == m_Static_Camera)
		return E_FAIL;

	m_pCamera = m_Static_Camera;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, m_Static_Camera)))
		return E_FAIL;*/
		



	/*list<CGameObject*> pCameraList;

	const map<const _tchar*, CLayer*>* pMapLayer = m_pObject_Manager->GetMapLayer();

	for (auto& iter : pMapLayer[SCENE_STATIC])
	{
		if (!lstrcmp(L"Layer_Static_Camara", iter.first))
		{
			pCameraList = *iter.second->GetGameObjList();
			break;
		}
	}

	if (pCameraList.empty())
		return E_FAIL;

	m_pCamera = *pCameraList.begin();

	m_pObject_Manager->Set_pCamera(dynamic_cast<CCamera*>(m_pCamera));*/

	return NOERROR;
}

HRESULT CStage2::Ready_Layer_Terrain(const _tchar * pLayerTag)
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
	/*pGameObject = CSky::Create(m_pGraphicDev);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return E_FAIL;*/

	return NOERROR;
}

HRESULT CStage2::Ready_Layer_Imp(const _tchar * pLayertag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMonster_Imp::Create(m_pGraphicDev, &_vec3(10.f, 0.f, 15.f), SCENE_STAGE2);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE2, pLayertag, pGameObject)))
		return E_FAIL;


	pGameObject = CMonster_Imp::Create(m_pGraphicDev, &_vec3(15.f, 0.f, 15.f), SCENE_STAGE2);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE2, pLayertag, pGameObject)))
		return E_FAIL;


	pGameObject = CMonster_Imp::Create(m_pGraphicDev, &_vec3(20.f, 0.f, 15.f), SCENE_STAGE2);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE2, pLayertag, pGameObject)))
		return E_FAIL;


	return NOERROR;
}

HRESULT CStage2::Ready_Layer_Stage2_Boss(const _tchar * pLayertag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CStage2_Boss::Create(m_pGraphicDev, &_vec3(63.f, 0.f, 45.f), SCENE_STAGE2);
	if (nullptr == pGameObject)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE2, pLayertag, pGameObject)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CStage2::ChangeScene(SCENETYPE eSceneID)
{
	//
	/*if (m_pInput_Device->Get_DIKeyState(DIK_SPACE) & 0x80)
	{
	CScene*			pNewScene = nullptr;

	switch (eSceneID)
	{
	case SCENE_STAGE2:
	pNewScene = CStage3::Create(m_pGraphicDev);
	if (nullptr == pNewScene)
	return E_FAIL;
	break;
	}

	if (FAILED(CSceneMgr::GetInstance()->SetUp_CurrentScene(pNewScene)))
	return E_FAIL;
	}*/

	return NOERROR;
}

void CStage2::LoadMap(void)
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	//Map
	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(
		L"../Bin/DataFiles/Stage2/New_Stage2.dat",
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

	//Change_Terrain(tempData_Terrain->iX, tempData_Terrain->iZ, tempData_Terrain->iItv);

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
		L"../Bin/DataFiles/Stage2/New_Obj_Stage2.dat",
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

	///////////////////////////////////////////////////////////////////////////////////////////////
	////Item
	dwByte = 0;

	hFile = CreateFile(
		L"../Bin/DataFiles/Stage2/New_Item_Stage2.dat",
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
		L"../Bin/DataFiles/Stage2/New_Mon_Stage2.dat",
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

void CStage2::Change_Terrain(const _int & iX, const _int & iZ, const _float & fItv)
{
	CComponent* pComponent = nullptr;

	pComponent = Engine::CTerrain_Buffer::Create(m_pGraphicDev, iX, iZ, fItv);
	if (nullptr == pComponent)
		return;
	
	/*m_pObject_Manager->Release_Object_Certain(SCENE_STATIC, L"Layer_Terrain");
	Engine::CComponent_Manager::GetInstance()->Release_Component_Certain(SCENE_STATIC, L"Component_Buffer_Terrain");

	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STATIC, L"Component_Buffer_Terrain", pComponent)))
		return;
	if (FAILED(Ready_Layer_Terrain(L"Layer_Terrain")))
		return;*/
}

void CStage2::Add_WallCube(const _vec3 * vPos, const _vec3 * vScale, const _int & iType, const _bool & iDoorCheck)
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

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE2, L"WallCube", pGameObject)))
		return;
}

void CStage2::Add_Obj(const _vec3 * vPos, const _vec3 * vScale, const _int & iType)
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

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE2, L"Object", pGameObject)))
		return;
}

void CStage2::Add_Item(const _vec3 * vPos, const _vec3 * vScale, const _int & iType)
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

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE2, L"Item", pGameObject)))
		return;
}

void CStage2::Add_Monster(const _tchar * tagLayer, const _vec3 * vPos, const _vec3 * vScale, const _int & iType)
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

void CStage2::Add_Imp(const _tchar * tagLayer, const _vec3 * vPos, const _vec3 * vScale)
{
	if (nullptr == m_pObject_Manager)
		return;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMonster_Imp::Create(m_pGraphicDev, vPos, SCENE_STAGE2);

	if (nullptr == pGameObject)
		return;

	pGameObject->Set_Scale(vScale);

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE2, tagLayer, pGameObject)))
		return;
}

void CStage2::Add_Demon(const _tchar * tagLayer, const _vec3 * vPos, const _vec3 * vScale)
{
	if (nullptr == m_pObject_Manager)
		return;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMonster_Demon::Create(m_pGraphicDev, vPos, SCENE_STAGE2);

	if (nullptr == pGameObject)
		return;

	pGameObject->Set_Scale(vScale);

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE2, tagLayer, pGameObject)))
		return;
}

void CStage2::Add_Deco(const _tchar * tagLayer, const _vec3 * vPos, const _vec3 * vScale)
{
}

void CStage2::Add_Afrit(const _tchar * tagLayer, const _vec3 * vPos, const _vec3 * vScale)
{
	if (nullptr == m_pObject_Manager)
		return;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMonster_Afrit::Create(m_pGraphicDev, vPos, SCENE_STAGE2);

	if (nullptr == pGameObject)
		return;

	pGameObject->Set_Scale(vScale);

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE2, tagLayer, pGameObject)))
		return;
}

void CStage2::Add_Baron(const _tchar * tagLayer, const _vec3 * vPos, const _vec3 * vScale)
{
	if (nullptr == m_pObject_Manager)
		return;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMonster_Baron::Create(m_pGraphicDev, vPos, SCENE_STAGE2);

	if (nullptr == pGameObject)
		return;

	pGameObject->Set_Scale(vScale);

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE2, tagLayer, pGameObject)))
		return;
}

void CStage2::Add_Segerent(const _tchar * tagLayer, const _vec3 * vPos, const _vec3 * vScale)
{
	if (nullptr == m_pObject_Manager)
		return;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMonster_Sergeant::Create(m_pGraphicDev, vPos, SCENE_STAGE2);

	if (nullptr == pGameObject)
		return;

	pGameObject->Set_Scale(vScale);

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE2, tagLayer, pGameObject)))
		return;
}

void CStage2::Add_Element(const _tchar * tagLayer, const _vec3 * vPos, const _vec3 * vScale)
{
	if (nullptr == m_pObject_Manager)
		return;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMonster_Elemental::Create(m_pGraphicDev, vPos, SCENE_STAGE2);

	if (nullptr == pGameObject)
		return;

	pGameObject->Set_Scale(vScale);

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE2, tagLayer, pGameObject)))
		return;
}

void CStage2::Add_NormSoldier(const _tchar * tagLayer, const _vec3 * vPos, const _vec3 * vScale)
{
	if (nullptr == m_pObject_Manager)
		return;

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CMonster_NormalSordier::Create(m_pGraphicDev, vPos, SCENE_STAGE2);

	if (nullptr == pGameObject)
		return;

	pGameObject->Set_Scale(vScale);

	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STAGE2, tagLayer, pGameObject)))
		return;
}

CStage2 * CStage2::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pPlayer)
{
	CStage2 *	pInstance = new CStage2(pGraphicDev, pPlayer);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MessageBox(0, L"CStage2 Created Failed", nullptr, MB_OK);
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CStage2::Free(void)
{
	/*Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pGraphicDev);*/


	if (FAILED(CScene::Release_Object(SCENE_STAGE2)))
		MSG_BOX("CStage2 Release Failed");

	if (FAILED(Engine::CComponent_Manager::GetInstance()->Release_Component(SCENE_STAGE2)))
		MSG_BOX("CStage2 Release Failed");

	/*if (FAILED(CScene::Release_Object(SCENE_STATIC)))
		MSG_BOX("Static Release Failed");*/

	return CScene::Free();
}
