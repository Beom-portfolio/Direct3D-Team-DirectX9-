#pragma once

#include "Engine_Defines.h"
#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class DLL_EXPORT CGameObject : public CBase
{
	
public:
	enum RENDER_SORT { SORT_ZERO, SORT_ONE, SORT_TWO, SORT_THREE, SORT_FOUR, SORT_FIVE, SORT_SIX, SORT_SEVEN, SORT_END};

protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGameObject() = default;

public:
	void Set_bHit(bool bHit) { m_tInfo.bHit = bHit; }
	void Set_bSearch(bool bSearch) { m_tInfo.bSearch = bSearch; }
	void Set_bAtkSearch(bool bAtkSearch) { m_tInfo.bAtkSearch = bAtkSearch; }
	void Set_bAtk(bool bAtk) { m_tInfo.bSearch = bAtk; }
	void Set_bEffectOn(bool bEffectOn) { m_tInfo.bEffectOn = bEffectOn; }
	void Set_iHp(int iHp) { m_tInfo.iHp += iHp; }
	void Set_iArmor(int iArmor) { m_tInfo.iArmor += iArmor; }
	void Set_Certian_iHp(int iHp) { m_tInfo.iHp = iHp; }
	void Set_Certian_iArmor(int iArmor) { m_tInfo.iArmor = iArmor; }
	INFO& Set_Info(void) { return m_tInfo; }

	void Set_Scale(const _vec3* vScale)
	{
		m_vScale = *vScale;
	}
public:
	Engine::CComponent* Get_Component(const _tchar* pComTag);
	Engine::CGameObject* Get_GameObject(void);
	RENDER_SORT Get_RenderState(void) { return m_RenderState; }
	const INFO& Get_Info(void) { return m_tInfo; }
public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
protected:
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;
protected:
	map<const _tchar*, CComponent*>			m_mapComponent[Engine::CComponent::TYPE_END];
	typedef map<const _tchar*, CComponent*>	MAPCOMPONENT;

protected:
	RENDER_SORT						m_RenderState = SORT_END;

	INFO		m_tInfo;
	_vec3		m_vScale;
private:
	CComponent* Find_Component(const _tchar* pComTag);
protected:
	virtual _ulong Free(void) override;
};

END