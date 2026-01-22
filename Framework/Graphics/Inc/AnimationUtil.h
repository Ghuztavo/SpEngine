#pragma once

#include "ModelManager.h"

namespace SpEngine::Graphics::AnimationUtil
{
	// defining a vector of bone matrices to use for skeleton calculations
	using BoneTransforms = std::vector<Math::Matrix4>;

	// computes the matrices for all the bones in the hierarchy
	void ComputeBoneTransforms(ModelId modelId, BoneTransforms& boneTransforms);
	// to be called after ComputeBoneTransforms, draws the skeleton hierarchy
	void DrawSkeleton(ModelId modelId, const BoneTransforms& boneTransforms);
	// to be called to apply bone offsets for skining data
	void ApplyBoneOffset(ModelId modelId, BoneTransforms& boneTransforms);
}