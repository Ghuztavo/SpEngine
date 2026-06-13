#include "Ai.h"
#include <queue>

// simple A* algorithm I got from a previous project that I tried to apply to this engine
// it was originally made in 2d with raylib

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

std::vector<Node*> Ai::AStarSearchThreadSafe(Node* start, Node* goal) const
{
	std::vector<Node*> path;

	// local state array instead of copying the grid
	struct SearchState {
		float gCost;
		float hCost;
		float fCost;
		Node* parent;
		bool visited;
	};

	thread_local std::unique_ptr<SearchState[]> states = std::make_unique<SearchState[]>(ROW_COUNT * COL_COUNT);

	for (int i = 0; i < ROW_COUNT * COL_COUNT; ++i)
	{
		states[i].gCost = FLT_MAX;
		states[i].hCost = FLT_MAX;
		states[i].fCost = FLT_MAX;
		states[i].parent = nullptr;
		states[i].visited = false;
	}

	auto& statesRef = states;
	auto GetState = [&statesRef](int r, int c) -> SearchState& {
		return statesRef[r * COL_COUNT + c];
		};

	auto Heuristic = [](Node* a, Node* b) {
		return std::abs(a->row - b->row) + std::abs(a->col - b->col);
		};

	auto Compare = [&GetState](Node* a, Node* b) {
		SearchState& stateA = GetState(a->row, a->col);
		SearchState& stateB = GetState(b->row, b->col);
		return stateA.fCost > stateB.fCost || (stateA.fCost == stateB.fCost && stateA.hCost > stateB.hCost);
		};

	std::priority_queue<Node*, std::vector<Node*>, decltype(Compare)> openSet(Compare);

	SearchState& startState = GetState(start->row, start->col);
	startState.gCost = 0.0f;
	startState.hCost = Heuristic(start, goal);
	startState.fCost = startState.hCost;

	openSet.push(start);

	while (!openSet.empty())
	{
		Node* current = openSet.top();
		openSet.pop();

		SearchState& currentState = GetState(current->row, current->col);
		currentState.visited = true;

		if (current == goal)
		{
			Node* curr = current;
			while (curr != nullptr)
			{
				path.push_back(curr);
				curr = GetState(curr->row, curr->col).parent;
			}
			return path;
		}

		Node* neighbors[4] = { nullptr, nullptr, nullptr, nullptr };
		int ncount = 0;
		int r = current->row;
		int c = current->col;

		if (r - 1 >= 0) neighbors[ncount++] = const_cast<Node*>(&grid[r - 1][c]);
		if (r + 1 < ROW_COUNT) neighbors[ncount++] = const_cast<Node*>(&grid[r + 1][c]);
		if (c + 1 < COL_COUNT) neighbors[ncount++] = const_cast<Node*>(&grid[r][c + 1]);
		if (c - 1 >= 0) neighbors[ncount++] = const_cast<Node*>(&grid[r][c - 1]);

		for (int i = 0; i < ncount; ++i)
		{
			Node* neighbor = neighbors[i];
			SearchState& neighborState = GetState(neighbor->row, neighbor->col);

			if (neighbor->blocked || neighborState.visited)
				continue;

			float tentative_gScore = currentState.gCost + 1.0f;
			if (tentative_gScore < neighborState.gCost)
			{
				neighborState.gCost = tentative_gScore;
				neighborState.hCost = Heuristic(neighbor, goal);
				neighborState.fCost = neighborState.gCost + neighborState.hCost;
				neighborState.parent = current;
				openSet.push(neighbor);
			}
		}
	}

	return path;
}