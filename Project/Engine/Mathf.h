//Protection
#pragma once

//Datatype will always be open gl
#include <GL\glew.h>

//Math functions
#include <math.h>
#include <algorithm>

//For perlin noise
#include <noise\noise.h>

#include <time.h>
#include <random>
#include <iterator>
#include <numeric>

//Supports some glm stuff
#include <glm\glm.hpp>

//General mathematic functions
//This will recreate the important functions from:
//https://docs.unity3d.com/ScriptReference/Mathf.html
//But in C++ with opengl values

static class cMathf
{
public:
	/*
	Missing functions (This that should be added but I couldnt figure out):
	- Inverse learping - find the iterpolant between a and b based on t
	- Negative infinity - C++ does not support (I think)
	*/

	/* ----- Constant values ----- */
	const GLfloat PI = 3.1415926f; //The circumference to the perimeter of a circle
	const GLfloat TAU = 6.2831852f; //Double 'pi'
	const GLfloat TRUNCATION_LIMITS = 0.005f; //Temp
	const GLfloat INFINITE = std::numeric_limits<float>::infinity(); //Like: HUGE_VALF
	const GLfloat DEG_TO_RAD = (TAU) / 360; //For converting degrees to radians
	const GLfloat RAD_TO_DEG = 360 / (TAU); //For converting radians to degrees

	/* ----- Wrapper functions ----- */

	//General
	inline GLfloat sqrt(GLfloat f) { return sqrtf(f); }
	inline GLfloat pow(GLfloat f, GLfloat power) { return powf(f, power); }
	inline GLfloat root(GLfloat f, GLfloat n) { return pow(f, 1.0f / n); } //Convert to fractional indicies

	//Trig
	inline GLfloat cos(GLfloat foo) { return cosf(foo); }
	inline GLfloat sin(GLfloat foo) { return sinf(foo); }
	inline GLfloat abs(GLfloat foo) { return fabs(foo); }
	inline GLfloat acos(GLfloat foo) { return acosf(foo); }
	inline GLfloat asin(GLfloat foo) { return asinf(foo); }
	inline GLfloat atan(GLfloat foo) { return atanf(foo); }
	inline GLfloat atan2(GLfloat x, GLfloat y) { return atan2f(x, y); }

	//Rounding
	inline GLfloat ceil(GLfloat f) { return ceilf(f); }
	inline GLfloat floor(GLfloat f) { return floorf(f); }
	inline GLint ceilInt(GLfloat f) { return (GLint)ceilf(f); }
	inline GLint floorInt(GLfloat f) { return (GLint)floorf(f); }
	inline GLfloat max(GLfloat a, GLfloat b) { return std::max(a, b); }
	inline GLfloat min(GLfloat a, GLfloat b) { return std::min(a, b); }
	inline GLfloat round(GLfloat f) { return roundf(f); }
	inline GLint roundInt(GLfloat f) { return (GLint)roundf(f); }

	/* ----- Useful functions ----- */
	//Converts a quaternion to an Euler Angle
	glm::vec3 quatToEuler(glm::quat q)
	{
		//Create glm::vec3 to hold the converted glm::quat rotation
		glm::vec3 eulerAngle;

		//Implement Euler's algorithm for converting quaternions into vectors
		eulerAngle.x = atan2((2) * (q.y * q.z + q.w * q.x), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);
		eulerAngle.y = asin(clamp((-2) * (q.x * q.z - q.w * q.y), (-1), (1)));
		eulerAngle.z = atan2((2) * (q.x * q.y + q.w * q.z), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z);
				
		//Multply the angle as a glm::vec3 by (360 / 2 * PI) to convert radians back into degrees form
		eulerAngle *= RAD_TO_DEG;
		
		//And then finally return the same rotation as a glm::vec3
		return eulerAngle;
	}

	//Since the C++ '%' doesnt support floats
	GLfloat mod(GLfloat num, GLfloat div) { return div * ((num / div) - Mathf.floor(num / div)); }

	//Returns a value inbetween min and max
	inline GLfloat clamp(GLfloat value, GLfloat minimum, GLfloat maximum)
	{
		//Combine the maximum and mimumm functions to achive this
		return std::max(minimum, std::min(value, maximum));
	}

	//Clamps between 0 and 1
	inline GLfloat clamp01(GLfloat value) { return Mathf.clamp(value, 0.0f, 1.0f); }

	//Smoothdamp, much like lerp interpolates between values
	//But smoothing (much like a broad cubic function)
	//Apply the equation:
	//fn(x) = 3x^2 - 2x^3
	//This is in the form of a wuatratic equation
	//Source: https://en.wikipedia.org/wiki/Smoothstep
	// http://http.developer.nvidia.com/Cg/smoothstep.html
	GLfloat smoothstep(GLfloat left, GLfloat right, GLfloat x)
	{
		//Clamp the value
		x = Mathf.clamp01((x - left) / (right - left));

		//Evaluate quadratic
		return (GLfloat)(x * x * (3.0 - 2.0 * x));
	}
	GLfloat smoothstep(GLfloat x) { return Mathf.smoothstep(0.0f, 1.0f, x); }

	//This applies a combination of lerping and smooth stepping
	//By haveint a smooth interpolation to a result
	//This is a 100% copy of unitys Smooth damp, found here:
	//https://docs.unity3d.com/ScriptReference/Mathf.SmoothDamp.html
	GLfloat SmoothDamp(GLfloat current, GLfloat target, GLfloat vel, float smoothTime, float maxSpeed, float deltaTime)
	{
		smoothTime = Mathf.max(0.0001f, smoothTime);
		float num = 2.0f / smoothTime;
		float num2 = num * deltaTime;
		float num3 = 1.0f / (1.0f + num2 + 0.48f * num2 * num2 + 0.235f * num2 * num2 * num2);
		float num4 = current - target;
		float num5 = target;
		float num6 = maxSpeed * smoothTime;
		num4 = Mathf.clamp(num4, -num6, num6);
		target = current - num4;
		float num7 = (vel + num * num4) * deltaTime;
		vel = (vel - num * num7) * num3;
		float num8 = target + (num4 + num7) * num3;
		if (num5 - current > 0.0f == num8 > num5)
		{
			num8 = num5;
			vel = (num8 - num5) / deltaTime;
		}

		return num8;
	}

	//Similar but with the equation:
	//6x^5 - 15x^4 + 10x^3
	GLfloat smootherstep(GLfloat left, GLfloat right, GLfloat x)
	{
		//Scale and clamp
		GLfloat nx = Mathf.clamp01((x - left) / (right - left));

		//Evaluate
		return nx * nx * nx * (nx * (nx * 6 - 15) + 10);
	}
	GLfloat smootherstep(GLfloat x) { return Mathf.smootherstep(0.0f, 1.0f, x); }

	//A simplified version of smotherstep
	//This is used for perlin noise interpolation between seed vector
	GLfloat interpolateNoiseCurve(GLfloat t) { return t * t * t * (t * (t * 6 - 15) + 10); }

	//For finding the gradient of two point in the perlin noise cube
	GLfloat noiseGradient(GLint hash, GLfloat x, GLfloat y, GLfloat z)
	{
		//From: https://github.com/sol-prog/Perlin_Noise
		int h = hash & 15;

		double u = h < 8 ? x : y,
			v = h < 4 ? y : h == 12 || h == 14 ? x : z;
		return (GLfloat)(((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v));
	}

	//Find the sign (posative of negative) of a number
	//Note: 0 is considered a posative number
	inline GLfloat sign(GLfloat foo) { return (foo < 0.0f) ? -1.0f : 1.0f; }

	//Linearly interpolate between two floats by t
	inline GLfloat lerpUnclamped(GLfloat a, GLfloat b, GLfloat t)
	{
		//Just use equation from:
		//(1 - t) * v0 + t * v1
		//https://devblogs.nvidia.com/parallelforall/lerp-faster-cuda/

		return (1 - t) * a + t * b;
	}
	inline GLfloat lerp(GLfloat a, GLfloat b, GLfloat t) { return Mathf.lerpUnclamped(a, b, Mathf.clamp01(t)); }
	inline GLfloat noiseLerp(GLfloat t, GLfloat a, GLfloat b) { return a + t * (b - a); }

	//Like lerping but the value will never exceed a delta
	GLfloat moveTowards(GLfloat current, GLfloat target, GLfloat delta)
	{
		//Make sure distance is less than delta
		if (Mathf.abs(target - current) <= delta) return target;

		//Otherwise apply a lerp
		return current + Mathf.sign(target - current) * delta;
	}

	//Ping pong will loop a value between 0 and the upper limit
	GLfloat bounce(GLfloat value, GLfloat min, GLfloat max)
	{
		float range = max - min;
		float state = mod(value - min, 2 * range);

		if (state > range)
			state = (2 * range) - state;

		return state + min;
	}
	//Overload
	GLfloat bounce(GLfloat value, GLfloat max) { return Mathf.bounce(value, 0.0f, max); }
	GLfloat bounce(GLfloat value)			   { return Mathf.bounce(value, 0.0f, 1.0f); }

	//Logarithmics...
	inline GLfloat log(GLfloat f, GLfloat base)
	{
		//Consider C++ only priveds a base 2 and 10 log
		//The other base must be calculated
		return logf(f) / logf(base);
	}
	inline GLfloat log(GLfloat f) { return logf(f); }
	inline GLfloat log2(GLfloat f) { return Mathf.log(f, 2); }
	inline GLfloat log10(GLfloat f) { return Mathf.log(f, 10); }

	//This will find the percentage trhough a lerp based on paramters
	//Inverse lerp - not done

	//Find the the closest difference between two angles
	inline GLfloat deltaAngle(GLfloat current, GLfloat target)
	{
		//FInd difference
		float diff = target - current;

		//Adjust signs
		while (diff < -180) diff += 360;
		while (diff > 180) diff -= 360;

		//Done
		return diff;
	}

	//Find the value as a power of two
	inline GLint closestBinaryPower(GLfloat value)
	{
		//Needs	 the find the 2 root of the value then round that to an int
		return Mathf.roundInt(Mathf.pow(Mathf.round(Mathf.sqrt(value)), 2.0f));
	}
	inline bool isBinaryPower(GLfloat value) { return closestBinaryPower(value) == value; }

	//Find the max where:
	//Arr is a heap array
	//size is the BYTE size
	GLfloat max(GLfloat* arr, int size)
	{
		//Stores the best value
		GLfloat current_highest = 0;

		//Go through and set based on max
		for (unsigned int i = 0; i < size / sizeof(GLfloat); i++) { if (arr[i] > current_highest || i == 0) current_highest = arr[i]; }

		//Done
		return current_highest;
	}

	//Find the min where:
	//Arr is a heap array
	//size is the BYTE size
	GLfloat min(GLfloat* arr, int size)
	{
		//Stores the best value
		GLfloat current_lowest = 0;

		//Go through and set based on max
		for (unsigned int i = 0; i < size / sizeof(GLfloat); i++) { if (arr[i] < current_lowest || i == 0) current_lowest = arr[i]; }

		//Done
		return current_lowest;
	}

	//Because of slight round errors in floats, this should be used when comparing
	bool approximatly(GLfloat a, GLfloat b) { return Mathf.abs(a - b) < Mathf.TRUNCATION_LIMITS; }

	/* ----- Perlin noise ----- */

	//Create an instance of a perlin noise object
	noise::module::Perlin modual;

	//Holds all the properties of the noise object
	noise::NoiseQuality quality = noise::NoiseQuality::QUALITY_FAST; //Self explaitory
	double persistance = modual.GetPersistence(); //How each octave affects the pervious
	int octave_count = modual.GetOctaveCount(); //Number of consective maps to generate
	double lacunarity = modual.GetLacunarity(); //The increase in scale for each consective map
	double frequency = modual.GetFrequency(); //The change in power each map has
	int seed = modual.GetSeed(); //Random number gen seed

	//Set the values
	void createMap()
	{
		//Manually set
		modual.SetOctaveCount(octave_count);
		modual.SetPersistence(persistance);
		modual.SetLacunarity(lacunarity);
		modual.SetNoiseQuality(quality);
		modual.SetFrequency(frequency);
		modual.SetSeed(seed);
	}

	//Just a wrapper
	GLfloat perlinNoise(GLfloat x, GLfloat y, GLfloat z = 0.0f) { return (GLfloat)modual.GetValue(x, y, z); }

	//Other wrappers

	//Old (depreciated)
	/*
	std::vector<GLint> permutations; //Stores all permutations, considering perlin noise is an array of random seeds

	void seedPerlinNoise(GLuint seed)
	{
	// Duplicate the permutation vector
	permutations.insert(permutations.end(), permutations.begin(), permutations.end());

	//Generate an array of possible versions of the perlin noise
	permutations.resize(256);

	//This will sequentiall fill the vector with increasing values
	//Alternitives are:
	//std::generate
	//for (int i = 0; i < permutations.size(); i++) permutations[i] = i;
	std::iota(permutations.begin(), permutations.end(), 0);

	// Initialize a random engine with seed
	//The default random engine is: mt19937
	std::default_random_engine engine(seed);

	//This means that each member of the array is ordered with an equal spread of probability
	//So and number choosen will seem random
	std::shuffle(permutations.begin(), permutations.end(), engine);

	//Duplicate the vector
	permutations.insert(permutations.end(), permutations.begin(), permutations.end());
	}

	GLfloat perlinNoise(GLfloat x, GLfloat y, GLfloat z = 0.0f)
	{
	//Find the cube (of size 1, 1, 1) that contains the point mentionsed in the parameters
	glm::vec3 unit_cube;

	//This will clamp the value to an edge of the cube because:
	//0000000 & x = 0
	//1111111 & x = x
	unit_cube.x = (GLint) floor(x) & 255;
	unit_cube.y = (GLint) floor(y) & 255;
	unit_cube.z = (GLint) floor(z) & 255;

	//Make relative to that point in the square
	unit_cube.x -= floor(x);
	unit_cube.y -= floor(y);
	unit_cube.z -= floor(z);

	//Interpolate the vector seed values
	glm::vec3 interpolation_point;

	interpolation_point.x = Mathf.interpolateNoiseCurve(unit_cube.x);
	interpolation_point.y = Mathf.interpolateNoiseCurve(unit_cube.y);
	interpolation_point.z = Mathf.interpolateNoiseCurve(unit_cube.z);

	//Find the coordinates of each corner of the cube
	//Remembering the perlin notation of 00, 01, 10, 11...
	GLint A = permutations[(GLint)unit_cube.x] + (GLint)unit_cube.y;
	GLint AA = permutations[A] + (GLint)unit_cube.z;
	GLint AB = permutations[A + 1] + (GLint)unit_cube.z;
	GLint B = permutations[(GLint)unit_cube.x + 1] + (GLint)unit_cube.y;
	GLint BA = permutations[B] + (GLint)unit_cube.z;
	GLint BB = permutations[B + 1] + (GLint)unit_cube.z;

	//Using the formula found:
	//https://en.wikipedia.org/wiki/Perlin_noise
	//Each normalized corner must be added
	double result = Mathf.noiseLerp(interpolation_point.z, Mathf.noiseLerp(interpolation_point.y,
	Mathf.noiseLerp(interpolation_point.x, Mathf.noiseGradient(permutations[AA], x, y, z), Mathf.noiseGradient(permutations[BA], x - 1, y, z)),
	Mathf.noiseLerp(interpolation_point.x, Mathf.noiseGradient(permutations[AB], x, y - 1, z), Mathf.noiseGradient(permutations[BB], x - 1, y - 1, z))),
	Mathf.noiseLerp(interpolation_point.y, Mathf.noiseLerp(interpolation_point.x, Mathf.noiseGradient(permutations[AA + 1], x, y, z - 1),
	Mathf.noiseGradient(permutations[BA + 1], x - 1, y, z - 1)),
	Mathf.noiseLerp(interpolation_point.x, Mathf.noiseGradient(permutations[AB + 1], x, y - 1, z - 1),
	Mathf.noiseGradient(permutations[BB + 1], x - 1, y - 1, z - 1))));

	//Done
	return (result + 1.0) / 2.0;
	}
	*/
} Mathf;

//Some simple functions for random number generation
//This is so perlin noise is easier
static class cRandom
{
public:
	//Seed the random value
	void seed(GLuint s = time(NULL)) { srand(s); }

	//Random number between min and max
	GLfloat range(GLfloat min, GLfloat max) { return min + (GLfloat)(rand()) / ((GLfloat)(RAND_MAX / (max - min))); }

	//Generates a random number between 0 and 1
	GLfloat value() { return Random.range(0.0f, 1.0f); }

	//Generate a random integer 
	GLint randInt(GLint min, GLint max) { return rand() % (max - min) + min; }
	GLint randBin() { return Random.randInt(0, 2); }
} Random;
