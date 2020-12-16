#include "world.h"
#include "material.h"
#include "vector3D.h"
#include <iostream>

#define epsilon 0.001 // epsilon for numerical precision of reflected ray.
#define PI 3.14159265
#define N 4


glm::mat3 getBaseChangeMatrix(Vector3D normal){
    // Rotation using change of basis matrix 
    glm::vec3 w(normal.X(),normal.Y(),normal.Z());
    glm::vec3 up(normal.X()+1.1,normal.Y()-1.5,normal.Z()+2.0);
    glm::vec3 u = glm::normalize(glm::cross(up,w));
    glm::vec3 v = glm::normalize(glm::cross(w,u));


    float change_of_basis[9] = {
        u.x, v.x, w.x, // first column
        u.y, v.y, w.y, // second column
        u.z, v.z, w.z  // third column
        };

    glm::mat3 base_change_mat = glm::make_mat3(change_of_basis);
    return base_change_mat;
}

Ray Material::reflect(const Ray& incident) const 
{
	Vector3D incidence_dir = incident.getPosition() - incident.getOrigin();
	incidence_dir.normalize();
	Vector3D reflect_origin = incident.getPosition();
	Vector3D reflect_dir = incidence_dir - 2*dotProduct(incidence_dir,incident.getNormal())*incident.getNormal();
	reflect_dir.normalize();
	reflect_origin = reflect_origin + epsilon*reflect_dir;
	Ray reflected_ray(reflect_origin, reflect_dir);
	reflected_ray.setLevel(incident.getLevel()+1);
	return reflected_ray;
}

Color Material::shade(const Ray& incident, const bool isSolid) const
{
	Color total_intensity = Color(0);
	float cos_i, theta, phi;

	Vector3D incidence_pos = incident.getPosition();
	int curLevel = incident.getLevel();

	glm::mat3 base_change_mat;

	if (specularity>0){
		Ray reflected_ray = reflect(incident);
		if (specularity == 1){
			return albedo*world->shade_ray(reflected_ray);
		}
		base_change_mat = glm::inverse(getBaseChangeMatrix(reflected_ray.getDirection())); 
	}
	else{
		base_change_mat = glm::inverse(getBaseChangeMatrix(incident.getNormal()));
	}

	glm::vec3 randTransRay(0.0f,0.0f,0.0f);
	Vector3D randDir;

	// Random Direction Sampling
	for(int i=0; i<N; i++){
		// cos_i = -0.1;

		// theta = (((float) drand48())*PI - PI/2.0f)*(1-pow(specularity,5));
		// phi = ((float) drand48())*PI; 

		// glm::vec3 randCartRay(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
		// randTransRay = base_change_mat*randCartRay;
		// randDir = Vector3D(randTransRay.x,randTransRay.y,randTransRay.z);
		// randDir.normalize();
		// if (specularity>0)
		// 	cos_i = (float) dotProduct(randDir,reflect(incident).getDirection()); //incident.getNormal()); // should this be reflected_ray dir?
		// else
		// 	cos_i = (float) dotProduct(randDir,incident.getNormal());
		// Ray randRay = Ray(incidence_pos+epsilon*randDir,randDir,curLevel+1);
		// total_intensity =  total_intensity + 2* PI * (1.0f/(float)N) * (albedo/PI) * world->shade_ray(randRay)*color* cos_i;//sampling hemisphere
		// total_intensity = total_intensity + (albedo/PI) * world->light_ray(incident)*color;//sampling lights

		//sampling hemisphere - importance sampling
		float r0 = drand48(), r1 = drand48();
		float r = sqrt(r0);
		float theta = 2 * PI * r1;
		float x = r * cos( theta );
		float y = r * sin( theta );
		float solid_angle = -1;
		glm::vec3 randCartRay(x,y,sqrt(1 - r0));
		randTransRay = base_change_mat*randCartRay;
		randDir = Vector3D(randTransRay.x,randTransRay.y,randTransRay.z);
		randDir.normalize();
		cos_i = (float) dotProduct(randDir,incident.getNormal());
		Ray randRay = Ray(incidence_pos+epsilon*randDir,randDir,curLevel+1);
		
		Color direct = world->light_ray(incident, solid_angle);
		float pdf_hemi = 1/(2*PI);
		float pdf_direct = 1/solid_angle;

		float ratio = pdf_direct/(pdf_direct + pdf_hemi);

		float balance = ratio * (pdf_direct) + (1 - ratio) * pdf_hemi;

		total_intensity = total_intensity + (1.0f/(float)N) * albedo *  world->shade_ray(randRay, 1) * color;
		total_intensity = total_intensity + (1.0f/(float)N) * cos_i * (albedo/PI) * direct *color * (1/balance);
	}

	total_intensity = total_intensity + emittance*color + world->getAmbient()*color;

	return total_intensity;
}
