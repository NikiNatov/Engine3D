#pragma once

namespace E3D
{
	class RenderAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
	public:
		static API GetAPI();
	private:
		static API s_API;
	};
}