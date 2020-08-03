#include "Engine3D.h"

class ExampleLayer : public E3D::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	virtual void OnUpdate() override
	{

	}

	virtual void OnEvent(E3D::Event& event) override
	{
		E3D_LOG_TRACE(event);
	}

	virtual void OnImGuiRender() override
	{

	}

	virtual void OnAttach() override
	{

	}

	virtual void OnDetach() override
	{

	}
private:
};


class TestGame : public E3D::Application
{
public:
	TestGame()
	{
		PushOverlay(new ExampleLayer());
	}

	~TestGame()
	{

	}
};

E3D::Application* E3D::Create()
{
	return new TestGame();
}