#include "world.h"
#include "material.h"
#include "vector3D.h"
#include <iostream>
#include <thread>
#include "settings.h"


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


Vector3D diffuseReflection(float specularity, float specular_flag, glm::mat3 &base_change_mat){
	// Declaring vector to store random reflected ray
	glm::vec3 randTransRay(0.0f,0.0f,0.0f);
	Vector3D randDir;

	// theta is angle about normal
	float theta = (((float) drand48())*PI - PI/2.0f)*(specular_flag*(1-pow(specularity,SPECULAR_POWER)) + (1-specular_flag));
	// phi is angle with the u avis on projecting onto the uv plane
	float phi = ((float) drand48())*PI; 

	// Creating random reflection ray
	glm::vec3 randCartRay(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
	randTransRay = base_change_mat*randCartRay;
	randDir = Vector3D(randTransRay.x,randTransRay.y,randTransRay.z);
	randDir.normalize();

	return randDir;
}

// Cosine weighted importance sampling based diffused reflection sampler
Vector3D cosineWeightedReflection(float specularity, float specular_flag, glm::mat3 &base_change_mat){
	// Declaring vector to store random reflected ray
	glm::vec3 randTransRay(0.0f,0.0f,0.0f);
	Vector3D randDir;

	// sqrt(r0) - y
	float r0 = drand48(), r1 = drand48();
	float r = sqrt(r0);
	float theta = 2 * PI * r1;
	float x = r * cos( theta );
	float y = r * sin( theta );
	
	glm::vec3 randCartRay(x,y,sqrt(1 - r0));
	randTransRay = base_change_mat*randCartRay;
	randDir = Vector3D(randTransRay.x,randTransRay.y,randTransRay.z);
	randDir.normalize();

	return randDir;
}


Ray Material::reflect(const Ray& incident) const 
{
	Vector3D incidence_dir = incident.getPosition() - incident.getOrigin();
	incidence_dir.normalize();
	Vector3D reflect_origin = incident.getPosition();
	Vector3D reflect_dir = incidence_dir - 2*dotProduct(incidence_dir,incident.getNormal())*incident.getNormal();
	reflect_dir.normalize();
	reflect_origin = reflect_origin + EPSILON*reflect_dir;
	Ray reflected_ray(reflect_origin, reflect_dir);
	reflected_ray.setLevel(incident.getLevel()+1);
	return reflected_ray;
}

Color Material::shade(const Ray& incident, const float cur_throughput, const bool isSolid) const
{
	// Initializing values for donwstream rendering equation
	Color total_intensity = Color(0);
	float cos_i, theta, phi, surface_phenomena;

	// Storing current values of ray
	Vector3D incidence_pos = incident.getPosition();
	int curLevel = incident.getLevel();

	// Change of basis matrix for random sampling around normal
	glm::mat3 base_change_mat;

	// Setting up random experiment to determine specular/diffused behaviour
	surface_phenomena = (float)drand48();
	float specular_flag = 0.0f;

	// Setup for Specular Reflection
	if (surface_phenomena<specularity){
		Ray reflected_ray = reflect(incident);

		// Performing mirror reflection
		if (specularity == 1){
			return albedo*world->shade_ray(reflected_ray);
		}

		// Setting up change of basis matrix into a uvw coordinate system
		// The w basis cooresponds to the reflected ray direction
		base_change_mat = glm::inverse(getBaseChangeMatrix(reflected_ray.getDirection())); 

		// setting the flag to indicate specular reflection
		specular_flag = 1.0f;
	}
	// Setup for Diffused Reflection
	else{

		// Setting up change of basis matrix into a uvw coordinate system
		// The w basis cooresponds to the normal at the point of intersection
		base_change_mat = glm::inverse(getBaseChangeMatrix(incident.getNormal()));
	}

	// Declaring vector to store random reflected ray
	glm::vec3 randTransRay(0.0f,0.0f,0.0f);
	Vector3D randDir;

	////////////////////////////
	// Random Direction Sampling
	
	// int sampling_model_flag;
	float hemi_PDF;
	float dir_PDF;
	float BRDF = albedo/PI;
	float solid_angle = -1;

	// importance sampling of diffused reflection
	if(flag_importance_sampling){
		randDir = cosineWeightedReflection(specularity, specular_flag, base_change_mat);
	}
	// uniform sampling of diffused reflection
	else{
		randDir = diffuseReflection(specularity, specular_flag, base_change_mat);
	}
	

	// Computing cos of angle of random ray with w-axis in uvw space
	if (specular_flag>0)
		cos_i = (float) dotProduct(randDir,reflect(incident).getDirection()); //incident.getNormal()); // should this be reflected_ray dir?
	else
		cos_i = (float) dotProduct(randDir,incident.getNormal());

	// Final Random Reflected Ray
	Ray randRay = Ray(incidence_pos+EPSILON*randDir,randDir,curLevel+1);

	// Adjust PDF if importance sampling enabled
	if(flag_importance_sampling){
		hemi_PDF = cos_i/(PI);
	}
	else{
		hemi_PDF = 1/(2*PI);
	}

	// Direct Light Sampling
	Color dir_illumination = world->light_ray(incident, solid_angle);
	dir_PDF = 1/solid_angle;
	Color out_dir_illumination = (BRDF/dir_PDF) * dir_illumination * color;
	total_intensity = total_intensity + out_dir_illumination;

	// Implementing russian roullette
	float max_throughput = (float)std::max(out_dir_illumination.R(),std::max(out_dir_illumination.G(),out_dir_illumination.B()));
	const float new_throughput = std::min(cur_throughput + std::abs(max_throughput),1.0f);
	float temp_new_throughput = 1.0f;

	float drop = (float) drand48();

	// Start dropping only after a certain depth
	if (incident.getLevel() > RUSSIAN_ROULETTE_START)
		temp_new_throughput = new_throughput;

	if (drop > temp_new_throughput){
		total_intensity = (total_intensity + emittance*color + world->getAmbient()*color)*(1/temp_new_throughput);
		return total_intensity;
	}

	// Hemisphere Sampling
	Color indir_illumination = world->shade_ray(randRay,sampling_model_flag,new_throughput);
	total_intensity =  total_intensity +  (BRDF / hemi_PDF) * indir_illumination * color * cos_i;


	// Multiple Importance Sampling - Balance Heuristics
	// float compbined_PDF = dir_PDF+hemi_PDF;
	// float BRDF_dir = dir_PDF/compbined_PDF;
	// float BRDF_hemi = hemi_PDF/compbined_PDF;

	// total_intensity =  (BRDF_dir/dir_PDF) * dir_illumination * color  +  (BRDF_hemi / hemi_PDF) * indir_illumination * color * cos_i;

	total_intensity = (total_intensity + emittance*color + world->getAmbient()*color)*(1/temp_new_throughput);

	return total_intensity;
}
