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

class CFace : public CGameObject
{
public:
	enum CHANGE_FACE {FACE_ZERO, FACE_ONE, FACE_TWO, FACE_THREE, FACE_FOUR, FACE_FIVE, FACE_END};

private:
	explicit CFace(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFace() = default;

public:
	virtual HRESULT Ready_GameObject(const _vec3* vUIPos, const _float& fWidth, const _float& fHeight, CGameObject* pPlayerObj);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

private:
	HRESULT Ready_Component();

public:
	static CFace* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vUIPos, const _float& fWidth, const _float& fHeight, CGameObject* pPlayerObj);

private:
	Engine::CTexture*				m_pTextureCom = nullptr;
	Engine::CRectUI_Texture*		m_pRectTextureCom = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;
private:
	Engine::CGameObject*			m_pPlayerObj = nullptr;
private:
	_vec3							m_Pos;
	_float							m_fWidth;
	_float							m_fHeight;
	_int							m_UICnt;
	CHANGE_FACE						m_Face;


private:
	virtual _ulong Free();
};

