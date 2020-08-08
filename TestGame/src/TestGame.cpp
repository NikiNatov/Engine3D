#include <Engine3D.h>
#include <Engine3D\Core\EntryPoint.h>

#include "ExampleLayer.h"

class TestGame : public E3D::Application
{
public:
	TestGame()
		: Application("Test Game")
	{
		PushLayer(new ExampleLayer());
	}

	~TestGame()
	{

	}
};

E3D::Application* E3D::Create()
{
	return new TestGame();
}