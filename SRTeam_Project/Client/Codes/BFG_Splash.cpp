#include "stdafx.h"
#include "..\Headers\BFG_Splash.h"

#include "Component_Manager.h"
#include "Object_Manager.h"
#include "BFG_Effect.h"

CBFG_Splash::CBFG_Splash(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_pTransformCom(nullptr)
	, m_pObjectManager(CObject_Manager::GetInstance())
{
	//m_pObjectManager->AddRef();
}


HRESULT CBFG_Splash::Ready_GameObject(const _vec3 * vPos)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (nullptr == m_pTransformCom)
		return E_FAIL;

	
	m_pTransformCom->Set_Infomation(Engine::CTransform::INFO_POSITION, vPos);

	m_tInfo.fCollisionSize = 5.f;


	m_vPos = *vPos;
	

	m_tInfo.iAtk = 100;

	m_pTransformCom->SetUp_OnGraphicDev();

	Ready_Effect();
	

	return NOERROR;
}

_int CBFG_Splash::Update_GameObject(const _float & fTimeDelta)
{
	

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

_int CBFG_Splash::LastUpdate_GameObject(const _float & fTimeDelta)
{

	//int a = 10;
	m_pTransformCom->SetUp_OnGraphicDev();



	if (0 >= m_tInfo.iHp)
		m_tInfo.bDie = true;

	if (true == m_tInfo.bDie)
		return 1;

	m_tInfo.iHp = -1;

	

	return 0;
}

void CBFG_Splash::Render_GameObject(void)
{
	
}

HRESULT CBFG_Splash::Ready_Component()
{
	Engine::CComponent*			pComponent;

	pComponent = m_pTransformCom = DCAST(Engine::CTransform*, CComponent_Manager::GetInstance()->Clone_Component(SCENE_STATIC, L"Component_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::CComponent::TYPE_STATIC].insert(MAPCOMPONENT::value_type(L"Transform_com", pComponent));
	m_pTransformCom->AddRef();

	return NOERROR;
}

void CBFG_Splash::Ready_Effect()
{

	Engine::CGameObject*	pGameObject = nullptr;

	const _vec3* vPos;
	_vec3* pathPos;
	for (size_t i = 0; i < 100; i++)
	{
		int randem = rand() % 2;
		if (randem == 0)
			randem = -1;
		else
			randem = 1;
		vPos = m_pTransformCom->Get_Infomation(Engine::CTransform::INFO_POSITION);
		pathPos = &_vec3(vPos->x + (rand() % 2) * randem, vPos->y, vPos->z + (rand() % 2) * randem);

		pGameObject = CBFG_Effect::Create(m_pGraphicDev, pathPos);

		if (nullptr == pGameObject)
			return;

		if (FAILED(m_pObjectManager->Ready_GameObject(SCENE_STATIC, L"BFG_Effect", pGameObject)))
			return;

		pGameObject = nullptr;
	}
	

}

CBFG_Splash * CBFG_Splash::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * vPos)
{
	CBFG_Splash* pInstance = new CBFG_Splash(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		MSG_BOX("CBFG_Splash Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CBFG_Splash::Free()
{
	_ulong		dwRefCnt = 0;

	Engine::Safe_Release(m_pTransformCom);
	//Engine::Safe_Release(m_pObjectManager);
	
	dwRefCnt = Engine::CGameObject::Free();

	return dwRefCnt;
}
