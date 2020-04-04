#include "..\Headers\Transform.h"

USING(Engine)

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{

}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_matWorld(rhs.m_matWorld)
{

}

void CTransform::Set_Scale(const _vec3 * pScale)
{
	_vec3		vRight, vUp, vLook;

	// x_Scale
	memcpy(&vRight, &m_matWorld.m[INFO_RIGHT][0], sizeof(_vec3));
	D3DXVec3Normalize(&vRight, &vRight);
	vRight *= pScale->x;
	memcpy(&m_matWorld.m[INFO_RIGHT][0], &vRight, sizeof(_vec3));

	memcpy(&vUp, &m_matWorld.m[INFO_UP][0], sizeof(_vec3));
	D3DXVec3Normalize(&vUp, &vUp);
	vUp *= pScale->y;
	memcpy(&m_matWorld.m[INFO_UP][0], &vUp, sizeof(_vec3));

	memcpy(&vLook, &m_matWorld.m[INFO_LOOK][0], sizeof(_vec3));
	D3DXVec3Normalize(&vLook, &vLook);
	vLook *= pScale->z;
	memcpy(&m_matWorld.m[INFO_LOOK][0], &vLook, sizeof(_vec3));
}

_float CTransform::Compute_Distance(const _matrix * pWorldMatrix)
{
	_vec3	vDistance = *(_vec3*)&pWorldMatrix->m[INFO_POSITION][0] - *(_vec3*)&m_matWorld.m[INFO_POSITION][0];

	return D3DXVec3Length(&vDistance);
}

void CTransform::Go_Straight(const _float & fSpeed, const _float & fTimeDelta)
{
	_vec3	vDirection;
	*((_vec3*)&m_matWorld.m[INFO_POSITION][0]) += *D3DXVec3Normalize(&vDirection, (_vec3*)&m_matWorld.m[INFO_LOOK][0]) * fSpeed * fTimeDelta;
}

void CTransform::Left_Right(const _float & fSpeed, const _float & fTimeDelta)
{
	_vec3	vDirection;
	*((_vec3*)&m_matWorld.m[INFO_POSITION][0]) += *D3DXVec3Normalize(&vDirection, (_vec3*)&m_matWorld.m[INFO_RIGHT][0]) * fSpeed * fTimeDelta;
}

void CTransform::Rotation_X(const _float & fAngle, const _float & fTimeDelta)
{
	_vec3		vUp = *(_vec3*)&m_matWorld.m[INFO_UP][0];
	_vec3		vLook = *(_vec3*)&m_matWorld.m[INFO_LOOK][0];

	vUp = _vec3(vUp.x,
		vUp.y * cosf(D3DXToRadian(fAngle) * fTimeDelta)
		+ vUp.z * -sinf(D3DXToRadian(fAngle) * fTimeDelta),
		vUp.y * sinf(D3DXToRadian(fAngle) * fTimeDelta)
		+ vUp.z * cosf(D3DXToRadian(fAngle) * fTimeDelta));
	memcpy(&m_matWorld.m[INFO_UP][0], &vUp, sizeof(_vec3));

	vLook = _vec3(vLook.x,
		vLook.y * cosf(D3DXToRadian(fAngle) * fTimeDelta)
		+ vLook.z * -sinf(D3DXToRadian(fAngle) * fTimeDelta),
		vLook.y * sinf(D3DXToRadian(fAngle) * fTimeDelta)
		+ vLook.z * cosf(D3DXToRadian(fAngle) * fTimeDelta));
	memcpy(&m_matWorld.m[INFO_LOOK][0], &vLook, sizeof(_vec3));
}

void CTransform::Rotation_Y(const _float & fAngle, const _float & fTimeDelta)
{
	//Calculate without vUp Vector
	_vec3		vRight = *(_vec3*)&m_matWorld.m[INFO_RIGHT][0];
	_vec3		vLook = *(_vec3*)&m_matWorld.m[INFO_LOOK][0];

	vRight = _vec3(vRight.x * cosf(D3DXToRadian(fAngle) * fTimeDelta)
		+ vRight.z * sinf(D3DXToRadian(fAngle) * fTimeDelta),
		vRight.y,
		vRight.x * -sinf(D3DXToRadian(fAngle) * fTimeDelta)
		+ vRight.z * cosf(D3DXToRadian(fAngle) * fTimeDelta));
	memcpy(&m_matWorld.m[INFO_RIGHT][0], &vRight, sizeof(_vec3));

	vLook = _vec3(vLook.x * cosf(D3DXToRadian(fAngle) * fTimeDelta)
		+ vLook.z * sinf(D3DXToRadian(fAngle) * fTimeDelta),
		vLook.y,
		vLook.x * -sinf(D3DXToRadian(fAngle) * fTimeDelta)
		+ vLook.z * cosf(D3DXToRadian(fAngle) * fTimeDelta));
	memcpy(&m_matWorld.m[INFO_LOOK][0], &vLook, sizeof(_vec3));
}

void CTransform::Rotation_Z(const _float & fAngle, const _float & fTimeDelta)
{
	_vec3      vRight = *(_vec3*)&m_matWorld.m[INFO_RIGHT][0];
	_vec3	   vUp = *(_vec3*)&m_matWorld.m[INFO_UP][0];

	vRight = _vec3(vRight.x * cosf(D3DXToRadian(fAngle) * fTimeDelta)
		+ vRight.y * -sinf(D3DXToRadian(fAngle) * fTimeDelta),
		vRight.x * sinf(D3DXToRadian(fAngle) * fTimeDelta)
		+ vRight.y * cosf(D3DXToRadian(fAngle) * fTimeDelta),
		vRight.z);
	memcpy(&m_matWorld.m[INFO_RIGHT][0], &vRight, sizeof(_vec3));


	vUp = _vec3(vUp.x * cosf(D3DXToRadian(fAngle) * fTimeDelta)
		+ vUp.y * -sinf(D3DXToRadian(fAngle) * fTimeDelta),
		vUp.x * sinf(D3DXToRadian(fAngle) * fTimeDelta)
		+ vUp.y * cosf(D3DXToRadian(fAngle) * fTimeDelta),
		vUp.z);
	memcpy(&m_matWorld.m[INFO_UP][0], &vUp, sizeof(_vec3));
}

void CTransform::Scaling(const _float & fX, const _float & fY, const _float & fZ)
{
	_vec3			vRight, vUp, vLook;

	memcpy(&vRight, &m_matWorld.m[INFO_RIGHT][0], sizeof(_vec3));
	D3DXVec3Normalize(&vRight, &vRight);
	vRight *= fX;
	memcpy(&m_matWorld.m[INFO_RIGHT][0], &vRight, sizeof(_vec3));

	memcpy(&vUp, &m_matWorld.m[INFO_UP][0], sizeof(_vec3));
	D3DXVec3Normalize(&vUp, &vUp);
	vUp *= fY;
	memcpy(&m_matWorld.m[INFO_UP][0], &vUp, sizeof(_vec3));

	memcpy(&vLook, &m_matWorld.m[INFO_LOOK][0], sizeof(_vec3));
	D3DXVec3Normalize(&vLook, &vLook);
	vLook *= fZ;
	memcpy(&m_matWorld.m[INFO_LOOK][0], &vLook, sizeof(_vec3));
}

void CTransform::Move_OnBuffer(Engine::CVIBuffer * pTargetBuffer)
{
	if (nullptr == pTargetBuffer)
		return;

	tuple<_uint, _uint, _float>	VertexCnt = pTargetBuffer->Get_VertexCntXZItv();

	const _vec3*	pVertexPos = pTargetBuffer->Get_VertexPos();

	_vec3			vPosition;
	memcpy(&vPosition, &m_matWorld.m[3][0], sizeof(_vec3));

	_int		iCurrentIdx = _int((_int(vPosition.z) / get<2>(VertexCnt)) * (get<0>(VertexCnt)) + (_int(vPosition.x) / get<2>(VertexCnt)));

	_int		iIndex[4];
	iIndex[0] = iCurrentIdx + get<0>(VertexCnt);
	iIndex[1] = iCurrentIdx + get<0>(VertexCnt) + 1;
	iIndex[2] = iCurrentIdx + 1;
	iIndex[3] = iCurrentIdx;

	_float		fRatioX = (vPosition.x - pVertexPos[iIndex[0]].x) / get<2>(VertexCnt);
	_float		fRatioZ = (pVertexPos[iIndex[0]].z - vPosition.z) / get<2>(VertexCnt);

	_float		fY = 0.f;

	// ¿À¸¥ÂÊ À§ »ï°¢Çü³»
	if (fRatioX > fRatioZ)
		fY = pVertexPos[iIndex[0]].y + (pVertexPos[iIndex[1]].y - pVertexPos[iIndex[0]].y) * fRatioX + ((pVertexPos[iIndex[2]]).y - (pVertexPos[iIndex[1]]).y) * fRatioZ;

	// ¿ÞÂÊ ¾Æ·¡ »ï°¢Çü³»
	else
		fY = pVertexPos[iIndex[0]].y + (pVertexPos[iIndex[3]].y - pVertexPos[iIndex[0]].y) * fRatioZ + ((pVertexPos[iIndex[2]]).y - (pVertexPos[iIndex[3]]).y) * fRatioX;
	
	fY += get<2>(VertexCnt);

	m_matWorld.m[Engine::CTransform::INFO_POSITION][1] = fY;
}

void CTransform::Move_OnBuffer(Engine::CVIBuffer * pTargetBuffer, const _float& fHieght)
{
	if (nullptr == pTargetBuffer)
		return;

	tuple<_uint, _uint, _float>	VertexCnt = pTargetBuffer->Get_VertexCntXZItv();

	const _vec3*	pVertexPos = pTargetBuffer->Get_VertexPos();

	_vec3			vPosition;
	memcpy(&vPosition, &m_matWorld.m[3][0], sizeof(_vec3));

	_int		iCurrentIdx = _int((_int(vPosition.z) / get<2>(VertexCnt)) * (get<0>(VertexCnt)) + (_int(vPosition.x) / get<2>(VertexCnt)));

	_int		iIndex[4];
	iIndex[0] = iCurrentIdx + get<0>(VertexCnt);
	iIndex[1] = iCurrentIdx + get<0>(VertexCnt) + 1;
	iIndex[2] = iCurrentIdx + 1;
	iIndex[3] = iCurrentIdx;

	_float		fRatioX = (vPosition.x - pVertexPos[iIndex[0]].x) / get<2>(VertexCnt);
	_float		fRatioZ = (pVertexPos[iIndex[0]].z - vPosition.z) / get<2>(VertexCnt);

	_float		fY = 0.f;

	// ¿À¸¥ÂÊ À§ »ï°¢Çü³»
	if (fRatioX > fRatioZ)
		fY = pVertexPos[iIndex[0]].y + (pVertexPos[iIndex[1]].y - pVertexPos[iIndex[0]].y) * fRatioX + ((pVertexPos[iIndex[2]]).y - (pVertexPos[iIndex[1]]).y) * fRatioZ;

	// ¿ÞÂÊ ¾Æ·¡ »ï°¢Çü³»
	else
		fY = pVertexPos[iIndex[0]].y + (pVertexPos[iIndex[3]].y - pVertexPos[iIndex[0]].y) * fRatioZ + ((pVertexPos[iIndex[2]]).y - (pVertexPos[iIndex[3]]).y) * fRatioX;

	fY += fHieght;

	m_matWorld.m[Engine::CTransform::INFO_POSITION][1] = fY;
}

HRESULT CTransform::Ready_Transform(void)
{
	m_matWorld.Identity();
	return NOERROR;
}

void CTransform::Update_Component(const _float & fTimeDelta)
{

}

CComponent * CTransform::Clone(void)
{
	return new CTransform(*this);
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransform *		pInstance = new CTransform(pGraphicDev);

	if (FAILED(pInstance->Ready_Transform()))
	{
		MSG_BOX("CTransform Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

_ulong CTransform::Free(void)
{

	return CComponent::Free();
}
