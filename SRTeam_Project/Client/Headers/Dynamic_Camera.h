#pragma once
#include "Defines.h"

#include "Camera.h"


class CDynamic_Camera : public CCamera
{
private:
	explicit CDynamic_Camera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamic_Camera() = default;
public:
	virtual HRESULT Ready_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp
		, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;

private:
	void Camera_Controler(const _float& fTimeDelta);
	void Fixed_MousePointer(const _int& iConterBackSizeX, const _int& iConterBackSizeY);

public:
	static CDynamic_Camera*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float & fFovY = D3DXToRadian(60.f), const _float & fAspect = _float(WINCX) / _float(WINCY), const _float & fNear = 0.2f, const _float & fFar = 1000.f);


private:
	float			m_fCameraSpeed = 10.f;
public:
	bool			m_CameraOn = true;

private:
	virtual _ulong Free() final;

};

