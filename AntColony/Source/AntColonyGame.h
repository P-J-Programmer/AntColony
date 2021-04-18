#pragma once

#include <memory>
#include <vector>

#include "Core/Game.h"
#include "GameFramework/CameraController.h"
#include "Renderer/Material.h"

#include "AntNest.h"
#include "AntWorld.h"
#include "FoodManager.h"
#include "GridGenerator.h"
#include "TileGrid.h"

std::unique_ptr<FGame> CreateGame();

class FAntColonyGame : public FGame
{
public:
	FAntColonyGame();

    virtual void SubmitRenderCommands(FRenderer& ShaderManager) override; 
    virtual void DrawUi() override;
    virtual void HandleInput(const FInput& Input, const float DeltaSeconds) override;
    virtual void Update(const float DeltaSeconds) override;

private:
    void GenerateWorld();
    void ResetWorld();

    void RenderOpaqueObjects(FRenderer& Renderer) const;
    void RenderTransparentObjects(FRenderer& Renderer) const;
    void RegisterMaterials();

private:
	FCameraController m_CameraController;
    FTileGrid m_TileGrid;
    FFoodManager m_FoodManager;
    FAntWorld m_World;

    FMaterial m_SpriteMaterial;

    std::unique_ptr<FGridGenerator> m_GridGenerator = nullptr;
    std::vector<std::unique_ptr<FAntNest>> m_Nests;

    float m_CameraSpeed = 2.5f;
    float m_CameraZoom = 1.0f;

    bool m_bRegenerateGrid = false;
};