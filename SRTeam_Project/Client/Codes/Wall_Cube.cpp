#include "stdafx.h"
#include "..\Headers\Wall_Cube.h"

#include "Component_Manager.h"

CWall_Cube::CWall_Cube(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pTransformCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pRendererCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_iCubeType(0)
{
}

const _vec3 & CWall_Cube::GetCubePos(void)
{
	return *(m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_POSITION));
}

HRESULT CWall_Cube::Ready_GameObject(void)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (true == m_bDoorCheck)
	{
		m_tInfo.fCollisionSize = 1.5f;
	}

	return NOERROR;
}

_int CWall_Cube::Update_GameObject(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return 0;
}

void CWall_Cube::Render_GameObject(void)
{
	if (nullptr == m_pBufferCom)
		return;

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pTransformCom->SetUp_OnGraphicDev();
	m_pTextureCom->SetUp_OnGraphicDev(0, m_iCubeType);
	m_pBufferCom->Render_Buffer();
}

void CWall_Cube::TransPos_Cube(const _vec3 * vPos)
{
	m_vOriginPos = *vPos;
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, vPos);
}

void CWall_Cube::Scaling_Cube(const _float & fX, const _float & fY, const _float & fZ)
{
	m_vScale = _vec3(fX, fY, fZ);
	m_pTransformCom->Scaling(fX, fY, fZ);
}

void CWall_Cube::Open_DoorX(const _float & fTimeDelta)
{
	float flimit = m_vOriginPos.x - 0.98f;
	
	if (flimit < m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_POSITION)->x)
		m_pTransformCom->Left_Right(-m_fDoorSpeed, fTimeDelta);
	
}

void CWall_Cube::Open_DoorZ(const _float & fTimeDelta)
{
	float flimit = m_vOriginPos.z - 0.98f;

	if (flimit < m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_POSITION)->z)
		m_pTransformCom->Go_Straight(-m_fDoorSpeed, fTimeDelta);
	
}

void CWall_Cube::Close_DoorX(const _float & fTimeDelta)
{
	if(m_vOriginPos.x > m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_POSITION)->x)
		m_pTransformCom->Left_Right(m_fDoorSpeed, fTimeDelta);
}

void CWall_Cube::Close_DoorZ(const _float & fTimeDelta)
{
	if (m_vOriginPos.z > m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_POSITION)->z)
		m_pTransformCom->Go_Straight(m_fDoorSpeed, fTimeDelta);
}

HRESULT CWall_Cube::Ready_Component(void)
{
	Engine::CComponent*			pComponent = nullptr;

	// For.Transform
	pComponent = m_pTransformCom = (Engine::CTransform*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Transform_com", pComponent));
	m_pTransformCom->AddRef();

	// For.Buffers
	pComponent = m_pBufferCom = (Engine::CRect_Cube*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Buffer_WallCube");
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	m_pBufferCom->AddRef();

	// For.Texture
	pComponent = m_pTextureCom = (Engine::CTexture*)CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Texture_WallCube");
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

CWall_Cube * CWall_Cube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWall_Cube *	pInstance = new CWall_Cube(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		MSG_BOX("WallCube Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CWall_Cube::Free(void)
{
	_ulong		dwRefCnt = 0;

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pBufferCom);

	dwRefCnt = Engine::CGameObject::Free();

	return dwRefCnt;
}
