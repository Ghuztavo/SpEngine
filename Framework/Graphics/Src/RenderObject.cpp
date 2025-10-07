#include "Precompiled.h"
#include "RenderObject.h"

using namespace SpEngine;
using namespace SpEngine::Graphics;

void RenderObject::Terminate()
{
	meshBuffer.Terminate();
}