#pragma once
#include <string>

class ModelInfo
{
public: 
	ModelInfo(); 

	// Setters 
	void SetName(std::string s); 
	void SetModelPath(std::string s); 
	void SetTexturePath(std::string s); 
	void SetPosition(float x_, float y_, float z_);
	void SetOrientation(float x_, float y_, float z_); 
	void SetScale(float x_, float y_, float z_); 

	// Getters 
	std::string GetName() { return name; }
	std::string GetModelPath() { return model_path; }
	std::string GetTexturePath() { return tex_diffuse_path; }

	float GetXPos() { return x_pos; }
	float GetYPos() { return y_pos; }
	float GetZPos() { return z_pos; }

	float GetXRot() { return x_rot; }
	float GetYRot() { return y_rot; }
	float GetZRot() { return z_rot; }

	float GetXScale() { return x_scale; }
	float GetYScale() { return y_scale; }
	float GetZScale() { return z_scale; }

private: 
	// General 
	std::string name; 
	std::string model_path; 
	std::string tex_diffuse_path; 

	// Position 
	float x_pos; 
	float y_pos; 
	float z_pos; 

	// Orientation 
	float x_rot; 
	float y_rot; 
	float z_rot; 

	// Scale 
	float x_scale; 
	float y_scale; 
	float z_scale; 
};

