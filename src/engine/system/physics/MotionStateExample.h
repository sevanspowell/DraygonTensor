#pragma once
#include <btBulletDynamicsCommon.h>

namespace ds
{
	/* Example of a motion state implementation.
	So, this gets passed to a rigid body constructor. Here you
	can pass initial cords and every tick, do something with current
	position. Prefer this over fetching raw world matrix because
	this is using interpolation.

	TODO: remove on release.
	There is a default that can be used though.
	
	See: http://www.bulletphysics.org/mediawiki-1.5.8/index.php/MotionStates */

	class MotionStateExample : public btMotionState
	{
	public:

		/* Pass in an initial position for the origin and
		also an object of sorts where you can update and send
		to the renderer.*/
		MotionStateExample(const btTransform &initialPosition);

		/* empty for time being.*/
		virtual ~MotionStateExample();

		/* Called when first created, on first tick. This is where
		bullet grabs the initial position from of the object.*/
		virtual void getWorldTransform(btTransform &worldTrans) const;

		/* Every tick, passed the interpolation matrix for you to make
		changes to the passed object from the constructor params.*/
		virtual void setWorldTransform(const btTransform &worldTrans);


	protected:

		/* Holds starting position of the object.*/
		btTransform m_initialPosition;
	};
}