#include "renderengine.h"
#include <iostream>
#include <time.h>


const Color RenderEngine::trace(const int i, const int j)
{

	Color pixelSample = Color(0,0,0);

	for(int k = 0; k<SAMPLES_PER_PIXEL; k++){
		Vector3D ray_dir = camera->get_ray_direction(i, j);
		Ray ray(camera->get_position(), ray_dir);
		Color path_illumination = world->shade_ray(ray);
		pixelSample = pixelSample + path_illumination*(1.0f/SAMPLES_PER_PIXEL);
	}

	return pixelSample;
}	

bool RenderEngine::renderLoop()
{

	if (last_start == -1){
		last_start = time (NULL);
		std::cout<< last_start<< std::endl;
	}

	static int i = 0;
	for(int j = 0; j<camera->getHeight(); j++)
	{
		Color color = trace(i, j);
		color.clamp();
		// camera->drawPixel(i, j, color);
		camera->updatePixel(i, j, getIteration(), color);
	}

	if(++i == camera->getWidth())
	{
		i = 0;
		updateIteration();
		std::cout<< "Iteration Number = "<< getIteration() + 1<< std::endl;
		std::cout<< "Iteration Time = "<< time (NULL) - last_start<< std::endl;
		last_start = time(NULL);

		return true;
	}
	return false;
}
