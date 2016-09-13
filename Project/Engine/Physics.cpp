#include "Physics.h"

namespace Physics
{
  //Sets Bullet up
  void Init();
  void Init()
  {
    //Defines Bullet's algorith for objects that should not collide
    btBroadPhaseInterface *broadPhase = new btDbvtBroadphase();
    //Default Bullet collision algorithm
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    //Incorporates gravity, collisions and object interactions
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    //Create dynamic world
    btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
  }
}
