#pragma once

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CTexture;
	class CRectUI_Texture;
	class CRenderer;
}

class CClient_Button : public CGameObject
{
private:
	explicit CClient_Button(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CClient_Button() = default;

public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

private:
	HRESULT Ready_Component();

public:
	static CClient_Button* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CTransform*				m_pTransformCom = nullptr;
	Engine::CTexture*				m_pTextureCom = nullptr;
	Engine::CRectUI_Texture*		m_pRectTextureCom = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;

	_float							m_fFrame = 0.f;

private:
	virtual _ulong Free();
};

