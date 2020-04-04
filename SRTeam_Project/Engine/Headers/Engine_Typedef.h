#pragma once

namespace Engine
{
	typedef unsigned char			_ubyte;
	typedef signed char				_byte;

	typedef unsigned short			_ushort;
	typedef signed short			_short;

	typedef unsigned int			_uint;
	typedef signed int				_int;

	typedef unsigned long			_ulong;
	typedef signed long				_long;

	typedef float					_float;
	typedef double					_double;

	typedef wchar_t					_tchar;

	typedef bool					_bool;

	typedef D3DXVECTOR2				_vec2;

	typedef D3DXVECTOR3				_vec3;

	//typedef struct MYVECTOR3 : public D3DXVECTOR3
	//{
	//	float Get_Length();
	//	float Get_Distance(cosnt D3DXVECTOR3* pTargetVector);
	//	D3DXVECTOR3 Get_Direction(cosnt D3DXVECTOR3* pTargetVector);
	//	D3DXVECTOR3 Get_Normalize()
	//}_vec3;

	typedef D3DXVECTOR4				_vec4;
	//typedef D3DXMATRIX				_matrix;

	typedef struct D3DXMATRIX_DERIVED : public D3DXMATRIX
	{
	public:
		D3DXMATRIX_DERIVED* Identity(void)
		{
			m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][2] = m[1][3] = m[2][0] = m[2][1] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0.0f;
			m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;

			return this;
		}

	}_matrix;
}