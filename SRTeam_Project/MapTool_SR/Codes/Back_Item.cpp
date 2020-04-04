#include "stdafx.h"
#include "..\Headers\Back_Item.h"

#include "Component_Manager.h"

CBack_Item::CBack_Item(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_iItemType(0)
{
}

const _vec3 & CBack_Item::GetItemPos(void)
{
	return *(m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_POSITION));
}

HRESULT CBack_Item::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return NOERROR;
}

_int CBack_Item::Update_GameObject(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	Billboard_Item();


	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return 0;
}

void CBack_Item::Render_GameObject(void)
{
	if (nullptr == m_pBufferCom)
		return;

	if (true == g_bWireFrameCheck)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTransformCom->SetUp_OnGraphicDev();
	m_pTextureCom->SetUp_OnGraphicDev(0, m_iItemType);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CBack_Item::TransPos_Item(const _vec3 * vPos)
{
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, vPos);
}

void CBack_Item::Billboard_Item(void)
{
	_matrix			matView;
	matView.Identity();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_RIGHT, (_vec3*)&matView.m[Engine::CTransform::INFO_RIGHT][0]);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_LOOK, (_vec3*)&matView.m[Engine::CTransform::INFO_LOOK][0]);

	Scaling_Item(m_vScale.x, m_vScale.y, m_vScale.z);
}

void CBack_Item::Scaling_Item(const _float & fX, const _float & fY, const _float & fZ)
{
	m_vScale = _vec3(fX, fY, fZ);
	m_pTransformCom->Scaling(fX, fY, fZ);
}

HRESULT CBack_Item::Ready_Component(void)
{
	Engine::CComponent*			pComponent = nullptr;

	// For.Transform
	pComponent = m_pTransformCom = (Engine::CTransform*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));
	m_pTransformCom->AddRef();

	// For.Buffers
	pComponent = m_pBufferCom = (Engine::CRect_Texture*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_LOGO, L"Component_Buffer_Rect");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	m_pBufferCom->AddRef();

	// For.Texture
	pComponent = m_pTextureCom = (Engine::CTexture*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_LOGO, L"Component_Texture_Item");
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

CBack_Item * CBack_Item::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBack_Item *	pInstance = new CBack_Item(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("Item Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CBack_Item::Free(void)
{
	_ulong		dwRefCnt = 0;

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);

	dwRefCnt = Engine::CGameObject::Free();

	return dwRefCnt;
}
