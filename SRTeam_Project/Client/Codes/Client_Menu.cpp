#include "stdafx.h"
#include "..\Headers\Client_Menu.h"



#include "Component_Manager.h"

CClient_Menu::CClient_Menu(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pTransformCom(nullptr)
	, m_pRectTextureCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_fFrame(0.f)
{
}

HRESULT CClient_Menu::Ready_GameObject()
{
	
	if (Ready_Component())
		return E_FAIL;

	

	//m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, vPos);

	
	m_RenderState = Engine::CGameObject::SORT_ZERO;

	return NOERROR;
}

_int CClient_Menu::Update_GameObject(const _float & fTimeDelta)
{
	m_fFrame += 30.f * fTimeDelta;

	if (m_fFrame >= 149.f)
	{
		m_fFrame = 148.f;
	}


	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);

	return 0;
}

void CClient_Menu::Render_GameObject(void)
{
	if (nullptr == m_pRectTextureCom)
		return;
	if (nullptr == m_pTextureCom)
		return;


	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	
	
	m_pRectTextureCom->SetUp_Buffer(&_vec3(WINCX * 0.5f, WINCY * 0.5f, 0.f), _float(WINCX), _float(WINCY));

	m_pTextureCom->SetUp_OnGraphicDev(0, _int(m_fFrame));
	m_pRectTextureCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

}

HRESULT CClient_Menu::Ready_Component()
{
	Engine::CComponent*			pComponent = nullptr;

	pComponent = m_pTransformCom = DCAST(Engine::CTransform*, CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Trans_Com", pComponent));
	m_pTransformCom->AddRef();

	
	pComponent = m_pRendererCom = DCAST(Engine::CRenderer*, CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Renderer_Com", pComponent));
	m_pRendererCom->AddRef();

	
	pComponent = m_pRectTextureCom = DCAST(Engine::CRectUI_Texture*, CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_RectUITex"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"RectUITex_Com", pComponent));
	m_pRectTextureCom->AddRef();



	pComponent = m_pTextureCom = DCAST(Engine::CTexture*, CComponent_Manager::GetInstance()->Clone_Component(SCENE_MENU, L"Component_Texture_GameMenu"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"MenuTex_Com", pComponent));
	m_pTextureCom->AddRef();
	

	return NOERROR;
}

CClient_Menu * CClient_Menu::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CClient_Menu *	pInstance = new CClient_Menu(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CClient_Menu Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CClient_Menu::Free()
{
	_ulong		dwRefcnt = 0;

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pRectTextureCom);


	dwRefcnt = Engine::CGameObject::Free();

	return dwRefcnt;
}
