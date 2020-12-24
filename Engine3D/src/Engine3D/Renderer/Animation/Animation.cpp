#include "pch.h"
#include "Animation.h"

#include "Engine3D\Renderer\Model.h"

namespace E3D
{
	Animator::Animator(Model* model)
		: m_ModelHandle(model), m_CurrentAnimation(nullptr), m_CurrentAnimationTime(0.0f)
	{
	}

	void Animator::Update(Timestep ts)
	{
		if (!m_CurrentAnimation)
		{
			E3D_CORE_ASSERT(false, "Assertion Failed: No animation was set");
			return;
		}

		IncreaseTime(ts * m_CurrentAnimation->TicksPerSecond);
		CalculateTransforms(m_ModelHandle->m_RootBone, glm::mat4(1.0f));
	}

	void Animator::SetAnimation(const Ref<Animation>& animation)
	{
		m_CurrentAnimationTime = 0.0f;
		m_CurrentAnimation = animation;
	}

	void Animator::IncreaseTime(Timestep ts)
	{
		m_CurrentAnimationTime += ts;
		if (m_CurrentAnimationTime > m_CurrentAnimation->Duration)
			m_CurrentAnimationTime = std::fmod(m_CurrentAnimationTime, m_CurrentAnimation->Duration);
	}

	void Animator::CalculateTransforms(Bone& rootBone, const glm::mat4& parentTransform)
	{
		auto& positions = m_CurrentAnimation->BoneTransformsTable[rootBone.Name].Positions;
		auto& rotations = m_CurrentAnimation->BoneTransformsTable[rootBone.Name].Rotations;
		auto& scales = m_CurrentAnimation->BoneTransformsTable[rootBone.Name].Scales;

		glm::vec3 finalPosition;
		glm::quat finalRotation;
		glm::vec3 finalScale;

		if (positions.size() == 1)
			finalPosition = positions.front().first;
		else
		{
			auto& [prevPositionIndex, nextPositionIndex, posInterpolationFactor] = GetPositionsAndInterpolationFactor(positions);
			finalPosition = glm::mix(positions[prevPositionIndex].first, positions[nextPositionIndex].first, posInterpolationFactor);
		}

		if (rotations.size() == 1)
			finalRotation = rotations.front().first;
		else
		{
			auto& [prevRotationIndex, nextRotationIndex, rotInterpolationFactor] = GetRotationsAndInterpolationFactor(rotations);
			finalRotation = glm::slerp(rotations[prevRotationIndex].first, rotations[nextRotationIndex].first, rotInterpolationFactor);
		}

		if (scales.size() == 1)
			finalScale = scales.front().first;
		else
		{
			auto& [prevScaleIndex, nextScaleIndex, scaleInterpolationFactor] = GetScalesAndInterpolationFactor(scales);
			finalScale = glm::mix(scales[prevScaleIndex].first, scales[nextScaleIndex].first, scaleInterpolationFactor);
		}

		glm::mat4 localTransform = glm::translate(glm::mat4(1.0f), finalPosition) * 
								   glm::toMat4(finalRotation) * 
								   glm::scale(glm::mat4(1.0f), finalScale);

		glm::mat4 globalTransform = parentTransform * localTransform;
		rootBone.AnimatedTransform = globalTransform * rootBone.InverseModelSpaceTransform;

		ShaderLibrary::Get("AnimatedModelShader")->SetMat4("u_BoneTransforms[" + std::to_string(rootBone.ID) + "]", 1, rootBone.AnimatedTransform);

		for (auto& child : rootBone.Children)
			CalculateTransforms(child, globalTransform);
	}

	std::tuple<uint32_t, uint32_t, float> Animator::GetPositionsAndInterpolationFactor(const std::vector<std::pair<glm::vec3, float>>& positions)
	{
		uint32_t numPositions = positions.size();
		int prevPos = -1;

		for (uint32_t i = 0; i < numPositions - 1; i++)
		{
			if (m_CurrentAnimationTime < positions[i + 1].second)
			{
				prevPos = i;
				break;
			}
		}

		E3D_CORE_ASSERT(prevPos != -1, "Assertion Failed: Could not find position index");

		int nextPos = prevPos + 1;
		float startTime = positions[prevPos].second;
		float endTime = positions[nextPos].second;
		float factor = (m_CurrentAnimationTime - startTime) / (endTime - startTime);

		return { prevPos, nextPos, factor };
	}

	std::tuple<uint32_t, uint32_t, float> Animator::GetRotationsAndInterpolationFactor(const std::vector<std::pair<glm::quat, float>>& rotations)
	{
		uint32_t numRotations = rotations.size();
		int prevRot = -1;

		for (uint32_t i = 0; i < numRotations - 1; i++)
		{
			if (m_CurrentAnimationTime < rotations[i + 1].second)
			{
				prevRot = i;
				break;
			}
		}

		E3D_CORE_ASSERT(prevRot != -1, "Assertion Failed: Could not find rotaion index");

		int nextRot = prevRot + 1;
		float startTime = rotations[prevRot].second;
		float endTime = rotations[nextRot].second;
		float factor = (m_CurrentAnimationTime - startTime) / (endTime - startTime);

		return { prevRot, nextRot, factor };
	}

	std::tuple<uint32_t, uint32_t, float> Animator::GetScalesAndInterpolationFactor(const std::vector<std::pair<glm::vec3, float>>& scales)
	{
		uint32_t numScales = scales.size();
		int prevScale = -1;

		for (uint32_t i = 0; i < numScales - 1; i++)
		{
			if (m_CurrentAnimationTime < scales[i + 1].second)
			{
				prevScale = i;
				break;
			}
		}

		E3D_CORE_ASSERT(prevScale != -1, "Assertion Failed: Could not find scale index");

		int nextScale = prevScale + 1;
		float startTime = scales[prevScale].second;
		float endTime = scales[nextScale].second;
		float factor = (m_CurrentAnimationTime - startTime) / (endTime - startTime);

		return { prevScale, nextScale, factor };
	}
}