#include "stdafx.h"
#include "..\Headers\Map_Object.h"

#include "Constant.h"
#include "Component_Manager.h"
#include "Object_Manager.h"


CMap_Object::CMap_Object(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pObject_Manager(Engine::CObject_Manager::GetInstance())
	, m_iObjType(0)
{
	//m_pObject_Manager->AddRef();
}

const _vec3 & CMap_Object::GetObjPos(void)
{
	return *(m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_POSITION));
}

HRESULT CMap_Object::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return NOERROR;
}

_int CMap_Object::Update_GameObject(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	Billboard_Obj();


	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return 0;
}

void CMap_Object::Render_GameObject(void)
{
	if (nullptr == m_pBufferCom)
		return;

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 254);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTransformCom->SetUp_OnGraphicDev();
	m_pTextureCom->SetUp_OnGraphicDev(0, m_iObjType);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CMap_Object::TransPos_Obj(const _vec3 * vPos)
{
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, vPos);
}


void CMap_Object::Billboard_Obj(void)
{
	const _matrix* pmatView = m_pObject_Manager->Get_pCamera()->Get_TempView();
	_matrix matView = *pmatView;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_RIGHT, (_vec3*)&matView.m[Engine::CTransform::INFO_RIGHT][0]);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_UP, (_vec3*)&matView.m[Engine::CTransform::INFO_UP][0]);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_LOOK, (_vec3*)&matView.m[Engine::CTransform::INFO_LOOK][0]);

	Scaling_Obj(m_vScale.x, m_vScale.y, m_vScale.z);
}

void CMap_Object::Scaling_Obj(const _float & fX, const _float & fY, const _float & fZ)
{
	m_vScale = _vec3(fX, fY, fZ);
	m_pTransformCom->Scaling(fX, fY, fZ);
}

HRESULT CMap_Object::Ready_Component(void)
{
	Engine::CComponent*			pComponent = nullptr;

	// For.Transform
	pComponent = m_pTransformCom = (Engine::CTransform*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Transform_com", pComponent));
	m_pTransformCom->AddRef();

	// For.Buffers
	pComponent = m_pBufferCom = (Engine::CRect_Texture*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_RectTex");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	m_pBufferCom->AddRef();

	// For.Texture
	pComponent = m_pTextureCom = (Engine::CTexture*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Texture_Object");
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

CMap_Object * CMap_Object::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMap_Object *	pInstance = new CMap_Object(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("Object Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CMap_Object::Free(void)
{
	_ulong		dwRefCnt = 0;

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);
	//Engine::Safe_Release(m_pObject_Manager);

	dwRefCnt = Engine::CGameObject::Free();

	return dwRefCnt;
}
