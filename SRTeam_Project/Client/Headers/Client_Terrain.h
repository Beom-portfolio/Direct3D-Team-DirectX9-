#pragma once

#include "defines.h"
#include "Engine_Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTerrain_Buffer;
	class CRenderer;
	class CTransform;
	class CTexture;
}

class CClient_Terrain : public CGameObject
{
private:
	explicit CClient_Terrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CClient_Terrain() = default;

public:
	virtual HRESULT Ready_GameObject(void) final;
	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject(void) final;

public:
	HRESULT Ready_Component(void);

public:
	static CClient_Terrain* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual _ulong Free(void) final;

private:
	Engine::CTransform*				m_pTransformCom = nullptr;
	Engine::CTerrain_Buffer*		m_pBufferCom = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CTexture*				m_pTextureCom = nullptr;



};

 