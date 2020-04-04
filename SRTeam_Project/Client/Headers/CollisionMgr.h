#pragma once

#include "Base.h"
#include "Engine_Defines.h"
#include "Defines.h"
#include "GameObject.h"

class CCollisionMgr : public CBase
{

public:
	explicit CCollisionMgr();
	virtual ~CCollisionMgr() = default;

public:
	static void CollisionMonsterHit_Circle(list<CGameObject*>* pDstList, list<CGameObject*>* pSrcList);
	static void CollisionPlayerHit_Circle(list<CGameObject*>* pDstList, list<CGameObject*>* pSrcList);
	static void CollisionMonsterSearch_Circle(list<CGameObject*>* pDstList, list<CGameObject*>* pSrcList);
	static void CollisionMonsterAtkSearch_Circle(list<CGameObject*>* pDstList, list<CGameObject*>* pSrcList);
	static void CollisionWall_Rect(list<CGameObject*>* pDstList, list<CGameObject*>* pSrcList, const _float& fTimeDelta);
	static void CollisionBulletWall_Rect(list<CGameObject*>* pDstList, list<CGameObject*>* pSrcList);
	static void CollisionHitItem_Circle(list<CGameObject*>* pDstList, list<CGameObject*>* pSrcList);
	static void Collision_WithMonster_Monster_Circle(list<CGameObject*>* pDstList, list<CGameObject*>* pSrcList);
	static void Collision_WithWall_Bullet_Circle(list<CGameObject*>* pDstList, list<CGameObject*>* pSrcList);
};

