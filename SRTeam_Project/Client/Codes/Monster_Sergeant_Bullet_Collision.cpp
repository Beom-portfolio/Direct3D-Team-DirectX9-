#include "stdafx.h"
#include "..\Headers\Monster_Sergeant_Bullet_Collision.h"


#include "Component_Manager.h"

CMonster_Sergeant_Bullet_Collision::CMonster_Sergeant_Bullet_Collision(LPDIRECT3DDEVICE9 pGraphicDev, SCENETYPE eStage)
	:CGameObject(pGraphicDev)
	, m_pTransformCom(nullptr)
{
}

HRESULT CMonster_Sergeant_Bullet_Collision::Ready_GameObject(const _vec3 * vPos, const _vec3 * vLook)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, vPos);
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_LOOK, vLook);


	m_vLook = *vLook;
	m_vPos = *vPos;
	m_vStartPos = m_vPos;

	D3DXVec3Normalize(&m_vLook, &m_vLook);

	m_tInfo.fCollisionSize = 0.5f;
	m_tInfo.iAtk = 7;


	return NOERROR;

}

_int CMonster_Sergeant_Bullet_Collision::Update_GameObject(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (m_vStartPos.x + 5.f <= m_vPos.x || m_vStartPos.x - 5.f >= m_vPos.x || m_vStartPos.z + 5.f <= m_vPos.z || m_vStartPos.z - 5.f >= m_vPos.z)
		m_tInfo.iHp = 0;

	return 0;
}

_int CMonster_Sergeant_Bullet_Collision::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (0 >= m_tInfo.iHp)
		m_tInfo.bDie = true;

	if (true == m_tInfo.bDie)
		return 1;


	//Bullet Move
	m_vPos += m_vLook * 60.f * fTimeDelta;
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &m_vPos);

	return 0;
}

void CMonster_Sergeant_Bullet_Collision::Render_GameObject(void)
{
	m_pTransformCom->SetUp_OnGraphicDev();
}

HRESULT CMonster_Sergeant_Bullet_Collision::Ready_Component()
{
	Engine::CComponent*			pComponent;

	pComponent = m_pTransformCom = DCAST(Engine::CTransform*, CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Transform_com", pComponent));
	m_pTransformCom->AddRef();

	return NOERROR;
}

CMonster_Sergeant_Bullet_Collision * CMonster_Sergeant_Bullet_Collision::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * vPos, const _vec3 * vLook, SCENETYPE eStage)
{
	CMonster_Sergeant_Bullet_Collision* pInstance = new CMonster_Sergeant_Bullet_Collision(pGraphicDev, eStage);
	if (FAILED(pInstance->Ready_GameObject(vPos, vLook)))
	{
		MSG_BOX("CMonster_Sergeant_Bullet_Collision Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CMonster_Sergeant_Bullet_Collision::Free()
{
	_ulong		dwRefCnt = 0;

	Engine::Safe_Release(m_pTransformCom);
	dwRefCnt = Engine::CGameObject::Free();

	return dwRefCnt;
}
