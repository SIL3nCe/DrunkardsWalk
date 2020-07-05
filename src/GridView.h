#pragma once

#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include <utility>
#include <vector>

enum class EDiagonalUse
{
	no_diagonals,
	with_diagonals,
	only_diagonals,
};

class GridView
{
public:
	GridView(int gridWidth, int gridHeight);

	void Draw(sf::RenderWindow& window);
	void DrawGUI();

	void Reset();

	void VisitCell(const std::pair<int, int>& vCoord);
	void VisitCell(int x, int y);

	bool OnMouseClicked(int posX, int posY);

	inline bool IsValidID(int x, int y) const;

	void VisitRandomCellCoordinates();

	void VisitRandomNeighbour(bool bAvoidVisitedCells, EDiagonalUse eDiagonals);

	void ApplySmoothing(int smoothingTrigger, bool bDiagonals);

private:
	void SetCurrentMaxGridSize();

	int GetNumberOfEmptyNeighbours(int x, int y, bool bDiagonals);

private:
	sf::VertexArray m_vertices;

	std::vector<std::vector<bool>> m_aaGrid;

	int m_EmptyCellCount;
	int m_SmoothedCellCount;
	std::vector<std::pair<int, int>> m_aVisitedCells;

	float m_fCellSize;
	float m_CellColor[4];

	int m_windowGridWidth;
	int m_windowGridHeight;

	int m_gridWidth;
	int m_gridHeight;

	int m_currentMaxGridWidth;
	int m_currentMaxGridHeight;
};
