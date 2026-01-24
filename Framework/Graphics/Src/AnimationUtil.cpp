#include "Precompiled.h"
#include "AnimationUtil.h"

#include "Color.h"
#include "SimpleDraw.h"

using namespace SpEngine;
using namespace SpEngine::Graphics;

// empty namespace for global functions isolated to the cpp file
namespace
{
	void computeBoneTransformsRecursive(const Bone* bone, AnimationUtil::BoneTransforms& boneTransforms, const Animator* animator)
	{
		if (bone != nullptr)
		{
			//set the bone transform to the array of matrices
			// if there is no animator or the bone does not have any animations, use the regular toparenttransform, otherwise use the animation
			if (animator == nullptr || !animator->GetToParentTransform(bone, boneTransforms[bone->index]))
			{
				boneTransforms[bone->index] = bone->toParentTransform;
			}
			//if there is a parent, apply the parent's transform as well
			if (bone->parent != nullptr)
			{
				boneTransforms[bone->index] = boneTransforms[bone->index] * boneTransforms[bone->parentIndex];
			}
			// go through the children and apply their transforms
			for (const Bone* child : bone->children)
			{
				computeBoneTransformsRecursive(child, boneTransforms, animator);
			}
		}
	}
}

void AnimationUtil::ComputeBoneTransforms(ModelId modelId, BoneTransforms& boneTransforms, const Animator* animator)
{
	const Model* model = ModelManager::Get()->GetModel(modelId);
	if (model != nullptr && model->skeleton != nullptr)
	{
		// resize to sync the number of bones with the matrices
		boneTransforms.resize(model->skeleton->bones.size());
		// generate matrices
		computeBoneTransformsRecursive(model->skeleton->root, boneTransforms, animator);
	}
}

void AnimationUtil::DrawSkeleton(ModelId modelId, const BoneTransforms& boneTransforms)
{
	const Model* model = ModelManager::Get()->GetModel(modelId);
	if(model != nullptr && model->skeleton != nullptr)
	{
		// iterate through the unique bone pointers
		for (const auto& bone : model->skeleton->bones)
		{
			if (bone->parent != nullptr)
			{
				// gets the bone and parent bone position
				const Math::Vector3 bonePos = Math::GetTranslation(boneTransforms[bone->index]);
				const Math::Vector3 parentPos = Math::GetTranslation(boneTransforms[bone->parentIndex]);
				// draws a line from the bone to its parent
				SimpleDraw::AddLine(bonePos, parentPos, Colors::AliceBlue);
				// draws a sphere at the bone position
				SimpleDraw::AddSphere(10, 10, 0.03f, Colors::Red, bonePos);
			}
		}
	}
}

void AnimationUtil::ApplyBoneOffset(ModelId modelId, BoneTransforms& boneTransforms)
{
	const Model* model = ModelManager::Get()->GetModel(modelId);
	if (model != nullptr && model->skeleton != nullptr)
	{
		for (auto& bone : model->skeleton->bones)
		{
			boneTransforms[bone->index] = bone->offsetTransform * boneTransforms[bone->index];
		}
	}
}
