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


class CBullet_Effect_Green : public CGameObject
{
public:
	explicit CBullet_Effect_Green(LPDIRECT3DDEVICE9 pGraphicDev, SCENETYPE eStage);
	virtual ~CBullet_Effect_Green() = default;

public:
	virtual HRESULT Ready_GameObject(const _vec3* vPos);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

private:
	HRESULT Ready_Component();

public:
	static CBullet_Effect_Green* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vPos, SCENETYPE eStage);

private:
	Engine::CTransform*				m_pTransformCom = nullptr;
	Engine::CRect_Texture*			m_pRectTextureCom = nullptr;
	Engine::CTexture*				m_pTextureCom = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;
private:
	Engine::CObject_Manager*		m_pObjectManager = nullptr;

private:
	_vec3							m_vPos;
	_vec3							m_vLook;
	_float							m_fFrame;
	_vec3							m_vStartPos;
	SCENETYPE						m_eStage = SCENE_END;

private:
	virtual _ulong	Free();
};

