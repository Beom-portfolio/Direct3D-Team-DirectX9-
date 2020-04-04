#include "..\Headers\GameObject.h"

USING(Engine)

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_RenderState(SORT_END)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	m_pGraphicDev->AddRef();

	m_tInfo.fCollisionSize = 1.f;
	m_tInfo.fCollisionSearchSize = 1.f;
	m_tInfo.fCollisionAtkSearchSize = 1.f;
	m_tInfo.fSpeed = 1.f;
	m_tInfo.bDie = false;
	m_tInfo.bHit = false;
	m_tInfo.bAtk = false;
	m_tInfo.bSearch = false;
	m_tInfo.bAtkSearch = false;
	m_tInfo.iHp = 1;
	m_tInfo.iHpMax = 1;
	m_tInfo.iArmor = 0;
	m_tInfo.iAtk = 0;
	m_tInfo.bEffectOn = false;
	
}



Engine::CComponent * CGameObject::Get_Component(const _tchar * pComTag)
{
	Engine::CComponent*	pComponent = Find_Component(pComTag);

	if (nullptr == pComponent)
		return nullptr;

	return pComponent;
}

Engine::CGameObject* CGameObject::Get_GameObject(void)
{
	return this;
}

HRESULT CGameObject::Ready_GameObject(void)
{
	return NOERROR;
}

_int CGameObject::Update_GameObject(const _float & fTimeDelta)
{
	for (auto& Pair : m_mapComponent[Engine::CComponent::TYPE_DYNAMIC])
	{
		Pair.second->Update_Component(fTimeDelta);
	}
	return 0;
}

_int CGameObject::LastUpdate_GameObject(const _float & fTimeDelta)
{
	return _int();
}

void CGameObject::Render_GameObject(void)
{
}

CComponent * CGameObject::Find_Component(const _tchar * pComTag)
{
	for (size_t i = 0; i < Engine::CComponent::TYPE_END; ++i)
	{
		auto& iter = find_if(m_mapComponent[i].begin(), m_mapComponent[i].end(), CFind_Tag(pComTag));
		if (iter != m_mapComponent[i].end())
			return iter->second;
	}
	return nullptr;
}

_ulong CGameObject::Free(void)
{
	_ulong dwRefCnt = 0;

	for (size_t i = 0; i < Engine::CComponent::TYPE_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CRelease_Pair());
		m_mapComponent[i].clear();
	}	

	Engine::Safe_Release(m_pGraphicDev);
	
	return dwRefCnt;
}
