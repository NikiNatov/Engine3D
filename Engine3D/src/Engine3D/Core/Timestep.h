#pragma once


namespace E3D
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time)
		{
		}

		inline float GetSeconds() const { return m_Time; }
		inline float GetMilliseconds() const { return m_Time * 1000.0f; }

		operator float() const { return GetSeconds(); }
	private:
		float m_Time;
	};
}