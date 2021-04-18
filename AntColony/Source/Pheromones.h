#pragma once

enum class EPheromoneType : unsigned char
{
	Attractive,

	Num
};

constexpr std::size_t NumPheromoneTypes = static_cast<unsigned int>(EPheromoneType::Num);
constexpr std::size_t ToIndex(const EPheromoneType PheromoneType) { return static_cast<std::size_t>(PheromoneType); }