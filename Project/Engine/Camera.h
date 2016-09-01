//Needs glm for matricies and maths
#include <glm\glm.hpp>
#include <glm\common.hpp>
#include <glm\matrix.hpp>
#include <glm\gtc\matrix_transform.hpp>

//One can create an object of the camera
class Camera
{
public:
	//Later this will be housed in a transform object
	glm::vec3 position;
	glm::vec3 look_at;

	//Is it an orthographic or perspective camera?
	enum cameraType { Perspective, Orthographic };
	cameraType type;

	//Properties of a camera
	float fov;
	float near_clipping;
	float far_clipping;

	//Takes params and set members
	Camera() : 
		position(glm::vec3(0, 0, 0)), 
		look_at(glm::vec3(0, 0, 0)), 
		type(cameraType::Perspective),
		near_clipping(0.1f), 
		far_clipping(100.0f),
		fov(60) {}

	void Init(glm::vec3 pos, glm::vec3 look, cameraType t = cameraType::Perspective, float f = 60, float n = 0.1f, float c = 100.0f)
	{
		near_clipping = n;
		far_clipping = c; 
		look_at = look;
		position = pos;
		type = t;
		fov = f;
	}

	//Will find the model view projection matrix for this camera
	glm::mat4 getMVP() 
	{
		//Find projection matrix
		if (type == cameraType::Perspective) _projection_mat = glm::perspective(glm::radians(fov), (float)(1280 / 720), near_clipping, far_clipping);
		if (type == cameraType::Orthographic) _projection_mat = glm::ortho(-fov, fov, -fov, fov, near_clipping, far_clipping);

		//Create the camera matrix
		_view_mat = glm::lookAt(position, look_at, glm::vec3(0, 1, 1));

		//Create identity matrix
		_model_mat = glm::mat4(1.0f);

		//Done
		_current_MVP = _projection_mat * _view_mat * _model_mat;
		return _current_MVP;
	};

//Member vars
private:
	glm::mat4 _current_MVP;

	glm::mat4 _projection_mat;
	glm::mat4 _view_mat;
	glm::mat4 _model_mat; //Temp
};