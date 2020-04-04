#include "stdafx.h"
#include "..\Headers\Client_UI.h"

#include "Component_Manager.h"
#include "Object_Manager.h"

CClient_UI::CClient_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pRectTextureCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pPlayerObj(nullptr)
	, m_Pos(0.f, 0.f, 0.f)
	, m_fWidth(0.f)
	, m_fHeight(0.f)
	, m_UICnt(0)
{
}

HRESULT CClient_UI::Ready_GameObject(const _vec3* vUIPos, const _float& fWidth, const _float& fHeight)
{

	if (FAILED(Ready_Component()))
		return E_FAIL;


	m_Pos = *vUIPos;
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	m_RenderState = Engine::CGameObject::SORT_ZERO;

	return NOERROR;
}

_int CClient_UI::Update_GameObject(const _float & fTimeDelta)
{

	//m_pPlayerObj->Get_Info().;

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);

	return 0;
}

void CClient_UI::Render_GameObject(void)
{

	m_pRectTextureCom->SetUp_Buffer(&_vec3(m_Pos.x, m_Pos.y, m_Pos.z), m_fWidth, m_fHeight);
	m_pTextureCom->SetUp_OnGraphicDev(0);
	m_pRectTextureCom->Render_Buffer();
}

HRESULT CClient_UI::Ready_Component(void)
{
	Engine::CComponent*			pComponent = nullptr;

	// For.Buffers
	pComponent = m_pRectTextureCom = (Engine::CRectUI_Texture*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_RectUITex");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_RectUITex", pComponent));
	m_pRectTextureCom->AddRef();

	// For.Texture
	pComponent = m_pTextureCom = (Engine::CTexture*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Texture_MainUI");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
	m_pTextureCom->AddRef();

	// For.Renderer
	pComponent = m_pRendererCom = (Engine::CRenderer*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));
	m_pRendererCom->AddRef();

	return NOERROR;
}

CClient_UI * CClient_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vUIPos, const _float& fWidth, const _float& fHeight)
{
	CClient_UI* pInstance = new CClient_UI(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vUIPos, fWidth, fHeight)))
	{
		MSG_BOX("CClient_UI Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CClient_UI::Free()
{
	_ulong		dwRefcnt = 0;

	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pRectTextureCom);


	dwRefcnt = Engine::CGameObject::Free();

	return dwRefcnt;
}
