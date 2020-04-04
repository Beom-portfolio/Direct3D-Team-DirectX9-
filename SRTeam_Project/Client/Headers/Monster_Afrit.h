#pragma once

#include "Defines.h"
#include "Engine_Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CVIBuffer;
	class CComponent_Manager;
	class CObject_Manager;
}

class CMonster_Afrit : public CGameObject
{

public:
	enum Monster_State_Afrit {WALK, HIT, ATTACK1, ATTACK2, DIE};

private:
	explicit CMonster_Afrit(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vStartPos, SCENETYPE eStage);
	virtual ~CMonster_Afrit() = default;

public:
	virtual HRESULT Ready_GameObject(const _vec3* vPos);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

private:
	HRESULT Ready_Component();
	void	Move_OnTerrain(void);
	void	PlayerPos_Check(const _float& fTimeDelta);
	HRESULT Create_Bullet(const _tchar* pLayertag, const _float& fTimeDelta);

public:
	static CMonster_Afrit* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vStartPos, SCENETYPE eStage);

private:
	Engine::CTransform*				m_pTransformCom = nullptr;
	Engine::CRect_Texture*			m_pBufferCom = nullptr;
	Engine::CTexture*				m_pTextureCom_Walk = nullptr;
	Engine::CTexture*				m_pTextureCom_Hit = nullptr;
	Engine::CTexture*				m_pTextureCom_Atk1 = nullptr;
	Engine::CTexture*				m_pTextureCom_Atk2 = nullptr;
	Engine::CTexture*				m_pTextureCom_Die = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CVIBuffer*				m_pTargetCom = nullptr;
	Engine::CComponent_Manager*		m_pComponent_Manager = nullptr;
	Engine::CObject_Manager*		m_pObject_Manager = nullptr;
private:
	Monster_State_Afrit				m_eState = WALK;
	SCENETYPE						m_eStage = SCENE_END;

	_vec3							m_vStartPos{ 0.f, 0.f, 0.f };

	bool							m_bSetUp_Hit = true;
	bool							m_bSetUp_Die = true;
	bool							m_bSetUp_Atk = true;
	bool							m_bDieing = true;
	bool							m_bAtkTF = false;

	_int							m_iCount = 0;

	_float							m_fFrame = 0.f;
	_float							m_fFrameMax = 0.f;

private:
	virtual _ulong Free();
};

