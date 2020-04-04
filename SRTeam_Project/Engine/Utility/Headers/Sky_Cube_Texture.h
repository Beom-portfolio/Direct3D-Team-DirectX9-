#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class DLL_EXPORT CSky_Cube_Texture final : public CVIBuffer
{
public:
	explicit CSky_Cube_Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSky_Cube_Texture(const CSky_Cube_Texture& rhs);
	virtual ~CSky_Cube_Texture();

public:
	HRESULT Ready_Buffer(void);
	void Render_Buffer(void);
public:
	virtual CComponent* Clone(void) final;
public:
	static CSky_Cube_Texture* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual _ulong Free(void) final;

};

END

