#pragma once
#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CSky_Cube_Texture;
	class CRenderer;
	class CTexture;
	class CTransform;
}

class CSky : public Engine::CGameObject
{
private:
	explicit CSky(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSky() = default;

public:
	virtual HRESULT Ready_GameObject(void) final;
	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject(void) final;
private:
	Engine::CTransform*				m_pTransformCom = nullptr;
	Engine::CSky_Cube_Texture*		m_pBufferCom = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CTexture*				m_pTextureCom = nullptr;
public:
	HRESULT Ready_Component(void);
public:
	static CSky* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual _ulong Free(void) final;
};

