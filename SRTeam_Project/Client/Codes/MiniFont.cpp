#include "stdafx.h"
#include "..\Headers\MiniFont.h"

#include "Component_Manager.h"

CMiniFont::CMiniFont(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pRectTextureCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_Pos(0.f, 0.f, 0.f)
	, m_fWidth(0.f)
	, m_fHeight(0.f)
	, m_UICnt(0)
{
}

HRESULT CMiniFont::Ready_GameObject(const _vec3* vUIPos, const _float& fWidth, const _float& fHeight, CGameObject* pPlayerObj, const _int& iUICnt)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_Pos = *vUIPos;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_pPlayerObj = pPlayerObj;
	m_UICnt = iUICnt;

	m_RenderState = Engine::CGameObject::SORT_TWO;

	return NOERROR;
}

_int CMiniFont::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);

	return 0;
}

void CMiniFont::Render_GameObject(void)
{

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pRectTextureCom->SetUp_Buffer(&_vec3(m_Pos.x, m_Pos.y, m_Pos.z), m_fWidth, m_fHeight);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->SetUp_OnGraphicDev(0, m_UICnt);
	m_pRectTextureCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CMiniFont::Ready_Component()
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

	pComponent = m_pTextureCom = DCAST(Engine::CTexture*, CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Texture_MiniNum"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"UITex_Com", pComponent));
	m_pTextureCom->AddRef();



	return NOERROR;
}

CMiniFont * CMiniFont::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vUIPos, const _float& fWidth, const _float& fHeight, CGameObject* pPlayerObj, const _int& iUICnt)
{
	CMiniFont *	pInstance = new CMiniFont(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vUIPos, fWidth, fHeight, pPlayerObj, iUICnt)))
	{
		MSG_BOX("CMiniFont Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CMiniFont::Free()
{
	_ulong		dwRefcnt = 0;

	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pRectTextureCom);


	dwRefcnt = Engine::CGameObject::Free();

	return dwRefcnt;
}