#pragma once

#include "Engine_Defines.h"
#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTexture;
	class CRectUI_Texture;
	class CRenderer;
}

class CNomalFont : public CGameObject
{
public:
	enum NUMDIGIT {  AMMO_ONE, AMMO_TWO, AMMO_THREE, HEALTH_ONE, HEALTH_TWO, HEALTH_THREE, HEALTH_FOUR, ARMOR_ONE, ARMOR_TWO, ARMOR_THREE, ARMOR_FOUR, 
					SHOTGUN_ONE, SHOTGUN_TWO, SHOTGUN_THREE, MAXSHOTGUN_ONE, MAXSHOTGUN_TWO, MAXSHOTGUN_THREE, CHAINGUN_ONE, CHAINGUN_TWO, CHAINGUN_THREE, MAXCHAINGUN_ONE, MAXCHAINGUN_TWO, MAXCHAINGUN_THREE,
					PLASMAGUN_ONE, PLASMAGUN_TWO, PLASMAGUN_THREE, MAXPLASMAGUN_ONE, MAXPLASMAGUN_TWO, MAXPLASMAGUN_THREE, BFG_ONE, BFG_TWO, BFG_THREE, MAXBFG_ONE, MAXBFG_TWO, MAXBFG_THREE, NUM_END };

private:
	explicit CNomalFont(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNomalFont() = default;

public:
	virtual HRESULT Ready_GameObject(CGameObject* pPlayerObj, const NUMDIGIT& iUICnt);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

private:
	HRESULT Ready_Component();

public:
	static CNomalFont* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pPlayerObj, const NUMDIGIT& iUICnt);

private:
	Engine::CTexture*				m_pTextureCom = nullptr;
	Engine::CRectUI_Texture*		m_pRectTextureCom = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;
private:
	Engine::CGameObject*			m_pPlayerObj = nullptr;
private:
	const INFO						m_PlayerInfo;
	NUMDIGIT						m_Digit;
	_vec3							m_Pos;
	_float							m_fWidth;
	_float							m_fHeight;
	_int							m_UICnt;

	_int							m_iArmorSize;
	_int							m_iAmmoSize;
	_int							m_iHealthSize;
	_int							m_iShotGunSize;
	_int							m_iMaxShotGunSize;
	_int							m_iChainGunSize;
	_int							m_iMaxChainGunSize;
	_int							m_iPlasmaSize;
	_int							m_iMaxPlasmaSize;
	_int							m_iBFGSize;
	_int							m_iMaxBFGSize;

private:
	virtual _ulong Free();
};

