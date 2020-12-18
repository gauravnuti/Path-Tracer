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

	// float cur_t = FLT_MAX;

	for(int i=0; i<objectList.size(); i++)
		objectList[i]->intersect(ray);

	// if(ray.didHit()) // Not a light source
	// 	ray.setObjectType(false); 
	// 	cur_t = ray.getParameter();

	// for(int i=0; i<lightSourceList.size(); i++){
	// 	if (lightSourceList[i]->getSolidity() == true){
	// 		lightSourceList[i]->intersect(ray);
	// 	}
	// }

	// if (ray.getParameter() < cur_t){
	// 	ray.setObjectType(true); 
	// }

	return ray.getParameter();
}

Color World::shade_ray(Ray& ray, int flag, const float cur_throughput)
{
	// firstIntersection(ray);

	// if (ray.didHit() && ray.getLevel() <= max_depth )
	// 	return ray.intersected()->shade(ray);
	// return background;


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
			return (ray.intersected())->shade(ray,cur_throughput);
		}
	}

	// return hit light source1
	if (light_hit && flag == 0)
		return intensity;
	if(light_hit)
		return Color(0);

	if (ray.getLevel()>0)
		return Color(0);

	return background;
}

Color World::light_ray(const Ray& ray, float &solid_angle)
{
	float lights = (float) lightSourceList.size();
	int ind = rand()%lightSourceList.size();
	// printf("%d\n", ind);
	Vector3D l = lightSourceList[ind]->getPosition() - ray.getPosition();
	float dist = l.length();
	l.normalize();
	// float cos_o = (float) lightSourceList[ind]->getcoso(-l);
	float cos_o = dotProduct( - ray.getDirection(), ray.getNormal());
	cos_o = cos_o * (float) lightSourceList[ind]->getcoso(-l);
	float cos_i = (float) dotProduct(l,ray.getNormal());
	Color total_intensity = Color(0);
	if(cos_o < 0 && cos_i < 0){
		return total_intensity;
	}
	int curLevel = ray.getLevel();
	Ray shadow_ray = Ray(ray.getPosition(),l,curLevel+1);
	float t = firstIntersection(shadow_ray);
	lightSourceList[ind]->intersect(shadow_ray);
	if(t == ray.getParameter()){
		return total_intensity;
	}
	else{
		total_intensity = lightSourceList[ind]->getIntensity();
		// printf("%f\n", cos_i * cos_o * (lights/(dist * dist)));
		float intense = 1.0;
		solid_angle = cos_o * (1/(dist * dist));
		return (total_intensity * intense * lights * cos_i);
	}
}