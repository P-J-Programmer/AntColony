// View
constexpr float AspectRatio = 4.0f / 3.0f;
constexpr float TileScale = 0.1f;
constexpr float AntToTileSizeRatio = 0.5f;
constexpr float DefaultCameraHeight = 1.0f;

// Map Generation
constexpr unsigned int NumGridRows = 50;
constexpr unsigned int NumGridCols = NumGridRows;

constexpr float DefaultMudTileFrequency = 0.35f;
constexpr float DefaultGrassTileFrequency = 0.65f;
constexpr int MudToGrassTransitionThreshold = 3;
constexpr int GrassToMudTransitionThreshold = 4;

constexpr int NumNests = 1;
constexpr int NumFoodSources = 40;

// Simulation
constexpr float EvaporationRatePerSecond = -1.0f;
constexpr float PheromoneDepositSize = 300.0f;
constexpr int FoodPerSource = 80;
constexpr int AntsPerNest = 25;

// Controls
constexpr float MinCameraSpeed = 0.0f;
constexpr float MaxCameraSpeed = 20.0f;
constexpr float MinCameraZoom = 0.1f;
constexpr float MaxCameraZoom = 3.0f;