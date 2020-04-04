#pragma once

#include "GameObject.h"

namespace Engine
{
	class CRect_Texture;
	class CRenderer;
	class CTexture;
	class CTransform;
	class CVIBuffer;
}

class CBack_Object final : public Engine::CGameObject
{
private:
	explicit CBack_Object(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBack_Object() = default;
private:
	_int							m_iObjType;
private:
	_vec3							m_vScale;
public:
	const _vec3&					GetObjPos(void);
	const _vec3&					GetObjScale(void)
	{
		return m_vScale;
	}
	CRect_Texture*					GetBuffer(void)
	{
		return m_pBufferCom;
	}
	const _int&						GetObjType(void)
	{
		return m_iObjType;
	}
	void SetObjType(const _int& iType)
	{
		m_iObjType = iType;
	}
public:
	virtual HRESULT Ready_GameObject(void) final;
	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject(void) final;
public:
	void TransPos_Obj(const _vec3* vPos);
	void Billboard_Obj(void);
	void Scaling_Obj(const _float & fX, const _float & fY, const _float & fZ);
private:
	Engine::CRect_Texture*			m_pBufferCom = nullptr;
	Engine::CTransform*				m_pTransformCom = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CTexture*				m_pTextureCom = nullptr;
public:
	HRESULT Ready_Component(void);
public:
	static CBack_Object* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual _ulong Free(void) final;
};

