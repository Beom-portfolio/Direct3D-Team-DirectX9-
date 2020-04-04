#pragma once

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CObject_Manager;
}


class CBFG_Splash : public CGameObject
{
private:
	explicit CBFG_Splash(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBFG_Splash() = default;


public:
	virtual HRESULT Ready_GameObject(const _vec3* vPos);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

private:
	HRESULT Ready_Component();
	void	Ready_Effect();

public:
	static CBFG_Splash* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vPos);

private:
	Engine::CTransform*				m_pTransformCom = nullptr;
private:
	Engine::CObject_Manager*		m_pObjectManager = nullptr;

private:
	_vec3							m_vPos;

private:
	virtual _ulong	Free();
};

