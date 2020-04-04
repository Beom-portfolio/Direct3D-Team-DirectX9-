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


class CCube final : public Engine::CGameObject
{
private:
	explicit CCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCube() = default;
private:
	_int						m_iCubeType;
private:
	_vec3						m_vScale;
public:
	const _vec3&				GetCubePos(void);
	const _vec3&				GetCubeScale(void)
	{
		return m_vScale;
	}
	CRect_Cube*					GetBuffer(void)
	{
		return m_pBufferCom;
	}
	const _int&					GetCubeType(void)
	{
		return m_iCubeType;
	}
	void SetCubeType(const _int& iType)
	{
		m_iCubeType = iType;
	}
public:
	virtual HRESULT Ready_GameObject(void) final;
	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject(void) final;
public:
	void TransPos_Cube(const _vec3* vPos);
	void Scaling_Cube(const _float & fX, const _float & fY, const _float & fZ);
private:
	Engine::CRect_Cube*				m_pBufferCom = nullptr;
	Engine::CTransform*				m_pTransformCom = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CTexture*				m_pTextureCom = nullptr;
public:
	HRESULT Ready_Component(void);
public:
	static CCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual _ulong Free(void) final;
};

