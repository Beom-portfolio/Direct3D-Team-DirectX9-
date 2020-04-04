#pragma once

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CObject_Manager;
}

class CBullet_ShotGun : public CGameObject
{
private:
	explicit CBullet_ShotGun(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBullet_ShotGun() = default;

public:
	virtual HRESULT Ready_GameObject(const _vec3* vPos, const _vec3* vLook);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

private:
	HRESULT Ready_Component();

public:
	static CBullet_ShotGun* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vPos, const _vec3* vLook);

private:
	Engine::CTransform*				m_pTransformCom = nullptr;
	Engine::CObject_Manager*		m_pObjectManager = nullptr;
private:
	bool							m_OnImp;
	_vec3							m_vPos;
	_vec3							m_vLook;
	_vec3							m_vStartPos;

private:
	virtual _ulong	Free();
};

