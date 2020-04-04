#pragma once

#include "Engine_Defines.h"
#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CTexture;
	class CRectUI_Texture;
	class CRenderer;
}

class CClient_Menu : public CGameObject
{
private:
	explicit CClient_Menu(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CClient_Menu() = default;

public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

private:
	HRESULT Ready_Component();

public:
	static CClient_Menu* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CTransform*				m_pTransformCom		= nullptr;
	Engine::CTexture*				m_pTextureCom		= nullptr;
	Engine::CRectUI_Texture*		m_pRectTextureCom	= nullptr;
	Engine::CRenderer*				m_pRendererCom		= nullptr;
private:
	_float							m_fFrame;


private:
	virtual _ulong Free();
};

