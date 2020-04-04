#pragma once

#include "GameObject.h"

namespace Engine
{
	class CRect_Texture;
	class CRenderer;
	class CTexture;
	class CTransform;
}

class CBack_Logo final : public Engine::CGameObject
{
private:
	explicit CBack_Logo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBack_Logo() = default;
public:
	virtual HRESULT Ready_GameObject(void) final;
	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject(void) final;
private:
	Engine::CTransform*				m_pTransformCom = nullptr;
	Engine::CRect_Texture*			m_pBufferCom = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CTexture*				m_pTextureCom = nullptr;
public:
	HRESULT Ready_Component(void);
public:
	static CBack_Logo* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual _ulong Free(void) final;

};

