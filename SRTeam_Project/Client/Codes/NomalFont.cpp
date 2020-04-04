#include "stdafx.h"
#include "..\Headers\NomalFont.h"

#include "Component_Manager.h"
#include "Client_Player.h"

CNomalFont::CNomalFont(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pRectTextureCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_Pos(0.f, 0.f, 0.f)
	, m_fWidth(0.f)
	, m_fHeight(0.f)
	, m_UICnt(0)
	, m_Digit(NUM_END)
	, m_iAmmoSize(0)
	, m_iArmorSize(0)
	, m_iHealthSize(0)
	, m_iShotGunSize(0)
	, m_iMaxShotGunSize(0)
	, m_iChainGunSize(0)
	, m_iMaxChainGunSize(0)
	, m_iPlasmaSize(0)
	, m_iMaxPlasmaSize(0)
	, m_iBFGSize(0)
	, m_iMaxBFGSize(0)
{
}

HRESULT CNomalFont::Ready_GameObject(CGameObject* pPlayerObj, const NUMDIGIT& iUICnt)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pPlayerObj = pPlayerObj;
	m_Digit = iUICnt;
	

	m_RenderState = Engine::CGameObject::SORT_TWO;
	return NOERROR;
}

_int CNomalFont::Update_GameObject(const _float & fTimeDelta)
{

	m_iArmorSize = DCAST(Client_Player*, m_pPlayerObj)->Get_Info().iArmor;
	m_iHealthSize = DCAST(Client_Player*, m_pPlayerObj)->Get_Info().iHp;
	m_iAmmoSize = DCAST(Client_Player*, m_pPlayerObj)->GetAmmoInfo().iAmmo;
	m_iShotGunSize = DCAST(Client_Player*, m_pPlayerObj)->GetAmmoInfo().iShotGun;
	m_iMaxShotGunSize = DCAST(Client_Player*, m_pPlayerObj)->GetAmmoInfo().iMaxShotGun;
	m_iChainGunSize = DCAST(Client_Player*, m_pPlayerObj)->GetAmmoInfo().iChainGun;
	m_iMaxChainGunSize = DCAST(Client_Player*, m_pPlayerObj)->GetAmmoInfo().iMaxChainGun;
	m_iPlasmaSize = DCAST(Client_Player*, m_pPlayerObj)->GetAmmoInfo().iPlasmaGun;
	m_iMaxPlasmaSize = DCAST(Client_Player*, m_pPlayerObj)->GetAmmoInfo().iMaxPlasmaGun;
	m_iBFGSize = DCAST(Client_Player*, m_pPlayerObj)->GetAmmoInfo().iBFG;
	m_iMaxBFGSize = DCAST(Client_Player*, m_pPlayerObj)->GetAmmoInfo().iMaxBFG;


	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);

	return 0;
}

void CNomalFont::Render_GameObject(void)
{

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);


	//AMMO_ONE, AMMO_TWO, AMMO_THREE, HEALTH_ONE, HEALTH_TWO, HEALTH_THREE, HEALTH_FOUR, ARMOR_ONE, ARMOR_TWO, ARMOR_THREE, ARMOR_FOUR,
	//	SHOTGUN_ONE, SHOTGUN_TWO, SHOTGUN_THREE, MAXSHOTGUN_ONE, MAXSHOTGUN_TWO, MAXSHOTGUN_THREE, CHAINGUN_ONE, CHAINGUN_TWO, CHAINGUN_THREE, MAXCHAINGUN_ONE, MAXCHAINGUN_TWO, MAXCHAINGUN_THREE,
	//	PLASMAGUN_ONE, PLASMAGUN_TWO, PLASMAGUN_THREE, MAXPLASMAGUN_ONE, MAXPLASMAGUN_TWO, MAXPLASMAGUN_THREE, BFG_ONE, BFG_TWO, BFG_THREE, MAXBFG_ONE, MAXBFG_TWO, MAXBFG_THREE

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	switch (m_Digit)
	{
	case CNomalFont::AMMO_ONE:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(90.f, 540.f, 0.f), _float(60), _float(80));
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iAmmoSize % 10);
		break;
	case CNomalFont::AMMO_TWO:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(55.f, 540.f, 0.f), _float(60), _float(80));
		for (int i = 0; i < 1; ++i)
			m_iAmmoSize /= 10;
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iAmmoSize % 10);
		break;
	case CNomalFont::AMMO_THREE:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(20.f, 540.f, 0.f), _float(60), _float(80));
		for (int i = 0; i < 2; ++i)
			m_iAmmoSize /= 10;
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iAmmoSize % 10);
		break;
	case CNomalFont::HEALTH_ONE:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(243.f, 540.f, 0.f), _float(60), _float(80));
		m_pTextureCom->SetUp_OnGraphicDev(0, 10);
		break;
	case CNomalFont::HEALTH_TWO:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(208.f, 540.f, 0.f), _float(60), _float(80));
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iHealthSize % 10);
		break;
	case CNomalFont::HEALTH_THREE:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(173.f, 540.f, 0.f), _float(60), _float(80));
		for (int i = 0; i < 1; ++i)
			m_iHealthSize /= 10;
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iHealthSize % 10);
		break;
	case CNomalFont::HEALTH_FOUR:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(138.f, 540.f, 0.f), _float(60), _float(80));
		for (int i = 0; i < 2; ++i)
			m_iHealthSize /= 10;
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iHealthSize % 10);
		break;
	case CNomalFont::ARMOR_ONE:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(570.f, 540.f, 0.f), _float(60), _float(80));
		m_pTextureCom->SetUp_OnGraphicDev(0, 10);
		break;
	case CNomalFont::ARMOR_TWO:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(535.f, 540.f, 0.f), _float(60), _float(80));
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iArmorSize % 10);
		break;
	case CNomalFont::ARMOR_THREE:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(500.f, 540.f, 0.f), _float(60), _float(80));
		for (int i = 0; i < 1; ++i)
			m_iArmorSize /= 10;
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iArmorSize % 10);
		break;
	case CNomalFont::ARMOR_FOUR:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(465.f, 540.f, 0.f), _float(60), _float(80));
		for (int i = 0; i < 2; ++i)
			m_iArmorSize /= 10;
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iArmorSize % 10);
		break;
	case CNomalFont::SHOTGUN_ONE:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(718.f, 526.f, 0.f), _float(20), _float(30));
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iShotGunSize % 10);
		break;
	case CNomalFont::SHOTGUN_TWO:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(708.f, 526.f, 0.f), _float(20), _float(30));
		for (int i = 0; i < 1; ++i)
			m_iShotGunSize /= 10;
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iShotGunSize % 10);
		break;
	case CNomalFont::SHOTGUN_THREE:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(698.f, 526.f, 0.f), _float(20), _float(30));
		for (int i = 0; i < 2; ++i)
			m_iShotGunSize /= 10;
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iShotGunSize % 10);
		break;
	case CNomalFont::MAXSHOTGUN_ONE:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(775.f, 526.f, 0.f), _float(20), _float(30));
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iMaxShotGunSize % 10);
		break;
	case CNomalFont::MAXSHOTGUN_TWO:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(765.f, 526.f, 0.f), _float(20), _float(30));
		for (int i = 0; i < 1; ++i)
			m_iMaxShotGunSize /= 10;
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iMaxShotGunSize % 10);
		break;
	case CNomalFont::MAXSHOTGUN_THREE:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(755.f, 526.f, 0.f), _float(20), _float(30));
		for (int i = 0; i < 2; ++i)
			m_iMaxShotGunSize /= 10;
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iMaxShotGunSize % 10);
		break;
	case CNomalFont::CHAINGUN_ONE:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(718.f, 546.f, 0.f), _float(20), _float(30));
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iChainGunSize % 10);
		break;
	case CNomalFont::CHAINGUN_TWO:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(708.f, 546.f, 0.f), _float(20), _float(30));
		for (int i = 0; i < 1; ++i)
			m_iChainGunSize /= 10;
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iChainGunSize % 10);
		break;
	case CNomalFont::CHAINGUN_THREE:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(698.f, 546.f, 0.f), _float(20), _float(30));
		for (int i = 0; i < 2; ++i)
			m_iChainGunSize /= 10;
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iChainGunSize % 10);
		break;
	case CNomalFont::MAXCHAINGUN_ONE:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(775.f, 546.f, 0.f), _float(20), _float(30));
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iMaxChainGunSize % 10);
		break;
	case CNomalFont::MAXCHAINGUN_TWO:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(765.f, 546.f, 0.f), _float(20), _float(30));
		for (int i = 0; i < 1; ++i)
			m_iMaxChainGunSize /= 10;
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iMaxChainGunSize % 10);
		break;
	case CNomalFont::MAXCHAINGUN_THREE:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(755.f, 546.f, 0.f), _float(20), _float(30));
		for (int i = 0; i < 2; ++i)
			m_iMaxChainGunSize /= 10;
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iMaxChainGunSize % 10);
		break;
	case CNomalFont::PLASMAGUN_ONE:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(718.f, 566.f, 0.f), _float(20), _float(30));
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iPlasmaSize % 10);
		break;
	case CNomalFont::PLASMAGUN_TWO:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(708.f, 566.f, 0.f), _float(20), _float(30));
		for (int i = 0; i < 1; ++i)
			m_iPlasmaSize /= 10;
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iPlasmaSize % 10);
		break;
	case CNomalFont::PLASMAGUN_THREE:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(698.f, 566.f, 0.f), _float(20), _float(30));
		for (int i = 0; i < 2; ++i)
			m_iPlasmaSize /= 10;
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iPlasmaSize % 10);
		break;
	case CNomalFont::MAXPLASMAGUN_ONE:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(775.f, 566.f, 0.f), _float(20), _float(30));
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iMaxPlasmaSize % 10);
		break;
	case CNomalFont::MAXPLASMAGUN_TWO:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(765.f, 566.f, 0.f), _float(20), _float(30));
		for (int i = 0; i < 1; ++i)
			m_iMaxPlasmaSize /= 10;
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iMaxPlasmaSize % 10);
		break;
	case CNomalFont::MAXPLASMAGUN_THREE:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(755.f, 566.f, 0.f), _float(20), _float(30));
		for (int i = 0; i < 2; ++i)
			m_iMaxPlasmaSize /= 10;
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iMaxPlasmaSize % 10);
		break;
	case CNomalFont::BFG_ONE:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(718.f, 586.f, 0.f), _float(20), _float(30));
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iBFGSize % 10);
		break;
	case CNomalFont::BFG_TWO:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(708.f, 586.f, 0.f), _float(20), _float(30));
		for (int i = 0; i < 1; ++i)
			m_iBFGSize /= 10;
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iBFGSize % 10);
		break;
	case CNomalFont::BFG_THREE:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(698.f, 586.f, 0.f), _float(20), _float(30));
		for (int i = 0; i < 2; ++i)
			m_iBFGSize /= 10;
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iBFGSize % 10);
		break;
	case CNomalFont::MAXBFG_ONE:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(775.f, 586.f, 0.f), _float(20), _float(30));
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iMaxBFGSize % 10);
		break;
	case CNomalFont::MAXBFG_TWO:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(765.f, 586.f, 0.f), _float(20), _float(30));
		for (int i = 0; i < 1; ++i)
			m_iMaxBFGSize /= 10;
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iMaxBFGSize % 10);
		break;
	case CNomalFont::MAXBFG_THREE:
		m_pRectTextureCom->SetUp_Buffer(&_vec3(755.f, 586.f, 0.f), _float(20), _float(30));
		for (int i = 0; i < 2; ++i)
			m_iMaxBFGSize /= 10;
		m_pTextureCom->SetUp_OnGraphicDev(0, m_iMaxBFGSize % 10);
		break;
	default:
		break;
	}
	




	
	m_pRectTextureCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CNomalFont::Ready_Component()
{
	Engine::CComponent*			pComponent = nullptr;

	pComponent = m_pRectTextureCom = DCAST(Engine::CRectUI_Texture*, CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_RectUITex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"RectTex_Com", pComponent));
	m_pRectTextureCom->AddRef();

	pComponent = m_pRendererCom = DCAST(Engine::CRenderer*, CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Renderer_Com", pComponent));
	m_pRendererCom->AddRef();

	pComponent = m_pTextureCom = DCAST(Engine::CTexture*, CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Texture_Number"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"UITex_Com", pComponent));
	m_pTextureCom->AddRef();



	return NOERROR;
}

CNomalFont * CNomalFont::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pPlayerObj, const NUMDIGIT& iUICnt)
{
	CNomalFont *	pInstance = new CNomalFont(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pPlayerObj, iUICnt)))
	{
		MSG_BOX("CNomalFont Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CNomalFont::Free()
{
	_ulong		dwRefcnt = 0;

	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pRectTextureCom);


	dwRefcnt = Engine::CGameObject::Free();

	return dwRefcnt;
}
