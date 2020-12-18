#ifndef _SETTINGS_H_
#define _SETTINGS_H_

// ray.h 
// Types of object hit
#define NON_LIGHT 0 
#define LIGHT_SOURCE 1

// material.cpp 
#define EPSILON 0.001f // epsilon for numerical precision of reflected ray.
#define PI 3.14159265
#define SPECULAR_POWER 2 // Specularity constant to determine the cone of reflection's apex angle
// #define NUM_SAMPLES 1 // Samples at each intersection point
#define RUSSIAN_ROULETTE_START 6// Start depth of russian roullete
const bool flag_importance_sampling = true; // Turn on/off cosine weighted importance sampling
// To turn on/off the contribution of lights in the indirect illumination component
const int sampling_model_flag = 0; // 1 to turn off ; 0 -to turn on.
// world.cpp
#define max_depth 5 // max recursion depth.

// renderengine.cpp
#define	SAMPLES_PER_PIXEL 2 // Number of paths per pixel

#endif

/*
Original Exp Params-
#define NUM_SAMPLES 1 // Samples at each intersection point
const bool flag_importance_sampling = false; // Turn on/off cosine weighted importance sampling
#define max_depth 3 // max recursion depth.
#define	SAMPLES_PER_PIXEL 2 // Number of paths per pixel
*/
