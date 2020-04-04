#pragma once

#include "Engine_Defines.h"
#include "Component.h"
#include "VIBuffer.h"

// 월드변환행렬의 제작 변환을 지속적으로 수행할 수 있게 해준다.

BEGIN(Engine)

class DLL_EXPORT CTransform : public CComponent
{
public:
	enum INFOMATION {INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POSITION, INFO_END};
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;
public:
	const _vec3* Get_Infomation(INFOMATION eInfo) { return (_vec3*)&m_matWorld.m[eInfo][0]; }
public:	
	void SetUp_OnGraphicDev(void) { m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld); }
	void Set_Infomation(INFOMATION eInfo, const _vec3* pVector) {
		memcpy(&m_matWorld.m[eInfo][0], pVector, sizeof(_vec3));}
	void Set_Infomation(INFOMATION eInfo, const _float& fX, const _float& fY, const _float& fZ) {
		m_matWorld.m[eInfo][0] = fX; m_matWorld.m[eInfo][1] = fY; m_matWorld.m[eInfo][2] = fZ; }
	void Set_Scale(const _vec3* pScale);

public:
	_float Compute_Distance(const _matrix* pWorldMatrix);	
	void Go_Straight(const _float& fSpeed, const _float& fTimeDelta);
	void Left_Right(const _float& fSpeed, const _float& fTimeDelta);
	void Rotation_X(const _float& fAngle, const  _float& fTimeDelta);
	void Rotation_Y(const _float& fAngle, const _float& fTimeDelta);
	void Rotation_Z(const _float& fAngle, const _float& fTimeDelta);
	void Scaling(const _float& fX, const _float& fY, const _float& fZ);
	void Move_OnBuffer(Engine::CVIBuffer* pTargetBuffer);
	void Move_OnBuffer(Engine::CVIBuffer* pTargetBuffer, const _float& fHeight);
public:
	HRESULT Ready_Transform(void);

public:
	virtual void Update_Component(const _float& fTimeDelta);
private:
	_matrix				m_matWorld;
private:
	virtual CComponent* Clone(void);
public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual _ulong Free(void);
};

END