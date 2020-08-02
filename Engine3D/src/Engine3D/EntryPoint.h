#pragma once

#include "Engine3D\Application.h"
#include "Engine3D\Log.h"
#include "Engine3D\Config.h"

extern E3D::Application* E3D::Create();

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	E3D::Log::Init();

	E3D::Application* app = E3D::Create();

	E3D_CORE_LOG_INFO("Application created!");

	app->Run();

	delete app;
}