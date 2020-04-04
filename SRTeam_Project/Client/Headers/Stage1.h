#pragma once
#include "Scene.h"

#include "Defines.h"
#include "Engine_Defines.h"

namespace Engine
{
	class CComponent_Manager;
	class CGameObject;
}

class CStatic_Camera;

class CStage1 :	public CScene
{
public:
	explicit CStage1(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage1() = default;

public:
	SCENETYPE	GetCurrentSceneType(void)
	{
		return m_eSceneType;
	}

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual int		Update_Scene(const _float& fTimeDelta) override;
	virtual void	Render_Scene(void) override;


private:
	HRESULT Ready_Stage1Component(void);
	HRESULT Ready_LightInfo(void);
	HRESULT Ready_UIInfo(const _tchar * pLayerTag);
	HRESULT Ready_Layer_Dynamic_Camera(const _tchar* pLayerTag, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp);
	HRESULT Ready_Layer_Static_Camera(const _tchar* pLayerTag, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY);
	HRESULT Ready_Layer_Terrain(const _tchar* pLayertag);
	HRESULT Ready_Layer_Player(const _tchar * pLayertag, const _vec3* vPosition, const _vec3* vLook);
	HRESULT Ready_Layer_Imp(const _tchar * pLayertag);
	HRESULT Ready_Layer_Demon(const _tchar * pLayertag);
	HRESULT Ready_Layer_Afrit(const _tchar * pLayertag);
	HRESULT Ready_Layer_Baron(const _tchar * pLayertag);
	HRESULT Ready_Layer_Sergeant(const _tchar * pLayertag);
	HRESULT Ready_Layer_Elemental(const _tchar * pLayertag);
	HRESULT Ready_Layer_NormalSordier(const _tchar * pLayertag);
	HRESULT Ready_Layer_Stage1_Boss(const _tchar * pLayertag);

private:
	int ChangeScene(SCENETYPE eSceneID);
	void	CameraChange(void);
	HRESULT	Ready_Monster_Component(void);
	
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
private:
	Engine::CGameObject*		 m_Dynamic_Camera = nullptr;
	Engine::CGameObject*		 m_Static_Camera = nullptr;
	Engine::CGameObject*		 m_pCamera = nullptr;
	Engine::CGameObject*		 m_pPlayer = nullptr;
private:
	SCENETYPE					 m_eSceneType;
	SCENETYPE					 m_eNewSceneType;
//bool
public:
	static CStage1* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual _ulong Free(void) final;
};

