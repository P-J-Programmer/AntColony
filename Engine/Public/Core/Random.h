#pragma once

#include <random>

class Random
{
public:
	/*
	* Returns T in range [Min, Max]
	*/
	template<typename T>
	static T RandInRange(T Min, T Max) 
	{
		assert(Min < Max);
		typename Distribution<T>::Type Dist(Min, Max);
		FEngineType& Engine = GetEngine();
		return Dist(Engine);
	}

private:
	template <typename T>
	struct Distribution
	{
		using Type = std::conditional_t<std::is_integral_v<T>, std::uniform_int_distribution<T>, std::uniform_real_distribution<T>>;
	};

	using FEngineType = std::mt19937;

private:
	static FEngineType& GetEngine()
	{
		static std::random_device SeedGenerator;
		static std::mt19937 Engine(SeedGenerator());
		return Engine;
	}
};