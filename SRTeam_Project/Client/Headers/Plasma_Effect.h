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


class CPlasma_Effect : public CGameObject
{
private:
	explicit CPlasma_Effect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlasma_Effect() = default;

public:
	virtual HRESULT Ready_GameObject(const _vec3* vPos);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

private:
	HRESULT Ready_Component();

public:
	static CPlasma_Effect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vPos);

private:
	Engine::CTransform*				m_pTransformCom = nullptr;
	Engine::CRect_Texture*			m_pRectTextureCom = nullptr;
	Engine::CTexture*				m_pTextureCom = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;
private:
	Engine::CObject_Manager*		m_pObjectManager = nullptr;

private:
	_vec3							m_vPos;
	_float							m_fFrame;
	_vec3							m_vStartPos;

private:
	virtual _ulong	Free();
};

