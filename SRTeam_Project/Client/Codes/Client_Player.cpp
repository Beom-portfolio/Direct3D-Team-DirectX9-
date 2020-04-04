#include "stdafx.h"
#include "..\Headers\Client_Player.h"

#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Input_Device.h"

#include "Bullet_Basic.h"
#include "Bullet_ShotGun.h"
#include "Bullet_BFG.h"
#include "Bullet_ChainGun.h"
#include "Bullet_Plasma.h"

//
#include "Layer.h"
#include "Wall_Cube.h"
#include "Yellow_UI.h"
#include "Hit.h"
#include <iostream>

Client_Player::Client_Player(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_CarryWeapon(WEAPON_BASIC)
	, m_Oncharge(false)
	, m_OnShot(false)
	, m_fFrame(0.f)
	, m_fFrameCnt(0.f)
	, m_fShotCnt(0.f)
	, m_pTransformCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pBasicCom(nullptr)
	, m_pShotGunCom(nullptr)
	, m_pBFGCom(nullptr)
	, m_pRecTexCom(nullptr)
	, m_pTargetCom(nullptr)
	, m_pChainGunCom(nullptr)
	, m_pPlasmaGunCom(nullptr)
	, m_pObject_Manager(CObject_Manager::GetInstance())
{
	//m_pObject_Manager->AddRef();
}

HRESULT Client_Player::Ready_GameObject(const _vec3* vPos, const _vec3* vLook)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	
	if (FAILED(Ready_Info()))
		return E_FAIL;


	//m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_LOOK, vLook);

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, vPos);
	m_pTransformCom->Set_Scale(&_vec3(0.3f, 0.3f, 0.3f));

	m_tInfo.fCollisionSize = 0.5f;
	m_tInfo.fCollisionAtkSearchSize = 1.f;
	m_tInfo.iHp = 100;
	m_tInfo.iArmor = 100;


	return NOERROR;
}

_int Client_Player::Update_GameObject(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);


	std::cout << "X: " << m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_LOOK)->x
			  << "  Y: " << m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_LOOK)->y
	          << "  Z: " << m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_LOOK)->z << endl;



	if (0.f < m_fFrame)
	{
		m_fFrame -= m_fFrameCnt * fTimeDelta;

		if (true == m_OnShot)
			if (m_fShotCnt > m_fFrame)
				BulletMove(L"Bullet");
	}
	else
	{
		m_Oncharge = false;
		//CSoundMgr::GetInstance()->StopSound(CSoundMgr::Channel_Player);
	}
		

	PlayerMove(fTimeDelta);
	OnMoveTerrain();

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALHPATEST, this);

	return 0;
}

_int Client_Player::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return 0;
}

void Client_Player::Render_GameObject(void)
{
	
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	m_pTransformCom->SetUp_OnGraphicDev();
	switch (m_CarryWeapon)
	{
	case WEAPON_BASIC:
		m_pBasicCom->SetUp_OnGraphicDev(0, _int(m_fFrame));
		break;
	case WEAPON_SHOTGUN:
		m_pShotGunCom->SetUp_OnGraphicDev(0, _int(m_fFrame));
		break;
	case WEAPON_CHAINGUN:
		m_pChainGunCom->SetUp_OnGraphicDev(0, _int(m_fFrame));
		break;
	case WEAPON_PLASMA:
		m_pPlasmaGunCom->SetUp_OnGraphicDev(0, _int(m_fFrame));
		break;
	case WEAPON_BFG:
		m_pBFGCom->SetUp_OnGraphicDev(0, _int(m_fFrame));
		break;

	}

	
	m_pRecTexCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT Client_Player::Ready_Component()
{
	CComponent*			pComponent = nullptr;

	// Transform
	pComponent = m_pTransformCom = (Engine::CTransform*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Transform_com", pComponent));
	m_pTransformCom->AddRef();
	
	// PlayerBuffer
	pComponent = m_pRecTexCom = (Engine::CRect_Texture*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_RectTex");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"BuffPlayer_com", pComponent));
	m_pRecTexCom->AddRef();

	//  PlayerBasic
	pComponent = m_pBasicCom = (Engine::CTexture*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Texture_Basic");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Basic_com", pComponent));
	m_pBasicCom->AddRef();

	//  PlayerShotGun
	pComponent = m_pShotGunCom = (Engine::CTexture*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Texture_ShotGun");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"ShotGun_com", pComponent));
	m_pShotGunCom->AddRef();

	//  PlayerBFG
	pComponent = m_pBFGCom = (Engine::CTexture*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Texture_BFG");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"BFG_com", pComponent));
	m_pBFGCom->AddRef();

	//  PlayerChainGun
	pComponent = m_pChainGunCom = (Engine::CTexture*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Texture_ChainGun");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"ChainGun_com", pComponent));
	m_pChainGunCom->AddRef();

	//  PlayerPlasmaGun
	pComponent = m_pPlasmaGunCom = (Engine::CTexture*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Texture_PlasmaGun");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"PlasmaGun_com", pComponent));
	m_pPlasmaGunCom->AddRef();

	// Renderer
	pComponent = m_pRendererCom = (Engine::CRenderer*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Renderer_com", pComponent));
	m_pRendererCom->AddRef();

	return NOERROR;
}

void Client_Player::OnMoveTerrain(void)
{
	if (nullptr == m_pTransformCom)
		return;

	if (nullptr == m_pTargetCom)
	{
		m_pTargetCom = (CVIBuffer*)Engine::CObject_Manager::GetInstance()->Get_Component(SCENE_STATIC, L"Layer_Terrain", 0, L"Com_Buffer");
		m_pTargetCom->AddRef();
	}

	m_pTransformCom->Move_OnBuffer(m_pTargetCom, 0.5f);
}

void Client_Player::PlayerMove(const _float& fTimeDelta)
{
	

	if (Engine::CInput_Device::GetInstance()->Get_DIMouseState(Engine::CInput_Device::DIM_LBUTTON) & 0x80)
	{
		if(0 != m_AmmoInfo.iAmmo)
		{
			if (true != m_Oncharge && true != m_OnShot)
			{
				switch (m_CarryWeapon)
				{
				case WEAPON_BASIC:
					m_fFrameCnt = 6.f;
					m_fFrame = 3.f;
					m_fShotCnt = 3.f;
					m_Oncharge = true;
					m_OnShot = true;
					--m_AmmoInfo.iBasic;
					m_AmmoInfo.iAmmo = m_AmmoInfo.iBasic;
					CSoundMgr::GetInstance()->PlaySound(L"BasicGun.wav", CSoundMgr::Channel_Player);
					break;
				case WEAPON_SHOTGUN:
					m_fFrameCnt = 15.f;
					m_fFrame = 11.f;
					m_fShotCnt = 11.f;
					m_Oncharge = true;
					m_OnShot = true;
					--m_AmmoInfo.iShotGun;
					m_AmmoInfo.iAmmo = m_AmmoInfo.iShotGun;
					CSoundMgr::GetInstance()->PlaySound(L"ShotgunFire.wav", CSoundMgr::Channel_Player);
					break;
				case WEAPON_CHAINGUN:
					m_fFrameCnt = 30.f;
					m_fFrame = 2.f;
					m_fShotCnt = 1.f;
					m_Oncharge = true;
					m_OnShot = true;
					--m_AmmoInfo.iChainGun;
					m_AmmoInfo.iAmmo = m_AmmoInfo.iChainGun;
					CSoundMgr::GetInstance()->PlaySound(L"ChainGun.wav", CSoundMgr::Channel_Player);
					break;
				case WEAPON_PLASMA:
					m_fFrameCnt = 15.f;
					m_fFrame = 2.f;
					m_fShotCnt = 1.f;
					m_Oncharge = true;
					m_OnShot = true;
					--m_AmmoInfo.iPlasmaGun;
					m_AmmoInfo.iAmmo = m_AmmoInfo.iPlasmaGun;
					CSoundMgr::GetInstance()->PlaySound(L"Plasmafire.wav", CSoundMgr::Channel_Player);
					break;
				case WEAPON_BFG:
					m_fFrame = m_fFrameCnt = 12.f;
					m_fShotCnt = 2.f;
					m_Oncharge = true;
					m_OnShot = true;
					--m_AmmoInfo.iBFG;
					m_AmmoInfo.iAmmo = m_AmmoInfo.iBFG;
					CSoundMgr::GetInstance()->PlaySound(L"BFGFire.wav", CSoundMgr::Channel_Player);
					break;

				}
			}
		}
		
	}
	

	if (true != m_Oncharge && true != m_OnShot)
	{
		if (Engine::CInput_Device::GetInstance()->Get_DIKeyState(DIK_1))
		{
			m_CarryWeapon = WEAPON_BASIC;
			m_AmmoInfo.iAmmo = m_AmmoInfo.iBasic;
		}

		if (Engine::CInput_Device::GetInstance()->Get_DIKeyState(DIK_2))
		{
			m_CarryWeapon = WEAPON_SHOTGUN;
			m_AmmoInfo.iAmmo = m_AmmoInfo.iShotGun;
		}

		if (Engine::CInput_Device::GetInstance()->Get_DIKeyState(DIK_3))
		{
			m_CarryWeapon = WEAPON_CHAINGUN;
			m_AmmoInfo.iAmmo = m_AmmoInfo.iChainGun;
		}

		if (Engine::CInput_Device::GetInstance()->Get_DIKeyState(DIK_4))
		{
			m_CarryWeapon = WEAPON_PLASMA;
			m_AmmoInfo.iAmmo = m_AmmoInfo.iPlasmaGun;
		}

		if (Engine::CInput_Device::GetInstance()->Get_DIKeyState(DIK_5))
		{
			m_CarryWeapon = WEAPON_BFG;
			m_AmmoInfo.iAmmo = m_AmmoInfo.iBFG;
		}
	}


	if (Engine::CInput_Device::GetInstance()->Get_DIKeyState(DIK_W) & 0x80)
	{
		m_pTransformCom->Go_Straight(5.f, fTimeDelta);
	}

	if (Engine::CInput_Device::GetInstance()->Get_DIKeyState(DIK_S) & 0x80)
	{
		m_pTransformCom->Go_Straight(-5.f, fTimeDelta);
	}

	if (Engine::CInput_Device::GetInstance()->Get_DIKeyState(DIK_A) & 0x80)
	{
		m_pTransformCom->Left_Right(-5.f, fTimeDelta);
	}

	if (Engine::CInput_Device::GetInstance()->Get_DIKeyState(DIK_D) & 0x80)
	{
		m_pTransformCom->Left_Right(5.f, fTimeDelta);
	}

	_long Move = 0;

	if (Move = Engine::CInput_Device::GetInstance()->Get_DIMouseMove(Engine::CInput_Device::DIMM_X))
	{
		m_pTransformCom->Rotation_Y(Move * 10.f, fTimeDelta);
	}

	if (Engine::CInput_Device::GetInstance()->Get_DIKeyState(DIK_0) & 0x80)
	{
		Ready_Info();
	}

	if (Engine::CInput_Device::GetInstance()->Get_DIKeyState(DIK_9) & 0x80)
	{
		m_tInfo.iHp -= 10;
	}

	/*
	/*std::cout << "Player  X : " << m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_POSITION)->x
		<< "  Y : " << m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_POSITION)->y
		<< "  Z : " << m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_POSITION)->z << endl;*/

	//½Â¹ü ¹® Ã¼Å©
	//¾È¾¸
	/*if (Engine::CInput_Device::GetInstance()->Get_DIKeyState(DIK_SPACE) & 0x80)
	{
		list<CGameObject*>* pWallCube = nullptr;

		const map<const _tchar*, CLayer*>* pMapLayer = m_pObject_Manager->GetMapLayer();

		for (auto& Pair : pMapLayer[SCENE_STAGE1])
		{
			if (!lstrcmp(L"WallCube", Pair.first))
				pWallCube = Pair.second->GetGameObjList();
		}

		for (auto& Door : *pWallCube)
		{
			CWall_Cube* tempCube = DCAST(CWall_Cube*, Door);

			if (true == tempCube->GetDoorCheck())
			{
				if (true == tempCube->GetDoorXZCheck())
					tempCube->Open_DoorZ(fTimeDelta);
				else
					tempCube->Open_DoorX(fTimeDelta);
			}
		}
	}

	if (Engine::CInput_Device::GetInstance()->Get_DIKeyState(DIK_B) & 0x80)
	{
		list<CGameObject*>* pWallCube = nullptr;

		const map<const _tchar*, CLayer*>* pMapLayer = m_pObject_Manager->GetMapLayer();

		for (auto& Pair : pMapLayer[SCENE_STAGE1])
		{
			if (!lstrcmp(L"WallCube", Pair.first))
				pWallCube = Pair.second->GetGameObjList();
		}

		for (auto& Door : *pWallCube)
		{
			CWall_Cube* tempCube = DCAST(CWall_Cube*, Door);

			if (true == tempCube->GetDoorCheck())
			{
				if (true == tempCube->GetDoorXZCheck())
					tempCube->Close_DoorZ(fTimeDelta);
				else
					tempCube->Close_DoorX(fTimeDelta);
			}
		}
	}*/

}

void Client_Player::BulletMove(const _tchar * pLayerTag)
{
	Engine::CGameObject*	pGameObject = nullptr;

	switch (m_CarryWeapon)
	{
	case WEAPON_BASIC:
		pGameObject = CBullet_Basic::Create(m_pGraphicDev, m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_POSITION), m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_LOOK));
		break;
	case WEAPON_SHOTGUN:
		pGameObject = CBullet_ShotGun::Create(m_pGraphicDev, m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_POSITION), m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_LOOK));
		break;
	case WEAPON_CHAINGUN:
		pGameObject = CBullet_ChainGun::Create(m_pGraphicDev, m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_POSITION), m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_LOOK));
		break;
	case WEAPON_PLASMA:
		pGameObject = CBullet_Plasma::Create(m_pGraphicDev, m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_POSITION), m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_LOOK));
		break;
	case WEAPON_BFG:
		pGameObject = CBullet_BFG::Create(m_pGraphicDev, m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_POSITION), m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_LOOK));
		break;
	}
	if (nullptr == pGameObject)
		return;
	if (FAILED(m_pObject_Manager->Ready_GameObject(SCENE_STATIC, pLayerTag, pGameObject)))
		return;
	
	m_OnShot = false;
}

HRESULT Client_Player::Ready_Info(void)
{


	m_AmmoInfo = tagAmmoInfo(0, 100, 100, 50, 100, 60, 120, 50, 120, 20, 100);

	switch (m_CarryWeapon)
	{
	case WEAPON_BASIC:
		m_AmmoInfo.iAmmo = m_AmmoInfo.iBasic;
		break;
	case WEAPON_SHOTGUN:
		m_AmmoInfo.iAmmo = m_AmmoInfo.iShotGun;
		break;
	case WEAPON_CHAINGUN:
		m_AmmoInfo.iAmmo = m_AmmoInfo.iPlasmaGun;
		break;
	case WEAPON_PLASMA:
		m_AmmoInfo.iAmmo = m_AmmoInfo.iChainGun;
		break;
	case WEAPON_BFG:
		m_AmmoInfo.iAmmo = m_AmmoInfo.iBFG;
		break;
	}

	m_tInfo.iHp = 100;
	m_tInfo.iArmor = 100;
	

	return NOERROR;
}

Client_Player * Client_Player::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vPos, const _vec3* vLook)
{
	Client_Player* pInstance = new Client_Player(pGraphicDev);


	if (FAILED(pInstance->Ready_GameObject(vPos, vLook)))
	{
		MSG_BOX("Client_Player Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void Client_Player::Earned_Item(const _int & iItemType)
{
	CSoundMgr::GetInstance()->PlaySound(L"ItemPickup.wav", CSoundMgr::Channel_Eff);
	switch (iItemType)
	{
	case ITEM_HP_LOW:
		if (100 <= Get_Info().iHp + 10)
		{
			Set_Certian_iHp(100);
		}
		else
		{ 
			Set_iHp(10);
		}
		break;
	case ITEM_HP_MEDIUM:
		if (100 <= Get_Info().iHp + 20)
		{
			Set_Certian_iHp(100);
		}
		else
		{
			Set_iHp(20);
		}
		break;
	case ITEM_HP_HIGH:
		if (100 <= Get_Info().iHp + 40)
		{
			Set_Certian_iHp(100);
		}
		else
		{
			Set_iHp(40);
		}
		break;
	case ITEM_BULLET:
		break;
	case ITEM_GUARD_SMALL:
		if (100 <= Get_Info().iArmor + 10)
		{
			Set_Certian_iArmor(100);
		}
		else
		{
			Set_iArmor(10);
		}
		break;
	case ITEM_GUARD_MEDIUM:
		if (100 <= Get_Info().iArmor + 20)
		{
			Set_Certian_iArmor(100);
		}
		else
		{
			Set_iArmor(20);
		}
		break;
	case ITEM_GUARD_HIGH:
		if (100 <= Get_Info().iArmor + 40)
		{
			Set_Certian_iArmor(100);
		}
		else
		{
			Set_iArmor(40);
		}
		break;
	case ITEM_GUN_BMG:
		if (100 <= m_AmmoInfo.iBFG + 5)
		{
			m_AmmoInfo.iBFG = 100;
		}
		else
		{
			m_AmmoInfo.iBFG += 5;
		}
		if (m_CarryWeapon == WEAPON_BFG)
			m_AmmoInfo.iAmmo = m_AmmoInfo.iBFG;
		break;
	case ITEM_GUN_SHOTGUN:
		if (100 <= m_AmmoInfo.iShotGun + 12)
		{
			m_AmmoInfo.iShotGun = 100;
		}
		else
		{
			m_AmmoInfo.iShotGun += 12;
			
		}
		if (m_CarryWeapon == WEAPON_SHOTGUN)
			m_AmmoInfo.iAmmo = m_AmmoInfo.iShotGun;
		break;
	case ITEM_GUN_CHAINGUN:
		if (120 <= m_AmmoInfo.iChainGun + 30)
		{
			m_AmmoInfo.iChainGun = 120;
		}
		else
		{
			m_AmmoInfo.iChainGun += 30;
		}
		if (m_CarryWeapon == WEAPON_CHAINGUN)
			m_AmmoInfo.iAmmo = m_AmmoInfo.iChainGun;
		break;
	case ITEM_GUN_PLASMA:
		if (100 <= m_AmmoInfo.iPlasmaGun + 20)
		{
			m_AmmoInfo.iPlasmaGun = 100;
		}
		else
		{
			m_AmmoInfo.iPlasmaGun += 20;
		}
		if (m_CarryWeapon == WEAPON_PLASMA)
			m_AmmoInfo.iAmmo = m_AmmoInfo.iPlasmaGun;
		break;
	}

	DCAST(CYellow_UI*, m_pObject_Manager->Get_GameObject(SCENE_STATIC, L"Player_Earn_UI", 0))->SetEarnOnScreen(false);
}

void Client_Player::Player_Hit(void)
{
	DCAST(CHit*, m_pObject_Manager->Get_GameObject(SCENE_STATIC, L"Player_Hit_UI", 0))->SetHitOnScreen(false);
}

_ulong Client_Player::Free(void)
{
	_ulong		dwRefcnt = 0;

	//Engine::Safe_Release(m_pObject_Manager);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pBasicCom);
	Engine::Safe_Release(m_pShotGunCom);
	Engine::Safe_Release(m_pBFGCom);
	Engine::Safe_Release(m_pChainGunCom);
	Engine::Safe_Release(m_pPlasmaGunCom);
	Engine::Safe_Release(m_pRecTexCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTargetCom);

	dwRefcnt = Engine::CGameObject::Free();

	return dwRefcnt;
}



