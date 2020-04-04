#pragma once

#include "Engine_Defines.h"
#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTexture;
	class CRectUI_Texture;
	class CRenderer;
}

class CHit : public CGameObject
{
private:
	explicit CHit(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHit() = default;

public:
	void	SetHitOnScreen(const _bool& bCheck)
	{
		m_bHide = bCheck;
	}

public:
	virtual HRESULT Ready_GameObject(const _vec3* vUIPos, const _float& fWidth, const _float& fHeight, CGameObject* pPlayerObj, const _int& iUICnt);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

private:
	HRESULT Ready_Component();

public:
	static CHit* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vUIPos, const _float& fWidth, const _float& fHeight, CGameObject* pPlayerObj, const _int& iUICnt);

private:
	Engine::CTexture*				m_pTextureCom = nullptr;
	Engine::CRectUI_Texture*		m_pRectTextureCom = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;
private:
	const Engine::CGameObject*			m_pPlayerObj = nullptr;
private:
	_vec3							m_Pos;
	_float							m_fWidth;
	_float							m_fHeight;
	_int							m_UICnt;
	_bool							m_bHide;
	_int							m_iDelay;

private:
	virtual _ulong Free();
};

