#include "Ai.h"
#include <queue>

void Ai::AStarSearch(Node* start, Node* goal)
{
	aStarPath.clear();

	auto Heuristic = [](Node* a, Node* b) {
		return std::abs(a->row - b->row) + std::abs(a->col - b->col); // Manhattan distance heuristic
		};

	// lambda function to compare nodes based on their fCost and if those are equal their hCost
	auto Compare = [](Node* a, Node* b) {
		return a->fCost > b->fCost || (a->fCost == b->fCost && a->hCost > b->hCost); 
		};

	// Priority queue to manage open nodes to explore
	std::priority_queue<Node*, std::vector<Node*>, decltype(Compare)> openSet(Compare);


	for (auto& row : grid)
	{
		for(auto& node : row)
		{
			node.Reset();
		}
	}

	start->gCost = 0.0f;


	start->hCost = Heuristic(start, goal);
	start->fCost = start->hCost;

	openSet.push(start);

	while (!openSet.empty())
	{
		Node* current = openSet.top();
		openSet.pop();
		current->visited = true;

		if (current == goal)
		{
			while (current != nullptr)
			{
				aStarPath.push_back(current);
				current = current->parent;
			}
			return;
		}

		for (Node* neighbor : GetNeighbor(current))
		{
			if (neighbor->blocked || neighbor->visited)
			{
				continue;
			}

			float tentative_gScore = current->gCost + 1.0f; // assuming uniform cost for moving to a neighbor
			if (tentative_gScore < neighbor->gCost)
			{
				neighbor->gCost = tentative_gScore;
				neighbor->hCost = Heuristic(neighbor, goal);
				neighbor->fCost = neighbor->gCost + neighbor->hCost;

				neighbor->parent = current;

				openSet.push(neighbor);
			}
		}
	}
}