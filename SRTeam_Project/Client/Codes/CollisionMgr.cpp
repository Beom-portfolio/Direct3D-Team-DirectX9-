#include "stdafx.h"
#include "..\Headers\CollisionMgr.h"

#include "Transform.h"
#include "Wall_Cube.h"
#include "Client_Player.h"
#include "Item.h"

CCollisionMgr::CCollisionMgr()
{
}

void CCollisionMgr::CollisionMonsterHit_Circle(list<CGameObject*>* pDstList, list<CGameObject*>* pSrcList)
{
	list<CGameObject*>::iterator dst_begin = pDstList->begin();
	list<CGameObject*>::iterator dst_end = pDstList->end();

	for (; dst_begin != dst_end; ++dst_begin)
	{
		if (true == (*dst_begin)->Get_Info().bDie)
			continue;


		list<CGameObject*>::iterator src_begin = pSrcList->begin();
		list<CGameObject*>::iterator src_end = pSrcList->end();

		for (; src_begin != src_end; ++src_begin)
		{

			if (true == (*src_begin)->Get_Info().bDie)
				continue;


			CComponent* pMonster_Component = (*dst_begin)->Get_Component(L"Transform_com");
			CComponent* pPlayerBullet_Component = (*src_begin)->Get_Component(L"Transform_com");

			const _vec3* pMonsterPos = dynamic_cast<CTransform*>(pMonster_Component)->Get_Infomation(CTransform::INFO_POSITION);
			const _vec3* pPlayerBulletPos = dynamic_cast<CTransform*>(pPlayerBullet_Component)->Get_Infomation(CTransform::INFO_POSITION);
			_vec3 vMonsterPos = *pMonsterPos;
			_vec3 vPlayerPos = *pPlayerBulletPos;


			float fSum = (*dst_begin)->Get_Info().fCollisionSize / 2 + (*src_begin)->Get_Info().fCollisionSize / 2;
			float fX = vMonsterPos.x - vPlayerPos.x;
			float fZ = vMonsterPos.z - vPlayerPos.z;
			float fHypo = sqrtf(fX * fX + fZ * fZ);

			if (fSum >= fHypo)
			{
				int iAtk = 0;
				iAtk = (*src_begin)->Get_Info().iAtk;
				(*dst_begin)->Set_bHit(true);
				(*dst_begin)->Set_iHp(-iAtk);
				(*src_begin)->Set_iHp(-1);
				(*src_begin)->Set_bEffectOn(true);
				(*dst_begin)->Set_bEffectOn(true);
			}
			else if (fSum < fHypo)
			{
			}
		}
	}

}

void CCollisionMgr::CollisionPlayerHit_Circle(list<CGameObject*>* pDstList, list<CGameObject*>* pSrcList)
{
	list<CGameObject*>::iterator dst_begin = pDstList->begin();
	list<CGameObject*>::iterator dst_end = pDstList->end();

	for (; dst_begin != dst_end; ++dst_begin)
	{
		if (true == (*dst_begin)->Get_Info().bDie)
			continue;


		list<CGameObject*>::iterator src_begin = pSrcList->begin();
		list<CGameObject*>::iterator src_end = pSrcList->end();

		for (; src_begin != src_end; ++src_begin)
		{

			if (true == (*src_begin)->Get_Info().bDie)
				continue;


			CComponent* pPlayer_Component = (*dst_begin)->Get_Component(L"Transform_com");
			CComponent* pMonsterBullet_Component = (*src_begin)->Get_Component(L"Transform_com");

			const _vec3* pPlayerPos = dynamic_cast<CTransform*>(pPlayer_Component)->Get_Infomation(CTransform::INFO_POSITION);
			const _vec3*pMonsterBulletPos = dynamic_cast<CTransform*>(pMonsterBullet_Component)->Get_Infomation(CTransform::INFO_POSITION);
			_vec3 vPlayerPos = *pPlayerPos;
			_vec3 vMonsterBulletPos = *pMonsterBulletPos;


			float fSum = (*dst_begin)->Get_Info().fCollisionSize / 2 + (*src_begin)->Get_Info().fCollisionSize / 2;
			float fX = vPlayerPos.x - vMonsterBulletPos.x;
			float fZ = vPlayerPos.z - vMonsterBulletPos.z;
			float fHypo = sqrtf(fX * fX + fZ * fZ);

			if (fSum >= fHypo)
			{

				dynamic_cast<Client_Player*>(*dst_begin)->Player_Hit();

				int iAtk = 0;


				iAtk = (*src_begin)->Get_Info().iAtk;

				//int iArmor = (*dst_begin)->Get_Info().iArmor;
				//iArmor = iArmor - (iAtk *  iArmor / 100);
				//(*dst_begin)->Set_iArmor(iArmor - 10);

				if (0 < (*dst_begin)->Get_Info().iArmor)
				{
					int iGuardAtk = (int)(iAtk / 3);
					(*dst_begin)->Set_iArmor(iGuardAtk - iAtk);
					(*dst_begin)->Set_bHit(true);
					(*dst_begin)->Set_iHp(-iGuardAtk);
					(*src_begin)->Set_iHp(-1);
				}
				else
				{
					if (0 >= (*dst_begin)->Get_Info().iHp)
					{
						(*dst_begin)->Set_iHp(100);
					}
					(*dst_begin)->Set_bHit(true);
					(*dst_begin)->Set_iHp(-iAtk);
					(*src_begin)->Set_iHp(-1);
				}
			}
			else if (fSum < fHypo)
			{
			}
		}
	}
}

void CCollisionMgr::CollisionMonsterSearch_Circle(list<CGameObject*>* pDstList, list<CGameObject*>* pSrcList)
{
	list<CGameObject*>::iterator dst_begin = pDstList->begin();
	list<CGameObject*>::iterator dst_end = pDstList->end();

	for (; dst_begin != dst_end; ++dst_begin)
	{
		if (true == (*dst_begin)->Get_Info().bDie)
			continue;


		list<CGameObject*>::iterator src_begin = pSrcList->begin();
		list<CGameObject*>::iterator src_end = pSrcList->end();

		for (; src_begin != src_end; ++src_begin)
		{

			if (true == (*src_begin)->Get_Info().bDie)
				continue;


			CComponent* pMonster_Component = (*dst_begin)->Get_Component(L"Transform_com");
			CComponent* pPlayerBullet_Component = (*src_begin)->Get_Component(L"Transform_com");

			if (nullptr == pMonster_Component || nullptr == pPlayerBullet_Component)
				continue;

			const _vec3* pMonsterPos = dynamic_cast<CTransform*>(pMonster_Component)->Get_Infomation(CTransform::INFO_POSITION);
			const _vec3* pPlayerBulletPos = dynamic_cast<CTransform*>(pPlayerBullet_Component)->Get_Infomation(CTransform::INFO_POSITION);
			_vec3 vMonsterPos = *pMonsterPos;
			_vec3 vPlayerPos = *pPlayerBulletPos;


			float fSum = (*dst_begin)->Get_Info().fCollisionSearchSize / 2 + (*src_begin)->Get_Info().fCollisionSearchSize / 2;
			float fX = vPlayerPos.x - vMonsterPos.x;
			float fZ = vPlayerPos.z - vMonsterPos.z;
			float fHypo = sqrtf(fX * fX + fZ * fZ);

			if (fSum >= fHypo)
			{
				(*src_begin)->Set_bSearch(true);
			}
			else if (fSum < fHypo)
			{
			}
		}
	}
}

void CCollisionMgr::CollisionMonsterAtkSearch_Circle(list<CGameObject*>* pDstList, list<CGameObject*>* pSrcList)
{
	list<CGameObject*>::iterator dst_begin = pDstList->begin();
	list<CGameObject*>::iterator dst_end = pDstList->end();

	for (; dst_begin != dst_end; ++dst_begin)
	{
		if (true == (*dst_begin)->Get_Info().bDie)
			continue;


		list<CGameObject*>::iterator src_begin = pSrcList->begin();
		list<CGameObject*>::iterator src_end = pSrcList->end();

		for (; src_begin != src_end; ++src_begin)
		{

			if (true == (*src_begin)->Get_Info().bDie)
				continue;


			CComponent* pMonster_Component = (*dst_begin)->Get_Component(L"Transform_com");
			CComponent* pPlayerBullet_Component = (*src_begin)->Get_Component(L"Transform_com");

			if (nullptr == pMonster_Component || nullptr == pPlayerBullet_Component)
				continue;

			const _vec3* pMonsterPos = dynamic_cast<CTransform*>(pMonster_Component)->Get_Infomation(CTransform::INFO_POSITION);
			const _vec3* pPlayerBulletPos = dynamic_cast<CTransform*>(pPlayerBullet_Component)->Get_Infomation(CTransform::INFO_POSITION);
			_vec3 vMonsterPos = *pMonsterPos;
			_vec3 vPlayerPos = *pPlayerBulletPos;


			float fSum = (*dst_begin)->Get_Info().fCollisionAtkSearchSize / 2 + (*src_begin)->Get_Info().fCollisionAtkSearchSize / 2 - 0.5f;
			float fX = vPlayerPos.x - vMonsterPos.x;
			float fZ = vPlayerPos.z - vMonsterPos.z;
			float fHypo = sqrtf(fX * fX + fZ * fZ);

			if (fSum >= fHypo)
			{
				(*src_begin)->Set_bAtkSearch(true);
			}
			else if (fSum < fHypo)
			{
				(*src_begin)->Set_bAtkSearch(false);
			}
		}
	}
}

void CCollisionMgr::CollisionWall_Rect(list<CGameObject*>* pDstList, list<CGameObject*>* pSrcList, const _float& fTimeDelta)
{
	list<CGameObject*>::iterator dst_begin = pDstList->begin();
	list<CGameObject*>::iterator dst_end = pDstList->end();

	for (; dst_begin != dst_end; ++dst_begin)
	{
		if (true == (*dst_begin)->Get_Info().bDie)
			continue;


		list<CGameObject*>::iterator src_begin = pSrcList->begin();
		list<CGameObject*>::iterator src_end = pSrcList->end();

		for (; src_begin != src_end; ++src_begin)
		{

			if (true == (*src_begin)->Get_Info().bDie)
				continue;


			CComponent* pWall_Component = (*dst_begin)->Get_Component(L"Transform_com");
			CComponent* pPlayer_Component = (*src_begin)->Get_Component(L"Transform_com");

			if (nullptr == pWall_Component || nullptr == pPlayer_Component)
				continue;

			const _vec3* pWallPos = dynamic_cast<CTransform*>(pWall_Component)->Get_Infomation(CTransform::INFO_POSITION);
			const _vec3* pPlayerPos = dynamic_cast<CTransform*>(pPlayer_Component)->Get_Infomation(CTransform::INFO_POSITION);
			_vec3 vWallPos = *pWallPos;
			_vec3 vPlayer = *pPlayerPos;

			float fWidth_Door = ((*dst_begin)->Get_Info().fCollisionSize / 2.f) + ((*src_begin)->Get_Info().fCollisionSearchSize / 2.f) ;
			float fHeight_Door = ((*dst_begin)->Get_Info().fCollisionSize / 2.f) + ((*src_begin)->Get_Info().fCollisionSearchSize / 2.f);

			float fWidth = ((*dst_begin)->Get_Info().fCollisionSize / 2.f) + ((*src_begin)->Get_Info().fCollisionSearchSize / 2.f) - 0.1f;
			float fHeight = ((*dst_begin)->Get_Info().fCollisionSize / 2.f) + ((*src_begin)->Get_Info().fCollisionSearchSize / 2.f) - 0.1f;

			float fDistX = abs(vWallPos.x - vPlayer.x);
			float fDistZ = abs(vWallPos.z - vPlayer.z);

			if ((fWidth_Door > fDistX) && (fHeight_Door > fDistZ))
			{
				//충돌됫을때

				bool bDoor_Check = dynamic_cast<CWall_Cube*>((*dst_begin))->GetDoorCheck();
				//벽들 중에 문인지 아닌지 체크
				if (true == bDoor_Check)
				{
					bool bDoor_Type = dynamic_cast<CWall_Cube*>((*dst_begin))->GetDoorXZCheck();

					if (true == bDoor_Type)
						dynamic_cast<CWall_Cube*>(*dst_begin)->Open_DoorZ(fTimeDelta);
					else
						dynamic_cast<CWall_Cube*>(*dst_begin)->Open_DoorX(fTimeDelta);
				}
			}
			else
			{
				dynamic_cast<CWall_Cube*>(*dst_begin)->Close_DoorX(fTimeDelta);
				dynamic_cast<CWall_Cube*>(*dst_begin)->Close_DoorZ(fTimeDelta);
			}
			
			if ((fWidth > fDistX) && (fHeight > fDistZ))
			{
				//충돌됫을때
				float fMoveX = fWidth - fDistX;
				float fMoveZ = fHeight - fDistZ;

				if (fMoveX > fMoveZ)
				{
					float fX = vPlayer.x;
					float fZ = vPlayer.z;

					if (vWallPos.z > fZ)
						fMoveZ *= -1;

					_vec3 vPlayerPos = _vec3(fX, vPlayer.y, fZ + fMoveZ);
					dynamic_cast<CTransform*>(pPlayer_Component)->Set_Infomation(CTransform::INFO_POSITION, &vPlayerPos);
				}
				else
				{
					float fX = vPlayer.x;
					float fZ = vPlayer.z;

					if (vWallPos.x > fX)
						fMoveX *= -1;

					_vec3 vPlayerPos = _vec3(fX + fMoveX, vPlayer.y, fZ);
					dynamic_cast<CTransform*>(pPlayer_Component)->Set_Infomation(CTransform::INFO_POSITION, &vPlayerPos);
				}
			}
			else
			{
				//충돌 안됬을때.

			}

		}

	}
}

void CCollisionMgr::CollisionBulletWall_Rect(list<CGameObject*>* pDstList, list<CGameObject*>* pSrcList)
{
}

void CCollisionMgr::CollisionHitItem_Circle(list<CGameObject*>* pDstList, list<CGameObject*>* pSrcList)
{
	list<CGameObject*>::iterator dst_begin = pDstList->begin();
	list<CGameObject*>::iterator dst_end = pDstList->end();

	for (; dst_begin != dst_end; ++dst_begin)
	{
		if (true == (*dst_begin)->Get_Info().bDie)
			continue;


		list<CGameObject*>::iterator src_begin = pSrcList->begin();
		list<CGameObject*>::iterator src_end = pSrcList->end();

		for (; src_begin != src_end; ++src_begin)
		{
			if (true == (*src_begin)->Get_Info().bDie)
				continue;

			CComponent* pPlayer_Component = (*dst_begin)->Get_Component(L"Transform_com");
			CComponent* pItem_Component   = (*src_begin)->Get_Component(L"Transform_com");

			const _vec3* pPlayerPos = dynamic_cast<CTransform*>(pPlayer_Component)->Get_Infomation(CTransform::INFO_POSITION);
			const _vec3* ItemPos = dynamic_cast<CTransform*>(pItem_Component)->Get_Infomation(CTransform::INFO_POSITION);
			_vec3 vPlayerPos = *pPlayerPos;
			_vec3 vItemPos = *ItemPos;


			float fSum = (*dst_begin)->Get_Info().fCollisionSize / 2.f + (*src_begin)->Get_Info().fCollisionSize / 2.f;
			float fX = vPlayerPos.x - vItemPos.x;
			float fZ = vPlayerPos.z - vItemPos.z;
			float fHypo = sqrtf(fX * fX + fZ * fZ);

			if (fSum >= fHypo)
			{
				//충돌
				dynamic_cast<Client_Player*>(*dst_begin)
					->Earned_Item(dynamic_cast<CItem*>(*src_begin)->GetItemType());
				
				//아이템 사라짐
				(*src_begin)->Set_iHp(-1);
			}
			else if (fSum < fHypo)
			{
				//충돌X
			}
		}
	}
}

void CCollisionMgr::Collision_WithMonster_Monster_Circle(list<CGameObject*>* pDstList, list<CGameObject*>* pSrcList)
{
	list<CGameObject*>::iterator dst_begin = pDstList->begin();
	list<CGameObject*>::iterator dst_end = pDstList->end();

	for (; dst_begin != dst_end; ++dst_begin)
	{
		if (true == (*dst_begin)->Get_Info().bDie)
			continue;


		list<CGameObject*>::iterator src_begin = pSrcList->begin();
		list<CGameObject*>::iterator src_end = pSrcList->end();

		for (; src_begin != src_end; ++src_begin)
		{

			if (true == (*src_begin)->Get_Info().bDie)
				continue;


			CComponent* pTargetMonster_Component = (*dst_begin)->Get_Component(L"Transform_com");
			CComponent* pOtherMonster_Component = (*src_begin)->Get_Component(L"Transform_com");

			if (nullptr == pTargetMonster_Component || nullptr == pOtherMonster_Component)
				continue;

			const _vec3* pTargetPos = dynamic_cast<CTransform*>(pTargetMonster_Component)->Get_Infomation(CTransform::INFO_POSITION);
			const _vec3* pOtherPos  = dynamic_cast<CTransform*>(pOtherMonster_Component)->Get_Infomation(CTransform::INFO_POSITION);

			if (*pOtherPos == *pTargetPos)
				continue;

			_vec3 TargetPos = *pTargetPos;
			_vec3 OtherPos  = *pOtherPos;

			float fWidth = ((*dst_begin)->Get_Info().fCollisionSize / 2.f) + ((*src_begin)->Get_Info().fCollisionSize / 2.f);
			float fHeight = ((*dst_begin)->Get_Info().fCollisionSize / 2.f) + ((*src_begin)->Get_Info().fCollisionSize / 2.f);

			float fDistX = abs(OtherPos.x - TargetPos.x);
			float fDistZ = abs(OtherPos.z - TargetPos.z);

			if ((fWidth > fDistX) && (fHeight > fDistZ))
			{
				//충돌됫을때
				float fMoveX = fWidth - fDistX;
				float fMoveZ = fHeight - fDistZ;

				if (fMoveX > fMoveZ)
				{
					float fX = TargetPos.x;
					float fZ = TargetPos.z;

					if (OtherPos.z > fZ)
						fMoveZ *= -1;

					_vec3 vTargetPos = _vec3(fX, TargetPos.y, fZ + fMoveZ);
					dynamic_cast<CTransform*>(pTargetMonster_Component)->Set_Infomation(CTransform::INFO_POSITION, &vTargetPos);
				}
				else
				{
					float fX = TargetPos.x;
					float fZ = TargetPos.z;

					if (OtherPos.x > fX)
						fMoveX *= -1;

					_vec3 vTargetPos = _vec3(fX + fMoveX, TargetPos.y, fZ);
					dynamic_cast<CTransform*>(pTargetMonster_Component)->Set_Infomation(CTransform::INFO_POSITION, &vTargetPos);
				}
			}
			else
			{
				//충돌 안됬을때.

			}

		}

	}
}

void CCollisionMgr::Collision_WithWall_Bullet_Circle(list<CGameObject*>* pDstList, list<CGameObject*>* pSrcList)
{
	list<CGameObject*>::iterator dst_begin = pDstList->begin();
	list<CGameObject*>::iterator dst_end = pDstList->end();

	for (; dst_begin != dst_end; ++dst_begin)
	{
		if (true == (*dst_begin)->Get_Info().bDie)
			continue;


		list<CGameObject*>::iterator src_begin = pSrcList->begin();
		list<CGameObject*>::iterator src_end = pSrcList->end();

		for (; src_begin != src_end; ++src_begin)
		{
			if (true == (*src_begin)->Get_Info().bDie)
				continue;

			CComponent* pWall_Component = (*dst_begin)->Get_Component(L"Transform_com");
			CComponent* pBullet_Component = (*src_begin)->Get_Component(L"Transform_com");

			const _vec3* pWallPos = dynamic_cast<CTransform*>(pWall_Component)->Get_Infomation(CTransform::INFO_POSITION);
			const _vec3* pBulletPos = dynamic_cast<CTransform*>(pBullet_Component)->Get_Infomation(CTransform::INFO_POSITION);
			_vec3 vWallPos = *pWallPos;
			_vec3 vBulletPos = *pBulletPos;


			float fSum = (*dst_begin)->Get_Info().fCollisionSize / 2.f + (*src_begin)->Get_Info().fCollisionSize / 2.f;
			float fX = vWallPos.x - vBulletPos.x;
			float fZ = vWallPos.z - vBulletPos.z;
			float fHypo = sqrtf(fX * fX + fZ * fZ);

			if (fSum >= fHypo)
			{
				//충돌

				//아이템 사라짐
				(*src_begin)->Set_iHp(-1);
			}
			else if (fSum < fHypo)
			{
				//충돌X
			}
		}
	}
}

