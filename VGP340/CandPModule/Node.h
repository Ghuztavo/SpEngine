#include <SpEngine/Inc/SpEngine.h>

struct Node
{
	float size;
	int row, col;
	SpEngine::Math::Vector3 position;
	// object that will be placed on the node if its blocked
	bool blocked = false;
	bool selected = false;

	// A* related variables
	float gCost = FLT_MAX; // cost from start to current node
	float hCost = FLT_MAX; // heuristic cost from current node to end
	float fCost = FLT_MAX;

	Node* parent = nullptr;
	bool visited = false;

	// constructor
	Node(float size, int row, int col)
		: size(size), row(row), col(col)
	{
		position = SpEngine::Math::Vector3(col * size, 0.0f, row * size);
	}

	// draw nodes with lines
	void Draw()
	{

	}

	void Reset()
	{
		gCost = FLT_MAX;
		hCost = FLT_MAX;
		fCost = FLT_MAX;
		parent = nullptr;
		visited = false;
	}
};