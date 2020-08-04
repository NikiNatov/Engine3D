#include "pch.h"
#include "RenderAPI.h"

namespace E3D
{
	RenderAPI::API RenderAPI::s_API = RenderAPI::API::OpenGL;

	RenderAPI::API RenderAPI::GetAPI()
	{
		return s_API;
	}
}