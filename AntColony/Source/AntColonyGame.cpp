#include "AntColonyGame.h"

#include "Imgui/imgui.h"

#include "Core/Input.h"
#include "Renderer/Renderer.h"
#include "Renderer/OffsetBuffer.h"

#include "GameConfig.h"
#include "GridGeneratorUtilities.h"

std::unique_ptr<FGame> CreateGame()
{
    return std::make_unique<FAntColonyGame>();
}

FAntColonyGame::FAntColonyGame::FAntColonyGame()
    : FGame()
    , m_CameraController(AspectRatio, DefaultCameraHeight)
    , m_TileGrid(DefaultGridSettings())
    , m_FoodManager(m_TileGrid)
    , m_World(m_TileGrid, m_FoodManager)
{
    GenerateWorld();
    RegisterMaterials();
}

void FAntColonyGame::SubmitRenderCommands(FRenderer& Renderer)
{
    FScene Scene{ m_CameraController.GetViewProjection() };
    Renderer.SetScene("Shader", Scene);
    RenderOpaqueObjects(Renderer);
    RenderTransparentObjects(Renderer);  
}

void FAntColonyGame::DrawUi()
{
	ImGui::NewFrame();

	ImGui::Begin("Camera Settings");
	ImGui::Text("Use WASD to move Camera");               
    ImGui::SliderFloat("Speed:", &m_CameraSpeed, MinCameraSpeed, MaxCameraSpeed);
    ImGui::SliderFloat("Zoom:", &m_CameraZoom, MinCameraZoom, MaxCameraZoom);
	ImGui::End();

    ImGui::Begin("Map");       
    if (ImGui::Button("Regenerate"))
    {
        m_bRegenerateGrid = true;
    }
    else
    {
        m_bRegenerateGrid = false;
    }
    ImGui::End();
}

void FAntColonyGame::HandleInput(const FInput& Input, const float DeltaSeconds)
{
    m_CameraController.HandleInput(Input, DeltaSeconds);
}

void FAntColonyGame::Update(const float DeltaSeconds)
{
    m_CameraController.SetCameraSpeed(m_CameraSpeed);
    m_CameraController.SetCameraZoom(m_CameraZoom);

    if (m_bRegenerateGrid)
    {
        ResetWorld();
        GenerateWorld();
    }

    for (std::unique_ptr<FAntNest>& Nest : m_Nests)
    {
        Nest->Update(DeltaSeconds);
    }
}

void FAntColonyGame::GenerateWorld()
{
    FTileFrequencies TileFrequencies = GridGeneratorUtility::GenerateDefaultTileFrequencies();
    FGeneratorRules Rules = GridGeneratorUtility::GenerateDefaultRules();
    m_GridGenerator = std::make_unique<FGridGenerator>(m_TileGrid, Rules, TileFrequencies);
    
    FGridPositions NestPositions = GridGeneratorUtility::PlaceNests(*m_GridGenerator, NumNests);
    for (const FGridPosition& GridPosition : NestPositions)
    {
        m_Nests.emplace_back(std::make_unique<FAntNest>(m_World, GridPosition)); 
        m_Nests.back()->SpawnAnts(AntsPerNest);
    }

    FGridPositions FoodPositions = GridGeneratorUtility::PlaceFoodSources(*m_GridGenerator, NumFoodSources);
    m_FoodManager.AddFoodSources(FoodPositions, FoodPerSource);
}

void FAntColonyGame::ResetWorld()
{
    m_Nests.clear();
    m_TileGrid.Reset();
    m_FoodManager.Reset();
}

void FAntColonyGame::RenderOpaqueObjects(FRenderer& Renderer) const
{
    static FRenderVertexData RenderVertexData;
    m_TileGrid.SubmitVerticesToBuffer(Renderer.GetTextureManager(), RenderVertexData);
    Renderer.SubmitDrawRequest(m_SpriteMaterial, RenderVertexData.Vertices, RenderVertexData.Indices.GetBuffer());
    ClearRenderVertexData(RenderVertexData);
}

void FAntColonyGame::RenderTransparentObjects(FRenderer& Renderer) const
{
    static FRenderVertexData RenderVertexData;
    for (const std::unique_ptr<FAntNest>& Nest : m_Nests)
    {
        Nest->SubmitVerticesToBuffer(Renderer.GetTextureManager(), RenderVertexData);
    }
    Renderer.SubmitDrawRequest(m_SpriteMaterial, RenderVertexData.Vertices, RenderVertexData.Indices.GetBuffer());
    ClearRenderVertexData(RenderVertexData);
}

void FAntColonyGame::RegisterMaterials()
{
    m_SpriteMaterial.Shader = "Shader";
    FTextureUnit TextureUnit;
    TextureUnit.Asset = "AntColonySpriteSheet";
    TextureUnit.Uniform = "u_Texture";
    TextureUnit.Slot = 0;
    m_SpriteMaterial.Textures.emplace_back(TextureUnit);
}
