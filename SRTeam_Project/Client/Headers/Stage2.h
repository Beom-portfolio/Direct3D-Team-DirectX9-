#pragma once
#include "Scene.h"

#include "Defines.h"
#include "Engine_Defines.h"

namespace Engine
{
	class CComponent_Manager;
	class CGameObject;
}

class CStage2 :
	public CScene
{
public:
	explicit CStage2(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pPlayer);
	virtual ~CStage2() = default;

	SCENETYPE		GetCurrentSceneType(void)
	{
		return		m_eSceneType;
	}

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual int		Update_Scene(const _float& fTimeDelta) override;
	virtual void	Render_Scene(void) override;

private:
	HRESULT Ready_Stage2Component(void);
	HRESULT Ready_MonsterComponent(void);
	HRESULT Ready_Layer_Player(const _vec3 * vPosition, const _vec3 * vLook);
	HRESULT Ready_Layer_Static_Camera(const _tchar * pLayerTag, const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const _float& fFovY);
	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Imp(const _tchar * pLayertag);
	HRESULT Ready_Layer_Stage2_Boss(const _tchar * pLayertag);

private:
	HRESULT ChangeScene(SCENETYPE eSceneID);

	//For.Mapping -Bum
	void    LoadMap(void);
	void	Change_Terrain(const _int & iX, const _int & iZ, const _float & fItv);
	void	Add_WallCube(const _vec3* vPos, const _vec3* vScale, const _int& iType, const _bool& iDoorCheck);
	void    Add_Obj(const _vec3* vPos, const _vec3* vScale, const _int& iType);
	void    Add_Item(const _vec3* vPos, const _vec3* vScale, const _int& iType);
	void    Add_Monster(const _tchar* tagLayer, const _vec3* vPos, const _vec3* vScale, const _int& iType);

	//For.Create Monster
	void    Add_Imp(const _tchar* tagLayer, const _vec3* vPos, const _vec3* vScale);
	void    Add_Demon(const _tchar* tagLayer, const _vec3* vPos, const _vec3* vScale);
	void    Add_Deco(const _tchar* tagLayer, const _vec3* vPos, const _vec3* vScale);
	void    Add_Afrit(const _tchar* tagLayer, const _vec3* vPos, const _vec3* vScale);
	void    Add_Baron(const _tchar* tagLayer, const _vec3* vPos, const _vec3* vScale);
	void    Add_Segerent(const _tchar* tagLayer, const _vec3* vPos, const _vec3* vScale);
	void    Add_Element(const _tchar* tagLayer, const _vec3* vPos, const _vec3* vScale);
	void    Add_NormSoldier(const _tchar* tagLayer, const _vec3* vPos, const _vec3* vScale);

public:
	static CStage2* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pPlayer);

private:
	virtual _ulong Free(void) final;


private:
	Engine::CGameObject*		 m_Static_Camera = nullptr;
	Engine::CGameObject*		 m_pPlayer = nullptr;

private:
	SCENETYPE					 m_eSceneType;

	bool						m_bSetup_StartPos = true;
};

