/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2007 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
///btDbvtBroadphase implementation by Nathanael Presson

#ifndef BT_DBVT_BROADPHASE_H
#define BT_DBVT_BROADPHASE_H

#include "btDbvt.h"
#include "LinearMath/btPoint3.h"
#include "LinearMath/btVector3.h"
#include "BulletCollision/BroadphaseCollision/btOverlappingPairCache.h"
#include "BulletCollision/BroadphaseCollision/btBroadphaseInterface.h"
#include "BulletCollision/BroadphaseCollision/btBroadphaseProxy.h"
#include "BulletCollision/BroadphaseCollision/btOverlappingPairCallback.h"
#include "BulletCollision/BroadphaseCollision/btOverlappingPairCallback.h"

//
// Compile time config
//

#define	DBVT_BP_PROFILE			1
#define DBVT_BP_DISCRETPAIRS	0
#define DBVT_BP_MARGIN			(btScalar)0.05

#if DBVT_BP_PROFILE
	#define	DBVT_BP_PROFILING_RATE	50
	#include "LinearMath/btQuickprof.h"
#endif

//
// btDbvtProxy
//
struct btDbvtProxy : btBroadphaseProxy
{
/* Fields		*/ 
btDbvtAabbMm		aabb;
btDbvt::Node*		leaf;
btDbvtProxy*		links[2];
int					stage;
/* ctor			*/ 
btDbvtProxy(void* userPtr,short int collisionFilterGroup, short int collisionFilterMask) :
	btBroadphaseProxy(userPtr,collisionFilterGroup,collisionFilterMask)
	{
	links[0]=links[1]=0;
	}
};

//
// btDbvtBroadphase
//
struct	btDbvtBroadphase : btBroadphaseInterface
{
/* Config		*/ 
enum	{
		DYNAMIC_SET			=	0,	/* Dynamic set index	*/ 
		FIXED_SET			=	1,	/* Fixed set index		*/ 
		STAGECOUNT			=	2,	/* Number of stages		*/ 
		PREDICTED_FRAMES	=	2,	/* Frames prediction	*/ 		
		};
/* Fields		*/ 
btDbvt					m_sets[2];					// Dbvt sets
btDbvtProxy*			m_stageRoots[STAGECOUNT+1];	// Stages list
int						m_stageCurrent;				// Current stage
btOverlappingPairCache*	m_paircache;				// Pair cache
btDbvtProxy*			m_fcursor;					// Current fixed cursor
btDbvtProxy*			m_dcursor;					// Current dynamic cursor
int						m_fupdates;					// % of fixed updates per frame
int						m_dupdates;					// % of dynamic updates per frame
int						m_pid;						// Parse id
int						m_gid;						// Gen id
#if DBVT_BP_PROFILE
btClock					m_clock;
struct	{
		unsigned long		m_total;
		unsigned long		m_ddcollide;
		unsigned long		m_fdcollide;
		unsigned long		m_cleanup;
		}				m_profiling;
#endif
/* Methods		*/ 
btDbvtBroadphase();
~btDbvtBroadphase();
void							collide(btDispatcher* dispatcher);
/* btBroadphaseInterface Implementation	*/ 
btBroadphaseProxy*				createProxy(const btVector3& aabbMin,const btVector3& aabbMax,int shapeType,void* userPtr,short int collisionFilterGroup,short int collisionFilterMask,btDispatcher* dispatcher,void* multiSapProxy);
void							destroyProxy(btBroadphaseProxy* proxy,btDispatcher* dispatcher);
void							setAabb(btBroadphaseProxy* proxy,const btVector3& aabbMin,const btVector3& aabbMax,btDispatcher* dispatcher);
void							calculateOverlappingPairs(btDispatcher* dispatcher);
btOverlappingPairCache*			getOverlappingPairCache();
const btOverlappingPairCache*	getOverlappingPairCache() const;
void							getBroadphaseAabb(btVector3& aabbMin,btVector3& aabbMax) const;
void							printStats();
};

#endif

