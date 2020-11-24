//matrial.h
#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "color.h"
#include "ray.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>

class World;

class Material
{
private:
	World *world;
public:
	//Data

	Color color;
	double albedo;//BRDF albedo
	double specularity;// [0,1] - 0 -Diffused / 1 - Specular
	Color emittance;

	Material(World *w, double _albeldo = 0.95, double _spec = 0, double _emittance = 0):
		world(w), color(0),
		albedo(_albeldo), specularity(_spec), emittance(Color(_emittance)) {}

	Ray reflect(const Ray& incident) const;
	Color refract(const Ray& incident, const int sign) const;
	Color shade(const Ray& incident, const bool isSolid) const;


};
#endif
