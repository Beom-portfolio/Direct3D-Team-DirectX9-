#pragma once
#include "Defines.h"
#include "GameObject.h"


namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CObject_Manager;
}


class CBoss_Rush_Collision :
	public CGameObject
{
public:
	explicit CBoss_Rush_Collision(LPDIRECT3DDEVICE9 pGraphicDev, SCENETYPE eStage);
	virtual ~CBoss_Rush_Collision() = default;


public:
	virtual HRESULT Ready_GameObject(const _vec3* vPos, const _vec3* vLook);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

private:
	HRESULT Ready_Component();
	void Create_Effect(const _tchar * pLayertag);

public:
	static CBoss_Rush_Collision* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vPos, const _vec3* vLook, SCENETYPE eStage);

private:
	virtual _ulong	Free();

private:
	Engine::CTransform*				m_pTransformCom = nullptr;
	Engine::CRect_Texture*			m_pRectTextureCom = nullptr;
	Engine::CTexture*				m_pTextureCom = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;
private:
	Engine::CObject_Manager*		m_pObjectManager = nullptr;
private:
	SCENETYPE						m_eStage = SCENE_END;

	_vec3							m_vPos;
	_vec3							m_vStartPos;
	_vec3							m_vLook;

	_float							m_fFrame = 0.f;



};
