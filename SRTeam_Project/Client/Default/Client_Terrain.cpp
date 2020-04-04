#include "stdafx.h"
#include "../Headers/Client_Terrain.h"

#include "Component_Manager.h"

USING(Engine)

CClient_Terrain::CClient_Terrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pMaterialCom(nullptr)
{

}

HRESULT CClient_Terrain::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return NOERROR;
}

_int CClient_Terrain::Update_GameObject(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return 0;
}

void CClient_Terrain::Render_GameObject(void)
{
	if (nullptr == m_pBufferCom)
		return;


	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pMaterialCom->SetUp_OnGraphicDev();
	m_pTransformCom->SetUp_OnGraphicDev();
	m_pTextureCom->SetUp_OnGraphicDev(0, 0);
	m_pBufferCom->Render_Buffer();
}

HRESULT CClient_Terrain::Ready_Component(void)
{
	Engine::CComponent*			pComponent = nullptr;

	// For.Material
	pComponent = m_pMaterialCom = (Engine::CMaterial*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE1, L"Component_Material");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Material", pComponent));
	m_pMaterialCom->AddRef();

	// For.Transform
	pComponent = m_pTransformCom = (Engine::CTransform*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));
	m_pTransformCom->AddRef();

	// For.Buffers
	pComponent = m_pBufferCom = (Engine::CTerrain_Buffer*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE1, L"Component_Buffer_Terrain");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	m_pBufferCom->AddRef();

	// For.Texture
	pComponent = m_pTextureCom = (Engine::CTexture*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STAGE1, L"Component_Texture_Terrain");
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


CClient_Terrain * CClient_Terrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CClient_Terrain* pInstance = new CClient_Terrain(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("Client_Terrain Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

_ulong CClient_Terrain::Free(void)
{
	_ulong		dwRefCnt = 0;

	Engine::Safe_Release(m_pMaterialCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);

	dwRefCnt = Engine::CGameObject::Free();

	return dwRefCnt;
}
