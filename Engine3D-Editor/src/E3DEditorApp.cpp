#include <Engine3D.h>
#include <Engine3D\Core\EntryPoint.h>

#include "EditorLayer.h"

namespace E3D
{
	class E3DEditor : public Application
	{
	public:
		E3DEditor()
			: Application("Engine3D Editor")
		{
			PushLayer(new EditorLayer());
		}

		~E3DEditor()
		{

		}
	};

	Application* Create()
	{
		return new E3DEditor();
	}
}