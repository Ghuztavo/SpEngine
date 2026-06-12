#pragma once

#include <SpEngine/Inc/SpEngine.h>
#include "Ai.h"

class GameState : public SpEngine::AppState 
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	std::filesystem::path mLevelFile;
	SpEngine::GameWorld mGameWorld;
	Ai mAi;
};