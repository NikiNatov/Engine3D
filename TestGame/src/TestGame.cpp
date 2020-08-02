#include "Engine3D.h"

class TestGame : public E3D::Application
{
public:
	TestGame()
	{

	}

	~TestGame()
	{

	}
};

E3D::Application* E3D::Create()
{
	return new TestGame();
}