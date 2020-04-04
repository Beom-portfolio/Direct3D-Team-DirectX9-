#pragma once

#include "GameObject.h"

namespace Engine
{
	class CTerrain_Buffer;
	class CRenderer;
	class CTexture;
	class CTransform;
}

class CTerrain final : public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain() = default;
public:
	virtual HRESULT Ready_GameObject(void) final;
	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject(void) final;
private:
	Engine::CTransform*				m_pTransformCom = nullptr;
	Engine::CTerrain_Buffer*		m_pBufferCom = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CTexture*				m_pTextureCom = nullptr;
public:
	HRESULT Ready_Component(void);
public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual _ulong Free(void) final;

};


