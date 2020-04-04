#include "stdafx.h"
#include "..\Headers\Sky.h"

#include "Component_Manager.h"

USING(Engine)

CSky::CSky(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTextureCom(nullptr)
{
}

HRESULT CSky::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(&_vec3(2.f, 2.f, 2.f));

	return NOERROR;
}

_int CSky::Update_GameObject(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_PRIORITY, this);

	return 0;
}

_int CSky::LastUpdate_GameObject(const _float & fTimeDelta)
{
	_matrix				matView;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, (_vec3*)&matView.m[3][0]);

	return 0;
}

void CSky::Render_GameObject(void)
{
	if (nullptr == m_pBufferCom)
		return;

	if (nullptr == m_pGraphicDev)
		return;

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_pTransformCom->SetUp_OnGraphicDev();
	m_pTextureCom->SetUp_OnGraphicDev(0, 0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CSky::Ready_Component(void)
{
	Engine::CComponent*			pComponent = nullptr;

	// For.Transform
	pComponent = m_pTransformCom = (Engine::CTransform*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));
	m_pTransformCom->AddRef();

	// For.Buffers
	pComponent = m_pBufferCom = (Engine::CSky_Cube_Texture*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_Sky");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer_Sky", pComponent));
	m_pBufferCom->AddRef();

	// For.Texture
	pComponent = m_pTextureCom = (Engine::CTexture*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Texture_Sky");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_Sky", pComponent));
	m_pTextureCom->AddRef();

	// For.Renderer
	pComponent = m_pRendererCom = (Engine::CRenderer*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Renderer");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));
	m_pRendererCom->AddRef();

	return NOERROR;
}

CSky * CSky::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSky *	pInstance = new CSky(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CSky Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CSky::Free(void)
{
	_ulong		dwRefCnt = 0;

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);

	dwRefCnt = Engine::CGameObject::Free();

	return dwRefCnt;
}
