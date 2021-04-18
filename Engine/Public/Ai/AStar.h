#pragma once

#include <unordered_map>
#include <queue>
#include <vector>

template <typename NodeType>
class IGraph
{
public:
	virtual std::vector<NodeType> GetNavigableNeighbourNodes(const NodeType& Node) const = 0;
	virtual float Cost(const NodeType& From, const NodeType& To) const = 0;
	virtual float Heuristic(const NodeType& From, const NodeType& To) const = 0;
};

template <typename NodeType>
class AStar
{
public:
	struct FPathfindingResult
	{
		std::vector<NodeType> Path;
		bool bSuccess = false;
	};

public:
	static FPathfindingResult Search(const IGraph<NodeType>& Graph, const NodeType& Start, const NodeType& Target)
	{
		FNodeQueue OpenList;
		FNodeList ClosedList;

		/* Previous node on the current shortest path Start starting point to this node */
		FNodeSourceMap Sources;

		/* Current lowest cost from start to this node */
		FPathCostMap CurrentBestCosts;

		FPathNode Root;
		Root.Node = Start;
		Root.Cost = Graph.Heuristic(Start, Target);
		OpenList.push(Root);
		Sources[Start] = Start;

		while (!OpenList.empty())
		{
			const FPathNode Current = OpenList.top();
			OpenList.pop();
			ClosedList.push_back(Current.Node);

			if (Current.Node == Target)
			{
				FPathfindingResult PathfindingResult;
				PathfindingResult.bSuccess = true;
				PathfindingResult.Path = MakePath(Start, Target, Sources);
				return PathfindingResult;
			}

			for (const NodeType& Neighbour : Graph.GetNavigableNeighbourNodes(Current.Node))
			{
				if (IsInList(Neighbour, ClosedList))
				{
					continue;
				}
				
				const float HeuristicCost = Graph.Heuristic(Neighbour, Target);
				const float SourceToNeighbourCost = Current.Cost + Graph.Cost(Current.Node, Neighbour);
				const float TotalEstimatedCost = SourceToNeighbourCost + HeuristicCost;

				FPathNode NeighbourNode;
				NeighbourNode.Node = Neighbour;
				NeighbourNode.Cost = TotalEstimatedCost;

				if (IsKnown(Neighbour, CurrentBestCosts))
				{
					const float CurrentBestCost = CurrentBestCosts[Current.Node];
					if (TotalEstimatedCost < CurrentBestCost) // Found a better path
					{
						/*
						 * Note: We don't remove the old dead elements here in order to avoid updating the priority queue.
						 * This is (one of the) standard solutions AFAIK
						 */
						Sources[Neighbour] = Current.Node;
						OpenList.push(NeighbourNode);
						CurrentBestCosts[Neighbour] = TotalEstimatedCost;

					}
				}
				else // Found a previously unknown node
				{
					OpenList.push(NeighbourNode);
					Sources[Neighbour] = Current.Node;
					CurrentBestCosts[Neighbour] = TotalEstimatedCost;
				}
			}
		}

		return FPathfindingResult();
	}

private:
	struct FPathNode
	{
		NodeType Node;
		float Cost;

		bool operator> (const FPathNode& Other) const
		{
			return Cost > Other.Cost;
		}
	};

	using FNodeQueue = std::priority_queue<FPathNode, std::vector<FPathNode>, std::greater<FPathNode>>;
	using FPathCostMap = std::unordered_map<NodeType, float>;
	using FNodeSourceMap = std::unordered_map<NodeType, NodeType>;
	using FNodeList = std::vector<NodeType>;

private:
	static bool IsInList(const NodeType& Node, const FNodeList& NodeList) { return std::find(NodeList.begin(), NodeList.end(), Node) != NodeList.end(); }
	static bool IsKnown(const NodeType& Node, const FPathCostMap& PathCostMap) { return PathCostMap.find(Node) != PathCostMap.end(); }

	static std::vector<NodeType> MakePath(const NodeType& Start, const NodeType& Target, FNodeSourceMap& NodeSources)
	{
		std::vector<NodeType> Path;
		NodeType Current = Target;
		while (Current != Start) 
		{
			Path.emplace_back(Current);
			Current = NodeSources[Current];
		}
		std::reverse(Path.begin(), Path.end());
		return Path;
	}
};