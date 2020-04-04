#include "stdafx.h"
#include "..\Headers\Bullet_ShotGun.h"

#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Effect_ShotGun.h"

CBullet_ShotGun::CBullet_ShotGun(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pTransformCom(nullptr)
	, m_pObjectManager(CObject_Manager::GetInstance())
{
	m_pObjectManager->AddRef();
}

HRESULT CBullet_ShotGun::Ready_GameObject(const _vec3 * vPos, const _vec3 * vLook)
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
	m_OnImp = true;

	m_tInfo.iAtk = 15;
	m_tInfo.fSpeed = 20.f;


	return NOERROR;
}

_int CBullet_ShotGun::Update_GameObject(const _float & fTimeDelta)
{
	
	

	if (m_vStartPos.x + 0.5f <= m_vPos.x || m_vStartPos.x - 0.5f >= m_vPos.x || m_vStartPos.z + 0.5f <= m_vPos.z || m_vStartPos.z - 0.5f >= m_vPos.z)
		m_tInfo.iHp = 0;


	D3DXVec3Normalize(&m_vLook, &m_vLook);

	m_vPos += m_vLook * m_tInfo.fSpeed * fTimeDelta;
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, &m_vPos);



	Engine::CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

_int CBullet_ShotGun::LastUpdate_GameObject(const _float & fTimeDelta)
{
	if (true == m_tInfo.bEffectOn)
	{
		Engine::CGameObject*	pGameObject = nullptr;
		const _vec3* vPos = m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_POSITION);
		pGameObject = CEffect_ShotGun::Create(m_pGraphicDev, vPos);

		m_tInfo.bEffectOn = false;

		if (nullptr == pGameObject)
			return 1;

		if (FAILED(m_pObjectManager->Ready_GameObject(SCENE_STATIC, L"BFG_Effect", pGameObject)))
			return 1;


	}

	if (true == m_tInfo.bDie)
	{
		return 1;
	}
		

	if (0 >= m_tInfo.iHp)
		m_tInfo.bDie = true;

	return 0;
}

void CBullet_ShotGun::Render_GameObject(void)
{
	if (nullptr == m_pTransformCom)
		return;


	m_pTransformCom->SetUp_OnGraphicDev();
}

HRESULT CBullet_ShotGun::Ready_Component()
{
	Engine::CComponent*			pComponent;

	pComponent = m_pTransformCom = DCAST(Engine::CTransform*, CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Transform_com", pComponent));
	m_pTransformCom->AddRef();


	return NOERROR;
}

CBullet_ShotGun * CBullet_ShotGun::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * vPos, const _vec3 * vLook)
{
	CBullet_ShotGun* pInstance = new CBullet_ShotGun(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(vPos, vLook)))
	{
		MSG_BOX("CBullet_ShotGun Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CBullet_ShotGun::Free()
{
	_ulong		dwRefCnt = 0;

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pObjectManager);

	dwRefCnt = Engine::CGameObject::Free();

	return dwRefCnt;
}
