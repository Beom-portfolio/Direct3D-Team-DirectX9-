#pragma once
#include "Scene.h"

#include "Defines.h"
#include "Engine_Defines.h"

namespace Engine
{
	class CComponent_Manager;
}

class CStage3 :	public CScene
{
public:
	explicit CStage3(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage3() = default;

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual int		Update_Scene(const _float& fTimeDelta) override;
	virtual void	Render_Scene(void) override;

private:
	HRESULT Ready_Stage3Component(void);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp);
	HRESULT Ready_Layer_Terrain(const _tchar* pLayertag);

private:
	HRESULT ChangeScene(SCENETYPE eSceneID);

public:
	static CStage3* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual _ulong Free(void) final;
};

