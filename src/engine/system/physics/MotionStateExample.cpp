#include "MotionStateExample.h"

ds::MotionStateExample::MotionStateExample(const btTransform & initialPosition)
{
	m_initialPosition = initialPosition;
}

void ds::MotionStateExample::getWorldTransform(btTransform & worldTrans) const
{
	worldTrans = m_initialPosition;
}

void ds::MotionStateExample::setWorldTransform(const btTransform & worldTrans)
{
	//btQuaternion rot = worldTrans.getRotation();
	//btVector3 pos = worldTrans.getOrigin();
	//// send to render.
}

ds::MotionStateExample::~MotionStateExample() {}
