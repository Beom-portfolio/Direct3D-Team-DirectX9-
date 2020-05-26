#include "stdafx.h"
#include "..\Headers\MainGame.h"

#include "Component_Manager.h"
#include "SceneMgr.h"
#include "Object_Manager.h"
#include "Input_Device.h"
#include "Logo.h"




CMainGame::CMainGame()
	: m_pGraphic_Device(Engine::CGraphic_Device::GetInstance())
	, m_pGraphicDev(nullptr)
	, m_pComponent_Manager(Engine::CComponent_Manager::GetInstance())
	, m_pSceneMgr(CSceneMgr::GetInstance())
{
	m_pGraphic_Device->AddRef();
	m_pComponent_Manager->AddRef();
	m_pSceneMgr->AddRef();
}

CMainGame::~CMainGame(void)
{
}

HRESULT CMainGame::Ready_MainGame(void)
{
	CSoundMgr::GetInstance()->LoadSoundFile();

	if (FAILED(Ready_Default(g_hWnd, CGraphic_Device::MODE_WIN, WINCX, WINCY)))
		return E_FAIL;

	if (FAILED(Ready_StaticComponent()))
		return E_FAIL;

	if (FAILED(Ready_StartScene()))
		return E_FAIL;

	return NOERROR;
}

int CMainGame::Update_MainGame(const float & fTimeDelta)
{
	if (nullptr == m_pSceneMgr)
		return -1;

	return m_pSceneMgr->Update_SceneMgr(fTimeDelta);
}

void CMainGame::Render_MainGame(void)
{
	if (nullptr == m_pGraphicDev)
		return;

	m_pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), 1.f, 0);
	m_pGraphicDev->BeginScene();

	// 그려야할객체들을 그린다.
	m_pSceneMgr->Render_SceneMgr();

	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(nullptr, nullptr, nullptr, nullptr);
}

HRESULT CMainGame::Ready_Default(HWND hWnd, Engine::CGraphic_Device::WINMODE eMode, const _uint & iBackCX, const _uint & iBackCY)
{
	if (nullptr == m_pGraphic_Device)
		goto FailMSG;

	if (FAILED(m_pGraphic_Device->Ready_GraphicDev(&m_pGraphicDev, hWnd, eMode, iBackCX, iBackCY)))
		goto FailMSG;


	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	// 조명
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	if (FAILED(m_pSceneMgr->Ready_SceneMgr(SCENE_END)))
		goto FailMSG;

	return NOERROR;


FailMSG:
	MSG_BOX("Ready_DefaultSetting Failed");
	return E_FAIL;
}

HRESULT CMainGame::Ready_StaticComponent(void)
{
	Engine::CComponent*			pComponent = nullptr;

	// Transform -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	pComponent = Engine::CTransform::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Ready_Component(SCENE_STATIC, L"Component_Transform", pComponent)))
		return E_FAIL;

	// Rect_Texture -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	pComponent = Engine::CRect_Texture::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Ready_Component(SCENE_STATIC, L"Component_Buffer_RectTex", pComponent)))
		return E_FAIL;
	
	// RectUI_Texture -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	pComponent = Engine::CRectUI_Texture::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Ready_Component(SCENE_STATIC, L"Component_Buffer_RectUITex", pComponent)))
		return E_FAIL;

	// Renderer -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	pComponent = Engine::CRenderer::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Ready_Component(SCENE_STATIC, L"Component_Renderer", pComponent)))
		return E_FAIL;
	m_pSceneMgr->SetUp_Renderer((CRenderer*)pComponent);

	// Player -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// for.Player_Basic
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Player/Gun/Basic/Basic%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STATIC, L"Component_Texture_Basic", pComponent)))
		return E_FAIL;

	// for.Player_SHotGun
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Player/Gun/ShotGun/ShotGun%d.png", 12);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STATIC, L"Component_Texture_ShotGun", pComponent)))
		return E_FAIL;

	// for.Player_BFG
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Player/Gun/BFG/BFG%d.png", 13);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STATIC, L"Component_Texture_BFG", pComponent)))
		return E_FAIL;

	// for.Player_ChainGun
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Player/Gun/ChainGun/ChainGun%d.png", 6);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STATIC, L"Component_Texture_ChainGun", pComponent)))
		return E_FAIL;
	
	// for.Player_PlasmaGun
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Player/Gun/PlasmaGun/PlasmaGun%d.png", 5);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STATIC, L"Component_Texture_PlasmaGun", pComponent)))
		return E_FAIL;

	// Bullet -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// for.Bullet_BFG
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Player/Bullet/BFG/Bullet_BFG%d.png", 4);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STATIC, L"Component_Texture_BFGBullet", pComponent)))
		return E_FAIL;

	// for.Bullet_Chain
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Player/Bullet/Chain/Chain_Bullet%d.png", 16);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STATIC, L"Component_Texture_ChainBullet", pComponent)))
		return E_FAIL;

	// for.Bullet_Plasma
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Player/Bullet/Plasma/Plasma_Effect%d.png", 16);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STATIC, L"Component_Texture_PlasmaBullet", pComponent)))
		return E_FAIL;


	// LOGO -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// LOGO
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Logo/Logo/Logo%d.png", 37);
	if (nullptr == pComponent)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Ready_Component(SCENE_LOGO, L"Component_Texture_BackLogo", pComponent)))
		return E_FAIL;

	// MENU -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


	// MainMenu_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/MainMenu/MainMenu%d.png", 149);
	if (nullptr == pComponent)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Ready_Component(SCENE_MENU, L"Component_Texture_GameMenu", pComponent)))
		return E_FAIL;

	// StartMenu_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/MenuStart/GameStart%d.png", 20);
	if (nullptr == pComponent)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Ready_Component(SCENE_MENU, L"Component_Texture_StartMenu", pComponent)))
		return E_FAIL;

	// UI -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//	MainUI_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/UI/MainUI.png");
	if (nullptr == pComponent)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Ready_Component(SCENE_STATIC, L"Component_Texture_MainUI", pComponent)))
		return E_FAIL;

	//	Number_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/UI/Number/Number%d.png", 12);
	if (nullptr == pComponent)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Ready_Component(SCENE_STATIC, L"Component_Texture_Number", pComponent)))
		return E_FAIL;

	//	MiniNum_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/UI/MiniNum/MiniNum%d.png", 11);
	if (nullptr == pComponent)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Ready_Component(SCENE_STATIC, L"Component_Texture_MiniNum", pComponent)))
		return E_FAIL;

	//	Face_UP_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/UI/Face/Up/Face_Up%d.png", 6);
	if (nullptr == pComponent)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Ready_Component(SCENE_STATIC, L"Component_Texture_Face_Up", pComponent)))
		return E_FAIL;

	//	Hit_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/UI/Hit/Hit0.png");
	if (nullptr == pComponent)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Ready_Component(SCENE_STATIC, L"Component_Texture_Hit", pComponent)))
		return E_FAIL;

	// Effect -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//	BFG_Effect
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Effect/BFG/BFG_Effect%d.png", 17);
	if (nullptr == pComponent)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Ready_Component(SCENE_STATIC, L"Component_Texture_BFG_Effect", pComponent)))
		return E_FAIL;

	//	Plasma_Effect
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Effect/Plasma/Plasma_Effect%d.png", 13);
	if (nullptr == pComponent)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Ready_Component(SCENE_STATIC, L"Component_Texture_Plasma_Effect", pComponent)))
		return E_FAIL;

	//	ShotGun_Effect
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Effect/ShotGun/ShotGun_Effect%d.png", 40);
	if (nullptr == pComponent)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Ready_Component(SCENE_STATIC, L"Component_Texture_ShotGun_Effect", pComponent)))
		return E_FAIL;

//	Earn_Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/UI/Earn/Earn0.png");
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(m_pComponent_Manager->Ready_Component(SCENE_STATIC, L"Component_Texture_Earn", pComponent)))
		return E_FAIL;


	// Mapping -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// For.Cube_Rect
	pComponent = Engine::CRect_Cube::Create(m_pGraphicDev);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STATIC, L"Component_Buffer_WallCube", pComponent)))
		return E_FAIL;

	// For.Texture_Cube
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_CUBE, L"../Bin/Resource/Mapping/Wall_Cube/Wall_%d.dds", iCubeIndex);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STATIC, L"Component_Texture_WallCube", pComponent)))
		return E_FAIL;

	// For.Texture_Obj
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Mapping/Object/Object_%d.png", iObjIndex);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STATIC, L"Component_Texture_Object", pComponent)))
		return E_FAIL;

	// For.Texture_Item
	pComponent = Engine::CTexture::Create(m_pGraphicDev, Engine::CTexture::TYPE_NORMAL, L"../Bin/Resource/Mapping/Item/Item_%d.png", iItemIndex);
	if (nullptr == pComponent)
		return E_FAIL;
	if (FAILED(Engine::CComponent_Manager::GetInstance()->Ready_Component(SCENE_STATIC, L"Component_Texture_Item", pComponent)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMainGame::Ready_StartScene(void)
{
	if (nullptr == m_pSceneMgr)
		return E_FAIL;

	CScene*			pCurrentScene = nullptr;

	pCurrentScene = CLogo::Create(m_pGraphicDev);
	if (nullptr == pCurrentScene)
		return E_FAIL;

	if (FAILED(m_pSceneMgr->SetUp_CurrentScene(pCurrentScene)))
		return E_FAIL;


	return NOERROR;
}

CMainGame * CMainGame::Create(void)
{
	CMainGame *	pInstance = new CMainGame();

	if (FAILED(pInstance->Ready_MainGame()))
	{
		MessageBox(0, L"CMainGame Created Failed", nullptr, MB_OK);
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

unsigned long CMainGame::Free(void)
{
	Engine::Safe_Release(m_pSceneMgr);
	Engine::Safe_Release(m_pComponent_Manager);
	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pGraphic_Device);


	_ulong	dwRefCnt = 0;

	if (dwRefCnt = CSceneMgr::GetInstance()->DestroyInstance())
		MSG_BOX("CSceneMgr Released Failed");

	if (dwRefCnt = CComponent_Manager::GetInstance()->DestroyInstance())
		MSG_BOX("CComponent_Manager Released Failed");

	if (dwRefCnt = CObject_Manager::GetInstance()->DestroyInstance())
		MSG_BOX("CObject_Manager Released Failed");

	if (dwRefCnt = CGraphic_Device::GetInstance()->DestroyInstance())
		MSG_BOX("CGraphic_Device Released Failed");

	

	return dwRefCnt;
}
