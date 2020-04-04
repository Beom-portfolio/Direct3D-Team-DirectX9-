#pragma once

#include "Defines.h"
#include "Engine_Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CRenderer;
	class CTexture;
	class CVIBuffer;
	class CComponent_Manager;
	class CObject_Manager;
}

class CMonster_NormalSordier : public CGameObject 
{
public:
	enum Monster_State { WALK, WALK_LEFT, WALK_RIGHT, HIT, ATTACK, DIE };

private:
	explicit CMonster_NormalSordier(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vStartPos, SCENETYPE eStage);
	virtual ~CMonster_NormalSordier() = default;


public:
	virtual HRESULT Ready_GameObject(void) final;
	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject(void) final;

private:
	HRESULT Ready_Component(void);
	void Move_OnTerrain(void);
	void PlayerPos_Check(float fTimeDelta);
	HRESULT Create_Bullet(const _tchar * pLayertag, float fTimeDelta);


public:
	static CMonster_NormalSordier* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vStartPos, SCENETYPE eStage);
protected:
	virtual _ulong Free(void) final;

private:
	Engine::CTransform*				m_pTransformCom = nullptr;
	Engine::CRect_Texture*			m_pBufferCom = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CTexture*				m_pWalkTextureCom = nullptr;
	/*Engine::CTexture*				m_pTextureCom_Walk_Left = nullptr;
	Engine::CTexture*				m_pTextureCom_Walk_Right = nullptr;*/
	Engine::CTexture*				m_pAttackTextureCom = nullptr;
	Engine::CTexture*				m_pDeathTextureCom = nullptr;
	Engine::CTexture*				m_pHitTextureCom = nullptr;
	Engine::CVIBuffer*				m_pTargetCom = nullptr;
	Engine::CComponent_Manager*		m_pComponent_Manager = nullptr;
	Engine::CObject_Manager*		m_pObject_Manager = nullptr;



private:
	Monster_State					m_eState = WALK;
	SCENETYPE						m_eStage = SCENE_END;

	_vec3							m_vStartPos{ 0.f, 0.f, 0.f };
	_vec3							m_vRendomMovePos{ 0.f, 0.f, 0.f };

	_bool							m_bDieing = true;
	_bool							m_bSetUp_Hit = true;
	_bool							m_bSetUp_Die = false;
	_float							m_fFrame = 0.f;
	_float							m_fFrameMax = 0.f;
	_int							m_iCount = 0;

};

