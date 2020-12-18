#ifndef _SETTINGS_H_
#define _SETTINGS_H_

// ray.h 
// Types of object hit
#define NON_LIGHT 0 
#define LIGHT_SOURCE 1

// material.cpp 
#define EPSILON 0.001f // epsilon for numerical precision of reflected ray.
#define PI 3.14159265
#define NUM_SAMPLES 1 // Samples at each intersection point
#define SPECULAR_POWER 2 // Specularity constant to determine the cone of reflection's apex angle
const bool flag_importance_sampling = true; // Turn on/off cosine weighted importance sampling

// world.cpp
#define max_depth 3 // max recursion depth.


#endif