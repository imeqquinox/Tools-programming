#include "ModelInfo.h"

ModelInfo::ModelInfo()
{
	name = "Default"; 
	model_path = "database/data/placeholder.cmo";
	tex_diffuse_path = "database/data/placeholder.dds";

	x_pos = 0; 
	y_pos = 0; 
	z_pos = 0; 

	x_rot = 0; 
	y_rot = 0; 
	z_rot = 0; 

	x_scale = 1; 
	y_scale = 1; 
	z_scale = 1; 
}

void ModelInfo::SetName(std::string s)
{
	name = s; 
}

void ModelInfo::SetModelPath(std::string s)
{
	model_path = s; 
}

void ModelInfo::SetTexturePath(std::string s)
{
	tex_diffuse_path = s;
}

void ModelInfo::SetPosition(float x_, float y_, float z_)
{
	x_pos = x_; 
	y_pos = y_; 
	z_pos = z_; 
}

void ModelInfo::SetOrientation(float x_, float y_, float z_)
{
	x_rot = x_;
	y_rot = y_; 
	z_rot = z_; 
}

void ModelInfo::SetScale(float x_, float y_, float z_)
{
	x_scale = x_; 
	y_scale = y_; 
	z_scale = z_; 
}