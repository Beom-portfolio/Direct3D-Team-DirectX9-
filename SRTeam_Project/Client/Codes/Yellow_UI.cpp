#include "stdafx.h"
#include "..\Headers\Yellow_UI.h"

#include "Component_Manager.h"

CYellow_UI::CYellow_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pRectTextureCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_Pos(0.f, 0.f, 0.f)
	, m_fWidth(0.f)
	, m_fHeight(0.f)
	, m_UICnt(0)
	, m_bHide(true)
	, m_iDelay(0)
{
}

HRESULT CYellow_UI::Ready_GameObject(const _vec3 * vUIPos, const _float & fWidth, const _float & fHeight, CGameObject * pPlayerObj, const _int & iUICnt)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_Pos = *vUIPos;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_pPlayerObj = pPlayerObj;
	m_UICnt = iUICnt;

	m_RenderState = Engine::CGameObject::SORT_ZERO;

	return NOERROR;
}

_int CYellow_UI::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);

	return 0;
}

void CYellow_UI::Render_GameObject(void)
{
	if (false == m_bHide)
	{
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		
		m_pRectTextureCom->SetUp_Buffer(&_vec3(m_Pos.x, m_Pos.y, m_Pos.z), m_fWidth, m_fHeight);

		m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_MIN);
	
		m_pTextureCom->SetUp_OnGraphicDev(0, 0);
		m_pRectTextureCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		m_iDelay += 1;

		if (4 < m_iDelay)
		{
			m_iDelay = 0;
			m_bHide = true;
		}
	}
}

HRESULT CYellow_UI::Ready_Component()
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

	pComponent = m_pTextureCom = DCAST(Engine::CTexture*, CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Texture_Earn"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"UITex_Com", pComponent));
	m_pTextureCom->AddRef();



	return NOERROR;
}

CYellow_UI * CYellow_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * vUIPos, const _float & fWidth, const _float & fHeight, CGameObject * pPlayerObj, const _int & iUICnt)
{
	CYellow_UI *	pInstance = new CYellow_UI(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vUIPos, fWidth, fHeight, pPlayerObj, iUICnt)))
	{
		MSG_BOX("Yellow_UI Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CYellow_UI::Free()
{
	_ulong		dwRefcnt = 0;

	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pRectTextureCom);


	dwRefcnt = Engine::CGameObject::Free();

	return dwRefcnt;
}
