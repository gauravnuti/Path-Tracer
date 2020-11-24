#include "world.h"
#include <iostream>


using namespace std;
// #define max_depth 0

Color World::lightIntersection(Ray& ray){

	float max_t = FLT_MAX;
	Color intensity = Color(0);

	for(int i=0; i<lightSourceList.size(); i++){
		if (lightSourceList[i]->getSolidity() == true){
			lightSourceList[i]->intersect(ray);
			if (ray.didHit()){
				if (ray.getParameter()<max_t){
					max_t = ray.getParameter();
					intensity = lightSourceList[i]->getIntensity() ;
				}
			}
		}
	}

	return intensity;
}

float World::firstIntersection(Ray& ray)
{
	for(int i=0; i<objectList.size(); i++)
		objectList[i]->intersect(ray);

	return ray.getParameter();
}

Color World::shade_ray(Ray& ray)
{

	// Check if we light source is hit
	Color intensity = lightIntersection(ray);
	float light_t = FLT_MAX;
	float light_hit = ray.didHit();
	if (light_hit)
		light_t = ray.getParameter();

	// check if a closer object is hit
	float obj_t = firstIntersection(ray);
	if(ray.didHit() && ray.getLevel() <= max_depth){
		if (obj_t < light_t){
			// if (ray.getLevel()>1){
			// 	std::cout <<  ray.getLevel() << std::endl;
			// }
			return (ray.intersected())->shade(ray);
		}
	}

	// return hit light source
	if (light_hit)
		return intensity;

	if (ray.getLevel()>0)
		return Color(0);

	return background;
}
