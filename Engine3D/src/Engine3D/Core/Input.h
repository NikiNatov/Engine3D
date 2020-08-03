#pragma once

namespace E3D
{
	struct MousePosition
	{
		float x, y;
	};

	class Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int button);

		static MousePosition GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}