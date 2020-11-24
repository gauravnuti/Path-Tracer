#ifndef _RENDERENGINE_H_
#define _RENDERENGINE_H_

#include "world.h"
#include "camera.h"

class RenderEngine
{
private:
	World *world;
	Camera *camera;
	int iteration = 0;
	const Color trace(const int i, const int j);

public:
	RenderEngine(World *_world, Camera *_camera):
		world(_world), camera(_camera) {}
	bool renderLoop();
	int getIteration(){return iteration;}
	void updateIteration(){iteration = iteration+1;}
};
#endif
