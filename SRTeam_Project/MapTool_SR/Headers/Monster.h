#pragma once

#include "GameObject.h"

namespace Engine
{
	class CRect_Cube;
	class CRenderer;
	class CTexture;
	class CTransform;
	class CVIBuffer;
}

class CMonster final : public Engine::CGameObject
{
private:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonster() = default;
private:
	_int						m_iMonType;
private:
	_vec3						m_vScale;
public:
	const _vec3&				GetMonPos(void);
	const _vec3&				GetMonScale(void)
	{
		return m_vScale;
	}
	CRect_Cube*					GetBuffer(void)
	{
		return m_pBufferCom;
	}
	const _int&					GetMonType(void)
	{
		return m_iMonType;
	}
	void SetMonType(const _int& iType)
	{
		m_iMonType = iType;
	}
public:
	virtual HRESULT Ready_GameObject(void) final;
	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject(void) final;
public:
	void TransPos_Mon(const _vec3* vPos);
	void Billboard_Mon(void);
	void Scaling_Mon(const _float & fX, const _float & fY, const _float & fZ);
private:
	Engine::CRect_Cube*				m_pBufferCom = nullptr;
	Engine::CTransform*				m_pTransformCom = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CTexture*				m_pTextureCom = nullptr;
public:
	HRESULT Ready_Component(void);
public:
	static CMonster* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual _ulong Free(void) final;
};
