#pragma once

#include "Engine_Defines.h"
#include "Scene_Tool.h"

namespace Engine
{
	class CPicking;
	class CVIBuffer;
	class CObject_Manager;
}

class CScene_Logo : public Engine::CScene_Tool
{
private:
	explicit CScene_Logo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene_Logo() = default;
public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene(void) override;
private:
	HRESULT Ready_LogoComponent(void);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
private:
	Engine::CPicking*				m_pPickingCom = nullptr;
	Engine::CVIBuffer*				m_pTargetBufferCom = nullptr;
private:
	Engine::CObject_Manager*		m_pObjectMgr = nullptr;
private:
	_vec3							m_vPos;
public:
	//For.Cube
	void Add_Cube(const _tchar* pLayerTag
				  , const _float& fX
				  , const _float& fY
				  , const _float& fZ
				  , const _vec3* vPos = &_vec3(0.f, 0.f, 0.f)
				  , const bool& bLoadCheck = false
				  , const _int&	iType = 0);
	void Del_Cube(const _tchar* pLayerTag);
	//For.Object
	void Add_Obj(const _tchar* pLayerTag
				, const _vec3* vPos
				, const bool& bLoadCheck = false
				, const _int&	iType = 0);
	void Del_Obj(const _tchar* pLayerTag);
	//For.Monster
	void Add_Mon(const _tchar* pLayerTag
				, const _float& fX
				, const _float& fY
				, const _float& fZ
				, const _vec3* vPos = &_vec3(0.f, 0.f, 0.f)
				, const bool& bLoadCheck = false
				, const _int&	iType = 0);
	void Del_Mon(const _tchar* pLayerTag);
	//For.Item
	void Add_Item(const _tchar* pLayerTag
		, const _float& fX
		, const _float& fY
		, const _float& fZ
		, const _vec3* vPos
		, const bool& bLoadCheck = false
		, const _int&	iType = 0);
	void Del_Item(const _tchar* pLayerTag);
	void Change_Terrain(const _int& iX, const _int& iZ, const _float & fItv = 1.f);
public:
	const _vec3& GetPickingPos(void);
	void SetBufferTarget(void);
	void Picking_Cube(void);
	void Picking_Obj(void);
	void Picking_Mon(void);
	void Picking_Item(void);
private:
	HRESULT Change_Scene(SCENETYPE eSceneID);
public:
	static CScene_Logo* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual _ulong Free(void) override;
};

