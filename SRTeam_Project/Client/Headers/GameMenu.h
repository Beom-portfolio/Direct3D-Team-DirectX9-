#pragma once

#include "Defines.h"
#include "Scene.h"

namespace Engine
{
	
}

class CGameMenu :
	public CScene
{
private:
	explicit CGameMenu(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGameMenu() = default;

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual int		Update_Scene(const _float& fTimeDelta) override;
	virtual void	Render_Scene(void) override;

private:
	HRESULT Ready_Component(void);
	HRESULT Ready_Layer_Menu(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Buttom(const _tchar* pLayerTag);
	HRESULT Ready_Proj_SetUp();
private:
	HRESULT ChangeScene(SCENETYPE eSceneID);


public:
	static CGameMenu* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual _ulong Free();
};

