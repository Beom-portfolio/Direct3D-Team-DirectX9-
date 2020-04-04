#pragma once

#include "Defines.h"
#include "Engine_Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CTexture;
	class CRect_Texture;
	class CRect_Cube;
	class CVIBuffer;
	class CComponent_Manager;
	class CObject_Manager;
}

class CStage2_Boss :
	public CGameObject
{
public:
	explicit CStage2_Boss(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vStartPos, SCENETYPE eStage);
	virtual ~CStage2_Boss();

private:
	enum Monster_State { WALK, ATTACK, DIE };
	enum Monster_Pattern { PATTERN1, PATTERN2 };

public:
	virtual HRESULT Ready_GameObject(void) final;
	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject(void) final;

private:
	HRESULT Ready_Component(void);
	void Move_OnTerrain(void);
	void PlayerPos_Check(float fTimeDelta);
	void Rush_PlayerPos(float fTimeDelta);

	HRESULT Create_Bullet1(const _tchar * pLayertag);
	HRESULT Create_Bullet2(const _tchar * pLayertag);
	HRESULT Create_Bullet3(const _tchar * pLayertag);
	HRESULT Create_Bullet4(const _tchar * pLayertag);
	HRESULT Create_Bullet5(const _tchar * pLayertag);

private:
	void BossPattern(float fTimeDelta);
	void Pattern_One(float fTimeDelta);
	void Pattern_Two(float fTimeDelta);

	void Boss_PlayPattern_One1(float fTimeDelta);
	void Boss_PlayPattern_One2(float fTimeDelta);
	void Boss_PlayPattern_One3(float fTimeDelta);
	void Boss_PlayPattern_One4(float fTimeDelta);

	void Boss_PlayPattern_Two1(float fTimeDelta);
	void Boss_PlayPattern_Two2(float fTimeDelta);
	void Boss_PlayPattern_Two3(float fTimeDelta);
	void Boss_PlayPattern_Two4(float fTimeDelta);

public:
	static CStage2_Boss* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vStartPos, SCENETYPE eStage);
protected:
	virtual _ulong Free(void) final;

private:
	Engine::CTransform*				m_pTransformCom = nullptr;
	Engine::CRect_Texture*			m_pBufferCom = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CTexture*				m_pTextureCom_Walk = nullptr;
	Engine::CTexture*				m_pTextureCom_Atk = nullptr;
	Engine::CTexture*				m_pTextureCom_Die = nullptr;
	Engine::CVIBuffer*				m_pTargetCom = nullptr;
	Engine::CComponent_Manager*		m_pComponent_Manager = nullptr;
	Engine::CObject_Manager*		m_pObject_Manager = nullptr;

private:
	SCENETYPE						m_eStage = SCENE_END;
	Monster_State					m_eState = WALK;


	_vec3							m_vStartPos{ 0.f, 0.f, 0.f };
	_vec3							m_vRendomMovePos{ 0.f, 0.f, 0.f };
	_vec3							m_vTempPos{ 0.f, 0.f, 0.f };
	_vec3							m_vTempShot{ 0.f, 0.f, 0.f };


	_bool							m_bSetUp_Die = true;
	_bool							m_bSetUpPattern = false;
	_bool							m_bSetUp_Pos = true;
	_bool							m_bSetUp_ShotCount = true;

	_bool							m_bSetUp_Pattern_Change = true;
	_bool							m_bSetUp_Pattern_Change2 = true;

	_bool							m_bDieing = true;
	_bool							m_bShotBullet = true;
	_bool							m_bSet_StartPos = true;
	_bool							m_bRush_Count = true;

	_int							m_iPattern = 0;
	_int							m_iPattern_Count = 1;
	_int							m_iPattern_Change_Count = 0;
	_int							m_iShotCount = 0;
	_int							m_iWaitCount = 0;


	_float							m_fFrame = 0.f;
	_float							m_fFrameMax = 0.f;
};

