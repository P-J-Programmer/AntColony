#pragma once

#include <memory>

class FGame;
class FInput;
class FRenderer;

extern std::unique_ptr<FGame> CreateGame();

class FGame
{
public:
	FGame() = default;
	virtual ~FGame() = default;

	virtual void HandleInput(const FInput& Input, const float DeltaTime) = 0;
	virtual void Update(const float DeltaTime) = 0;
	virtual void SubmitRenderCommands(FRenderer& Renderer) = 0;
	virtual void DrawUi() = 0;
};