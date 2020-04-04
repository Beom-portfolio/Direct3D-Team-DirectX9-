#include "stdafx.h"
#include "..\Headers\Bullet_Basic.h"


#include "Object_Manager.h"
#include "Component_Manager.h"


CBullet_Basic::CBullet_Basic(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pTransformCom(nullptr)
{
}

HRESULT CBullet_Basic::Ready_GameObject(const _vec3* vPos, const _vec3* vLook)
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

	m_tInfo.fCollisionSize = 0.3f;
	m_tInfo.iAtk = 5;
	m_tInfo.fSpeed = 30.f;


	return NOERROR;
}

_int CBullet_Basic::Update_GameObject(const _float & fTimeDelta)
{
	
	/*_vec3 vMinVac1 = m_fNewRange - m_fOldRange;
	_vec3 vMinVac2 = m_fMoveRange - m_fOldRange;

	if (vMinVac2.x <= vMinVac1.x || vMinVac2.z <= vMinVac1.z)
	{
		D3DXVec3Normalize(&m_vLook, &m_vLook);

		m_fMoveRange += m_vLook * 5.f * fTimeDelta;

		m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &m_fMoveRange);
	}
	else
		m_OnImp = false;*/

	/*if (m_fOldRange.x <= m_fNewRange.x && m_fOldRange.z <= m_fNewRange.z)
	{
		m_fBulletSpeed += 1.f * fTimeDelta;
	}*/

	if (m_vStartPos.x + 3.f <= m_vPos.x || m_vStartPos.x - 3.f >= m_vPos.x || m_vStartPos.z + 3.f <= m_vPos.z || m_vStartPos.z - 3.f >= m_vPos.z)
		m_tInfo.iHp = 0;


	D3DXVec3Normalize(&m_vLook, &m_vLook);

	m_vPos += m_vLook * m_tInfo.fSpeed * fTimeDelta;
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &m_vPos);



	Engine::CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

_int CBullet_Basic::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (true == m_tInfo.bDie)
		return 1;

	if (0 >= m_tInfo.iHp)
		m_tInfo.bDie = true;

	return 0;
}

void CBullet_Basic::Render_GameObject(void)
{
		if (nullptr == m_pTransformCom)
			return;
		m_pTransformCom->SetUp_OnGraphicDev();

	

	
	
}

HRESULT CBullet_Basic::Ready_Component()
{
	Engine::CComponent*			pComponent;

	pComponent = m_pTransformCom = DCAST(Engine::CTransform*, CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Transform_com", pComponent));
	m_pTransformCom->AddRef();


	return NOERROR;
}

CBullet_Basic * CBullet_Basic::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vPos, const _vec3* vLook)
{
	CBullet_Basic* pInstance = new CBullet_Basic(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(vPos, vLook)))
	{
		MSG_BOX("CBullet_Basic Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CBullet_Basic::Free()
{
	_ulong		dwRefCnt = 0;

	Engine::Safe_Release(m_pTransformCom);


	dwRefCnt = Engine::CGameObject::Free();

	return dwRefCnt;
}
