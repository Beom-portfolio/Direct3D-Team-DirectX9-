#pragma once

#include "Defines.h"

#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CTexture;
	class CRect_Texture;
	class CRect_Cube;
	class CVIBuffer;
	class CObject_Manager;
}

class Client_Player : public CGameObject
{
public:
	enum CHANGE_WEAPON {WEAPON_BASIC, WEAPON_SHOTGUN, WEAPON_BFG, WEAPON_CHAINGUN, WEAPON_PLASMA, WEAPON_END};


private:
	explicit Client_Player(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~Client_Player() = default;

public:
	virtual HRESULT Ready_GameObject(const _vec3* vPos, const _vec3* vLook) final;
	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject(void) final;

public:
	const AMMOINFO& GetAmmoInfo() {
		return m_AmmoInfo;
	}


private:
	Engine::CTransform*				m_pTransformCom = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CTexture*				m_pBasicCom = nullptr;
	Engine::CTexture*				m_pShotGunCom = nullptr;
	Engine::CTexture*				m_pBFGCom = nullptr;
	Engine::CTexture*				m_pChainGunCom = nullptr;
	Engine::CTexture*				m_pPlasmaGunCom = nullptr;
	Engine::CRect_Texture*			m_pRecTexCom = nullptr;
	//Engine::CRect_Cube*			m_pRectCubeCom = nullptr;
	Engine::CObject_Manager*		m_pObject_Manager = nullptr;
private:
	Engine::CVIBuffer*				m_pTargetCom = nullptr;

private:
	HRESULT Ready_Component(void);
	void	OnMoveTerrain(void);
	void	PlayerMove(const _float& fTimeDelta);
	void	BulletMove(const _tchar* pLayerTag);

private:
	HRESULT Ready_Info(void);

public:
	static Client_Player* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vPos, const _vec3* vLook);

public:
	void  Earned_Item(const _int& iItemType);
	void  Player_Hit(void);

private:
	CHANGE_WEAPON		m_CarryWeapon = WEAPON_BASIC;
	_bool				m_Oncharge = false;
	_bool				m_OnShot = false;
	_float				m_fFrame = 0.f;
	_float				m_fFrameCnt = 0.f;
	_float				m_fShotCnt = 0.f;
private:
	AMMOINFO			m_AmmoInfo;


public:
	virtual _ulong Free(void) final;
};

