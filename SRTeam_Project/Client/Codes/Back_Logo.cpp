#include "stdafx.h"
#include "..\Headers\Back_Logo.h"

#include "Component_Manager.h"

CBack_Logo::CBack_Logo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_fFrame(0.f)
	, m_fStack(0.f)
	, m_bLonogoEnd(false)
{

}

HRESULT CBack_Logo::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
		
	m_RenderState = Engine::CGameObject::SORT_ZERO;

	return NOERROR;
}

_int CBack_Logo::Update_GameObject(const _float & fTimeDelta)
{
	//m_pTransformCom->Rotation_Y(45.0f, fTimeDelta);

	if (m_fFrame >= 3.f)
		m_fFrame += 15.0f * fTimeDelta;
	else
		m_fFrame += 1.0f * fTimeDelta;


	if(m_fFrame >= 37.f)
	{
		++m_fStack;
		m_fFrame = 36.f;

		if(m_fStack >= 50.f)
			m_bLonogoEnd = true;
	}
	
	

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);

	return 0;
}

void CBack_Logo::Render_GameObject(void)
{
	if (nullptr == m_pBufferCom)
		return;	
	
	

	m_pBufferCom->SetUp_Buffer(&_vec3(WINCX * 0.5f, WINCY * 0.5f, 0.f), _float(WINCX), _float(WINCY));
	m_pTextureCom->SetUp_OnGraphicDev(0, _uint(m_fFrame));
	m_pBufferCom->Render_Buffer();	

	
}

HRESULT CBack_Logo::Ready_Component(void)
{
	Engine::CComponent*			pComponent = nullptr;


	// For.Buffers
	pComponent = m_pBufferCom = (Engine::CRectUI_Texture*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_RectUITex");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	m_pBufferCom->AddRef();

	// For.Texture
	pComponent = m_pTextureCom = (Engine::CTexture*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_LOGO, L"Component_Texture_BackLogo");
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

CBack_Logo * CBack_Logo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBack_Logo *	pInstance = new CBack_Logo(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CBack_Logo Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CBack_Logo::Free(void)
{
	_ulong		dwRefCnt = 0;

	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);

	dwRefCnt = Engine::CGameObject::Free();

	return dwRefCnt;
}
