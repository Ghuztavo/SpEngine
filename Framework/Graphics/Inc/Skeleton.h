#pragma once

#include "Bone.h"

namespace SpEngine::Graphics
{
	struct Skeleton
	{
		Bone* root = nullptr;
		std::vector<std::unique_ptr<Bone>> bones;


	};
}