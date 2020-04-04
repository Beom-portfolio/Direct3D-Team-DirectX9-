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

class CBullet_BFG : public CGameObject
{
private:
	explicit CBullet_BFG(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBullet_BFG() = default;

public:
	virtual HRESULT Ready_GameObject(const _vec3* vPos, const _vec3* vLook);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

private:
	HRESULT Ready_Component();

public:
	static CBullet_BFG* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vPos, const _vec3* vLook);

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

private:
	virtual _ulong	Free();
};

