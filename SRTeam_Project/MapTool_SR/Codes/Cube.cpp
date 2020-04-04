#include "stdafx.h"
#include "..\Headers\Cube.h"

#include "Component_Manager.h"

CCube::CCube(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_iCubeType(0)
{
}

const _vec3&  CCube::GetCubePos(void)
{
	return *(m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_POSITION));
}

HRESULT CCube::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return NOERROR;
}

_int CCube::Update_GameObject(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return 0;
}

void CCube::Render_GameObject(void)
{
	if (nullptr == m_pBufferCom)
		return;
	
	if(true == g_bWireFrameCheck)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTransformCom->SetUp_OnGraphicDev();
	m_pTextureCom->SetUp_OnGraphicDev(0, m_iCubeType);
	m_pBufferCom->Render_Buffer();
}

void CCube::TransPos_Cube(const _vec3 * vPos)
{
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, vPos);
}

void CCube::Scaling_Cube(const _float & fX, const _float & fY, const _float & fZ)
{
	m_vScale = _vec3(fX, fY, fZ);
	m_pTransformCom->Scaling(fX, fY, fZ);
}

HRESULT CCube::Ready_Component(void)
{
	Engine::CComponent*			pComponent = nullptr;

	// For.Transform
	pComponent = m_pTransformCom = (Engine::CTransform*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));
	m_pTransformCom->AddRef();

	// For.Buffers
	pComponent = m_pBufferCom = (Engine::CRect_Cube*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_LOGO, L"Component_Buffer_Cube");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	m_pBufferCom->AddRef();

	// For.Texture
	pComponent = m_pTextureCom = (Engine::CTexture*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_LOGO, L"Component_Texture_Cube");
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

CCube * CCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCube *	pInstance = new CCube(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("CCube Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CCube::Free(void)
{
	_ulong		dwRefCnt = 0;


	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);

	dwRefCnt = Engine::CGameObject::Free();

	return dwRefCnt;
}
