#pragma once

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRectUI_Texture;
	class CRenderer;
	class CTexture;
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

public:
	const _bool& Get_FrameEnd() {
		return m_bLonogoEnd;
	}
private:
	Engine::CRectUI_Texture*		m_pBufferCom = nullptr; 
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CTexture*				m_pTextureCom = nullptr;

	_float							m_fFrame = 0.f;
	_float							m_fStack = 0.f;
	_bool							m_bLonogoEnd = false;
public:
	HRESULT Ready_Component(void);
public:
	static CBack_Logo* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual _ulong Free(void) final;

};

