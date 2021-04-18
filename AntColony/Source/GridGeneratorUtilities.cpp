#include "GridGeneratorUtilities.h"

#include "GameConfig.h"

FTileFrequencies GridGeneratorUtility::GenerateDefaultTileFrequencies()
{
    std::vector<FTileFrequency> Frequencies;
    Frequencies.push_back({ ETileType::Mud, DefaultMudTileFrequency });
    Frequencies.push_back({ ETileType::Grass, DefaultGrassTileFrequency });
    return FTileFrequencies(Frequencies);
}

FGeneratorRules GridGeneratorUtility::GenerateDefaultRules()
{
    constexpr int MaxNumNeighbours = 8;

    FGeneratorRules Rules;
    for (int NumNeighbours = MudToGrassTransitionThreshold; NumNeighbours <= MaxNumNeighbours; ++NumNeighbours)
    {
        for (int NumWalls = 0; NumWalls <= MaxNumNeighbours - NumNeighbours; ++NumWalls)
        {
            const int NumMud = MaxNumNeighbours - NumNeighbours - NumWalls;
            FGeneratorRule Rule;
            Rule.NeighbourConditions[static_cast<std::size_t>(ETileType::Grass)] = NumNeighbours;
            Rule.NeighbourConditions[static_cast<std::size_t>(ETileType::Wall)] = NumWalls;
            Rule.NeighbourConditions[static_cast<std::size_t>(ETileType::Mud)] = NumMud;
            Rule.Result = ETileType::Grass;
            Rules.AddRule(ETileType::Mud, std::move(Rule));
        }
    }

    for (int NumNeighbours = GrassToMudTransitionThreshold; NumNeighbours <= MaxNumNeighbours; ++NumNeighbours)
    {
        for (int NumWalls = 0; NumWalls <= MaxNumNeighbours - NumNeighbours; ++NumWalls)
        {
            const int NumGrass = MaxNumNeighbours - NumNeighbours - NumWalls;
            FGeneratorRule Rule;
            Rule.NeighbourConditions[static_cast<std::size_t>(ETileType::Mud)] = NumNeighbours;
            Rule.NeighbourConditions[static_cast<std::size_t>(ETileType::Wall)] = NumWalls;
            Rule.NeighbourConditions[static_cast<std::size_t>(ETileType::Grass)] = NumGrass;
            Rule.Result = ETileType::Mud;
            Rules.AddRule(ETileType::Grass, std::move(Rule));
        }
    }

    return Rules;
}

FGridPositions GridGeneratorUtility::PlaceNests(FGridGenerator& Generator, int NumNests)
{
    static const std::vector<ETileType> ExcludedTiles = { ETileType::Food, ETileType::Nest, ETileType::Wall };
    FSimpleRule NestPlacementRule;
    NestPlacementRule.MinNumNeighbours = 8;
    NestPlacementRule.NeighbourType = ETileType::Grass;
    NestPlacementRule.ExcludedTiles = ExcludedTiles;
    return Generator.PlaceByRule(ETileType::Nest, NestPlacementRule, NumNests);
}

FGridPositions GridGeneratorUtility::PlaceFoodSources(FGridGenerator& Generator, int NumFoodSources)
{
    static const std::vector<ETileType> ExcludedTiles = { ETileType::Food, ETileType::Nest, ETileType::Wall };
    FSimpleRule FoodPlacementRule;
    FoodPlacementRule.MinNumNeighbours = 4;
    FoodPlacementRule.NeighbourType = ETileType::Mud;
    FoodPlacementRule.ExcludedTiles = ExcludedTiles;
    return Generator.PlaceByRule(ETileType::Food, FoodPlacementRule, NumFoodSources);
}
