#include "Ai.h"

std::vector<Node*> Ai::GetNeighbor(Node* node)
{
    std::vector<Node*> neighbors;

    // Helper lambda to check if a given position is within the grid and the node is not blocked
    auto isWithinGrid = [&](int row, int col) -> bool
        {
            return  row >= 0 &&
                row < (int)(grid.size()) &&
                col >= 0 &&
                col < (int)(grid[0].size());
        };

    // Check North
    if (isWithinGrid(node->row - 1, node->col))
    {
        neighbors.push_back(&grid[node->row - 1][node->col]);
    }

    // Check South
    if (isWithinGrid(node->row + 1, node->col))
    {
        neighbors.push_back(&grid[node->row + 1][node->col]);
    }

    // Check East
    if (isWithinGrid(node->row, node->col + 1))
    {
        neighbors.push_back(&grid[node->row][node->col + 1]);
    }

    // Check West
    if (isWithinGrid(node->row, node->col - 1))
    {
        neighbors.push_back(&grid[node->row][node->col - 1]);
    }

    return neighbors;
}