#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>

#include "Engine3D\Core\Timestep.h"
#include "Engine3D\Core\Config.h"

#include "Engine3D\Renderer\Animation\Animation.h"
#include "Engine3D\Renderer\Shader.h"

namespace E3D
{
	class Model;

	// Bone structure used for building the skeleton of the model
	struct Bone
	{
		int32_t ID = -1;
		std::string Name;
		glm::mat4 AnimatedTransform;				// Transform of the final pose(model space)
		glm::mat4 InverseModelSpaceTransform;		// Original position model space transform but inverted
		std::vector<Bone> Children;

		void AddChild(Bone& child)
		{
			Children.push_back(child);
		}
	};

	// Stores positions, rotations and scales of a bone for every frame
	struct BoneTransforms
	{
		std::vector<std::pair<glm::vec3, float>> Positions;
		std::vector<std::pair<glm::quat, float>> Rotations;
		std::vector<std::pair<glm::vec3, float>> Scales;
	};

	// Basic structure, storing all the transformations for each bone in the animation as well as
	// how long the animation is
	struct Animation
	{
		float Duration;
		float TicksPerSecond = 1.0f;
		std::unordered_map<std::string, BoneTransforms> BoneTransformsTable;

		Animation(float duration, float ticksPerSecond, std::unordered_map<std::string, BoneTransforms>& boneTransformsTable)
			: Duration(duration), TicksPerSecond(ticksPerSecond), BoneTransformsTable(boneTransformsTable)
		{}
	};

	class Animator
	{
	public:
		Animator(Model* model);

		void Update(Timestep ts);
		void SetAnimation(const Ref<Animation>& animation);

		inline const Ref<Animation>& GetCurrentAnimation() const { return m_CurrentAnimation; }
	private:
		void IncreaseTime(Timestep ts);
		void CalculateTransforms(Bone& rootBone, const glm::mat4& parentTransform);

		std::tuple<uint32_t, uint32_t, float> GetPositionsAndInterpolationFactor(const std::vector<std::pair<glm::vec3, float>>& positions);
		std::tuple<uint32_t, uint32_t, float> GetRotationsAndInterpolationFactor(const std::vector<std::pair<glm::quat, float>>& rotations);
		std::tuple<uint32_t, uint32_t, float> GetScalesAndInterpolationFactor(const std::vector<std::pair<glm::vec3, float>>& scales);
	private:
		Ref<Animation> m_CurrentAnimation;
		float m_CurrentAnimationTime;
		Model* m_ModelHandle;
	};

}