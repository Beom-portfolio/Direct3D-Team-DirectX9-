#pragma once

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRect_Cube;
	class CRenderer;
	class CTexture;
	class CTransform;
	class CVIBuffer;
}

class CWall_Cube final : public Engine::CGameObject
{
private:
	explicit CWall_Cube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWall_Cube() = default;
private:
	_int						m_iCubeType;
private:
	_vec3						m_vOriginPos;
	_vec3						m_vScale;
private:
	_bool						m_bDoorCheck;
private:
	_bool						m_bXZCheck;
private:
	_float						m_fDoorSpeed = 1.f;
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
	void SetDoorCheck(const _bool& iCheck)
	{
		m_bDoorCheck = iCheck;
		
		if (m_bDoorCheck == true)
		{
			if (m_iCubeType == 16)
				m_bXZCheck = true;
			else
				m_bXZCheck = false;
		}
	}
	const   _bool& GetDoorCheck(void)
	{
		return m_bDoorCheck;
	}
	const	_bool& GetDoorXZCheck(void)
	{
		return m_bXZCheck;
	}
public:
	virtual HRESULT Ready_GameObject(void) final;
	virtual _int Update_GameObject(const _float& fTimeDelta) final;
	virtual void Render_GameObject(void) final;
public:
	void TransPos_Cube(const _vec3* vPos);
	void Scaling_Cube(const _float & fX, const _float & fY, const _float & fZ);
public:
	void Open_DoorX(const _float& fTimeDelta);
	void Open_DoorZ(const _float& fTimeDelta);
	void Close_DoorX(const _float& fTimeDelta);
	void Close_DoorZ(const _float& fTimeDelta);
private:
	Engine::CRect_Cube*				m_pBufferCom = nullptr;
	Engine::CTransform*				m_pTransformCom = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CTexture*				m_pTextureCom = nullptr;
public:
	HRESULT Ready_Component(void);
public:
	static CWall_Cube* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual _ulong Free(void) final;
};
