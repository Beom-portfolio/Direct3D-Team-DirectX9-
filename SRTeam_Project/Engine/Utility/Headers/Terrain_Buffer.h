#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class DLL_EXPORT CTerrain_Buffer final : public CVIBuffer
{
private:
	explicit CTerrain_Buffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrain_Buffer(const CTerrain_Buffer& rhs);
	virtual ~CTerrain_Buffer() = default;
public:
	HRESULT Ready_Buffer(const _uint & iVtxCntX, const _uint & iVtxCntZ, const _float & fInterval);
	void Render_Buffer(void);
public:
	virtual CComponent* Clone(void);
	static CTerrain_Buffer* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iVtxCntX, const _uint& iVtxCntZ, const _float& fInterval = 1.0f);
private:
	virtual _ulong Free(void);

};

END