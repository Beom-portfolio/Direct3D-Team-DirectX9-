#pragma once

#include "Defines.h"

#include "Camera.h"


class CStatic_Camera : public CCamera
{
private:
	explicit CStatic_Camera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStatic_Camera() = default;


public:
	virtual HRESULT Ready_GameObject(const _vec3* vEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect, const _float& fNaer, const _float& fFar);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);

private:
	void PlayerCamera(void);
	void Fixed_MousePointer(const _int& iConterBackSizeX, const _int& iConterBackSizeY);
	
public:
	static CStatic_Camera* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* vEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY = D3DXToRadian(60.f), const _float& fAspect = _float(WINCX) / _float(WINCY), const _float& fNear = 0.2f, const _float& fFar = 1000.f);

public:
	bool			m_CameraOn = true;

private:
	virtual _ulong Free() final;

};

