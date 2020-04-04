#pragma once


namespace Engine
{

	// For.Vertex Struct
	typedef struct Vertex_Color
	{
		
		D3DXVECTOR3		vPosition;	
		unsigned long	dwColor;

	}VTXCOL;

	typedef struct Vertex_Texture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR2		vTexUV;
	}VTXTEX;

	typedef struct Vertex_Texture_ViewPort
	{
		D3DXVECTOR4		vPosition; // D3DFVF_XYZRHW(모든 렌더링파이프라인을 거친 이후의 정점 위치)
		D3DXVECTOR2		vTexUV; // D3DFVF_TEX1
	}VTXTEX_VIEWPORT;

	typedef struct Vertex_CubeTexture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vTexUV;
	}VTXCUBETEX;


	// For.Index Struct
	typedef struct Index_16
	{
		unsigned short _0, _1, _2;
	}INDEX16;

	typedef struct Index_32
	{
		unsigned long _0, _1, _2;
	}INDEX32;

	// For.Save Load
	typedef struct IOFileTerrain
	{
		int		iX;
		int		iZ;
		float	iItv;
	}TERRAIN_DATA;

	typedef struct IOFileCube
	{
		wchar_t			strName[255];
		D3DXVECTOR3		vPos;
		D3DXVECTOR3		vScale;
		int				iType;
		bool			bDoorCheck;
	}CUBE_DATA;

	typedef struct IOFileObj
	{
		wchar_t			strName[255];
		D3DXVECTOR3		vPos;
		D3DXVECTOR3		vScale;
		int				iType;
	}OBJ_DATA;

	typedef struct IOFileMon
	{
		wchar_t			strName[255];
		D3DXVECTOR3		vPos;
		D3DXVECTOR3		vScale;
		int				iType;
	}MON_DATA;

	typedef struct IOFileItem
	{
		wchar_t			strName[255];
		D3DXVECTOR3		vPos;
		D3DXVECTOR3		vScale;
		int				iType;
	}ITEM_DATA;

	typedef struct tagInfo
	{
		_float		fCollisionSize;
		_float		fCollisionSearchSize;
		_float		fCollisionAtkSearchSize;

		_float		fSpeed;

		_int		iHp;
		_int		iHpMax;
		_int		iArmor;
		_int		iAtk;

		_bool		bDie;
		_bool		bHit;
		_bool		bAtk;
		_bool		bSearch;
		_bool		bAtkSearch;
		
		_bool		bEffectOn;

	} INFO, *pINFO, *PINFO;
}