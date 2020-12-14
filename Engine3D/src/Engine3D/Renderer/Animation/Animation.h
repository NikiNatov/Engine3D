#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>

#include "Engine3D\Core\Timestep.h"
#include "Engine3D\Core\Config.h"

// Changed

namespace E3D
{
	struct AnimatedVertex
	{
		glm::vec3 Position;
		glm::vec2 TextureCoord;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
		glm::uvec4 BoneIDs;
		glm::vec4 BoneWights;
	};

	class Bone
	{
	public:
		Bone()
			: m_ID(-1), m_Name(""), m_AnimatedTransform(glm::mat4(1.0)), m_BoneSpaceTransform(glm::mat4(1.0)), m_InverseModelSpaceTransform(glm::mat4(1.0f))
		{
		}

		Bone(int32_t ID, const std::string& name, const glm::mat4& boneSpaceTransform)
			: m_ID(ID), m_Name(name), m_AnimatedTransform(glm::mat4(1.0f)), m_BoneSpaceTransform(boneSpaceTransform), m_InverseModelSpaceTransform(glm::mat4(1.0f))
		{
		}

		void AddChild(const Bone& child)
		{
			m_Children.push_back(child);
		}

		void CalculateInverseModelSpaceTransform(const glm::mat4& parentModelSpaceTransform)
		{
			glm::mat4 boneModelSpaceTransform = glm::inverse(parentModelSpaceTransform * m_BoneSpaceTransform);
			m_InverseModelSpaceTransform = boneModelSpaceTransform;

			for (auto& child : m_Children)
				child.CalculateInverseModelSpaceTransform(boneModelSpaceTransform);
		}

		inline glm::mat4& AnimatedTransform() { return m_AnimatedTransform; }
		inline const glm::mat4& InverseModelSpaceTransform() const { return m_InverseModelSpaceTransform; }
		inline const std::string& Name() const { return m_Name; }
		inline int32_t ID() const { return m_ID; }
		inline const std::vector<Bone>& Children() const { return m_Children; }

	private:
		int32_t m_ID;
		std::string m_Name;
		glm::mat4 m_AnimatedTransform;				// Transform of the final pose(model space)
		glm::mat4 m_BoneSpaceTransform;				// Transform in relation to the parent bone
		glm::mat4 m_InverseModelSpaceTransform;		// Original position model space transform but inverted
		std::vector<Bone> m_Children;
	};

	class BoneTransform
	{
	public:
		BoneTransform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
			: m_Position(position), m_Rotation(rotation), m_Scale(scale)
		{
		}

		glm::mat4 GetTransformMatrix() const
		{
			glm::mat4 matrix = glm::mat4(1.0);
			matrix = glm::translate(glm::mat4(1.0), m_Position) * glm::toMat4(m_Rotation) * glm::scale(glm::mat4(1.0), m_Scale);
			return matrix;
		}

		static BoneTransform Interpolate(const BoneTransform& first, const BoneTransform& second, float factor)
		{
			glm::vec3 interpolatedPos = glm::mix(first.m_Position, second.m_Position, factor);
			glm::quat interpolatedRotation = glm::slerp(first.m_Rotation, second.m_Rotation, factor);
			glm::vec3 interpolatedScale = glm::mix(first.m_Scale, second.m_Scale, factor);

			return BoneTransform(interpolatedPos, interpolatedRotation, interpolatedScale);
		}
	private:
		glm::vec3 m_Position;
		glm::quat m_Rotation;
		glm::vec3 m_Scale;
	};

	class KeyFrame
	{
		using BoneMap = std::unordered_map<std::string, BoneTransform>;
	public:
		KeyFrame(float timeStamp, const BoneMap& pose)
			: m_TimeStamp(timeStamp), m_Pose(pose)
		{
		}

		inline float Timestamp() const { return m_TimeStamp; }
		inline const BoneMap& Pose() const { return m_Pose; }
	private:
		float m_TimeStamp;
		BoneMap m_Pose;		// Bone-space transforms of the bones at the current keyframe
	};

	class Animation
	{
	public:
		Animation(float duration, const std::vector<KeyFrame>& keyFrames)
			: m_Duration(duration), m_KeyFrames(keyFrames)
		{
		}

		inline float Duration() const { return m_Duration; }
		inline const std::vector<KeyFrame>& KeyFrames() const { return m_KeyFrames; }
	private:
		float m_Duration;
		std::vector<KeyFrame> m_KeyFrames;
	};

	class Animator
	{
	public:
		/*Animator(const Ref<AnimatedModel>& model)
			: m_Model(model)
		{}*/

		void Update(Timestep ts)
		{
			if (!m_CurrentAnimation)
			{
				E3D_CORE_ASSERT(false, "Assertion Failed: No animation was set");
				return;
			}

			IncreaseTime(ts);
			std::unordered_map<std::string, glm::mat4> currentBoneTransforms = CalculateBoneTransforms();
			//ApplyTransforms(currentBoneTransforms, skeleton, parentTransform)
		}

		void SetAnimation(const Ref<Animation>& animation)
		{
			m_AnimationTime = 0.0f;
			m_CurrentAnimation = animation;
		}
	private:
		void IncreaseTime(Timestep ts)
		{
			m_AnimationTime += ts;
			if (m_AnimationTime > m_CurrentAnimation->Duration())
				m_AnimationTime = std::fmod(m_AnimationTime, m_CurrentAnimation->Duration());
		}

		std::unordered_map<std::string, glm::mat4> CalculateBoneTransforms()
		{
			auto& [prevFrame, nextFrame] = GetPreviousAndNextKeyFrames();
			float progression = CalculateProgressionBetweenFrames(prevFrame, nextFrame);
			return InterpolateTransforms(prevFrame, nextFrame, progression);
		}

		std::pair<KeyFrame, KeyFrame> GetPreviousAndNextKeyFrames()
		{
			auto& animKeyFrames = m_CurrentAnimation->KeyFrames();
			KeyFrame prev = animKeyFrames.front();
			KeyFrame next = animKeyFrames.front();

			for (auto& keyframe : animKeyFrames)
			{
				next = keyframe;
				if (keyframe.Timestamp() > m_AnimationTime)
					break;
				prev = keyframe;
			}

			return { prev, next };
		}

		float CalculateProgressionBetweenFrames(const KeyFrame& first, const KeyFrame& second)
		{
			float ts = second.Timestamp() - first.Timestamp();
			float currentTime = m_AnimationTime - first.Timestamp();
			return currentTime / ts;
		}

		std::unordered_map<std::string, glm::mat4> InterpolateTransforms(const KeyFrame& prev, const KeyFrame& next, float progression)
		{
			std::unordered_map<std::string, glm::mat4> currentBoneTransforms;

			auto& boneMap = prev.Pose();

			for (auto&[boneName, transform] : boneMap)
			{
				 const BoneTransform& prevFrameTransform = transform;
				 const BoneTransform& nextFrameTransform = next.Pose().at(boneName);
				 BoneTransform currentTransform = BoneTransform::Interpolate(prevFrameTransform, nextFrameTransform, progression);
				 currentBoneTransforms[boneName] = currentTransform.GetTransformMatrix();
			}

			return currentBoneTransforms;
		}
	private:
		Ref<Animation> m_CurrentAnimation;
		float m_AnimationTime;
		// Ref<AnimatedModel> m_Model;
	};

}