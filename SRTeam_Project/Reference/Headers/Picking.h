#pragma once

#include "Component.h"

BEGIN(Engine)

class CVIBuffer;
class DLL_EXPORT CPicking : public CComponent
{
private:
	explicit CPicking(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPicking(const CPicking& rhs);
	virtual ~CPicking() = default;
public:
	HRESULT Ready_Picking(void);
	_vec3* Picking_ToBuffer(HWND hWnd, Engine::CVIBuffer *pTargetBuffer, const _matrix* pWorldMatrix = nullptr);
	_vec3* Picking_ForCubeTool(HWND hWnd, Engine::CVIBuffer *pTargetBuffer, const _matrix* pWorldMatrix = nullptr);
public:
	virtual CComponent* Clone(void);
public:
	static CPicking* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual _ulong Free(void);

private:
	_vec3*	m_vTempVec = nullptr;
};

END

