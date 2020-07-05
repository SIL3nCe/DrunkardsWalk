#include "GridView.h"

#define MIN_CELL_SIZE 4.0f

using namespace std;

GridView::GridView(int gridWidth, int gridHeight)
: m_vertices()
, m_aaGrid()
, m_EmptyCellCount(0)
, m_SmoothedCellCount(0)
, m_aVisitedCells()
, m_fCellSize(MIN_CELL_SIZE)
, m_CellColor{ 1.0f, 1.0f , 1.0f, 1.0f }
, m_windowGridWidth(gridWidth)
, m_windowGridHeight(gridHeight)
{
	m_gridWidth = m_windowGridWidth / MIN_CELL_SIZE * 0.70f;
	m_gridHeight = m_windowGridHeight / MIN_CELL_SIZE * 0.9f;

	SetCurrentMaxGridSize();

	m_aaGrid.reserve(m_gridHeight);

	for (int i = 0; i < m_gridHeight; ++i)
	{
		vector<bool> aLine(m_gridWidth);
		m_aaGrid.push_back(aLine);

		for (int j = 0; j < m_gridWidth; ++j)
		{
			m_aaGrid[i][j] = false;
		}
	}

	m_vertices.setPrimitiveType(sf::PrimitiveType::Quads);
}

void GridView::SetCurrentMaxGridSize()
{
	m_currentMaxGridWidth = m_windowGridWidth / m_fCellSize * 0.70f;
	m_currentMaxGridHeight = m_windowGridHeight / m_fCellSize * 0.9f;
}

void GridView::Draw(sf::RenderWindow& window)
{
	m_vertices.clear();
	m_vertices.resize(4 * m_EmptyCellCount + 4 * m_SmoothedCellCount);

	sf::Color color(m_CellColor[0] * 255, m_CellColor[1] * 255, m_CellColor[2] * 255, m_CellColor[3] * 255);

	int currCell = 0;
	for (int i = 0; i < m_currentMaxGridHeight; ++i)
	{
		for (int j = 0; j < m_currentMaxGridWidth; ++j)
		{
			if (m_aaGrid[i][j])
			{
				sf::Vertex* quad = &m_vertices[currCell * 4];

				quad[0].position = sf::Vector2f(j * m_fCellSize, i * m_fCellSize);
				quad[1].position = sf::Vector2f(j * m_fCellSize + m_fCellSize, i * m_fCellSize);
				quad[2].position = sf::Vector2f(j * m_fCellSize + m_fCellSize, i * m_fCellSize + m_fCellSize);
				quad[3].position = sf::Vector2f(j * m_fCellSize, i * m_fCellSize + m_fCellSize);

				quad[0].color = color;
				quad[1].color = color;
				quad[2].color = color;
				quad[3].color = color;

				currCell++;
			}
		}
	}

	window.draw(m_vertices);
}

void GridView::DrawGUI()
{
	if (ImGui::Begin("Grid"))
	{
		ImGui::Text("%d / %d / %d (visited/smoothed/max)", m_EmptyCellCount, m_SmoothedCellCount, m_currentMaxGridWidth * m_currentMaxGridHeight);
		
		ImGui::Spacing();
		ImGui::SliderFloat("Cell Size", &m_fCellSize, MIN_CELL_SIZE, 10.0f, "%.2f");
		SetCurrentMaxGridSize();

		ImGui::Spacing();
		ImGui::ColorEdit4("Cell Color", m_CellColor);
	}
	ImGui::End();
}

void GridView::Reset()
{
	for (int i = 0; i < m_gridHeight; ++i)
	{
		for (int j = 0; j < m_gridWidth; ++j)
		{
			m_aaGrid[i][j] = false;
		}
	}

	m_aVisitedCells.clear();
	m_EmptyCellCount = 0;
	m_SmoothedCellCount = 0;
}

void GridView::VisitCell(const pair<int, int>& vCoord)
{
	VisitCell(vCoord.first, vCoord.second);
}

void GridView::VisitCell(int x, int y)
{
	if (!IsValidID(x, y))
		return;

	if (!m_aaGrid[x][y])
		m_EmptyCellCount++; // Only if not visited yet

	m_aVisitedCells.push_back({ x, y });

	m_aaGrid[x][y] = true;
}

bool GridView::OnMouseClicked(int posX, int posY)
{
	int x = posY / m_fCellSize;
	int y = posX / m_fCellSize;

	if (!IsValidID(x, y))
		return false;

	VisitCell(x, y);
	return true;
}

inline bool GridView::IsValidID(int x, int y) const
{
	return (x >= 0 && x < m_currentMaxGridHeight&& y >= 0 && y < m_currentMaxGridWidth);
}

void GridView::VisitRandomCellCoordinates()
{
	VisitCell(rand() % m_currentMaxGridHeight, rand() % m_currentMaxGridWidth);
}

void GridView::VisitRandomNeighbour(bool bAvoidVisitedCells, EDiagonalUse eDiagonals)
{
	if (m_aVisitedCells.empty())
	{
		VisitRandomCellCoordinates();
		return;
	}

	const pair<int, int>& vLastCell = m_aVisitedCells.back();

	vector<pair<int, int>> aNeighbours;

	if (eDiagonals != EDiagonalUse::only_diagonals)
	{
		if (IsValidID(vLastCell.first, vLastCell.second + 1) && (!bAvoidVisitedCells || !m_aaGrid[vLastCell.first][vLastCell.second + 1]))
			aNeighbours.push_back({ vLastCell.first, vLastCell.second + 1 });

		if (IsValidID(vLastCell.first, vLastCell.second - 1) && (!bAvoidVisitedCells || !m_aaGrid[vLastCell.first][vLastCell.second - 1]))
			aNeighbours.push_back({ vLastCell.first, vLastCell.second - 1 });

		if (IsValidID(vLastCell.first + 1, vLastCell.second) && (!bAvoidVisitedCells || !m_aaGrid[vLastCell.first + 1][vLastCell.second]))
			aNeighbours.push_back({ vLastCell.first + 1, vLastCell.second });

		if (IsValidID(vLastCell.first - 1, vLastCell.second) && (!bAvoidVisitedCells || !m_aaGrid[vLastCell.first - 1][vLastCell.second]))
			aNeighbours.push_back({ vLastCell.first - 1, vLastCell.second });
	}

	if (eDiagonals != EDiagonalUse::no_diagonals)
	{
		if (IsValidID(vLastCell.first + 1, vLastCell.second + 1) && (!bAvoidVisitedCells || !m_aaGrid[vLastCell.first + 1][vLastCell.second + 1]))
			aNeighbours.push_back({ vLastCell.first + 1, vLastCell.second + 1 });

		if (IsValidID(vLastCell.first - 1, vLastCell.second - 1) && (!bAvoidVisitedCells || !m_aaGrid[vLastCell.first - 1][vLastCell.second - 1]))
			aNeighbours.push_back({ vLastCell.first - 1, vLastCell.second - 1 });

		if (IsValidID(vLastCell.first + 1, vLastCell.second - 1) && (!bAvoidVisitedCells || !m_aaGrid[vLastCell.first + 1][vLastCell.second - 1]))
			aNeighbours.push_back({ vLastCell.first + 1, vLastCell.second - 1 });

		if (IsValidID(vLastCell.first - 1, vLastCell.second + 1) && (!bAvoidVisitedCells || !m_aaGrid[vLastCell.first - 1][vLastCell.second + 1]))
			aNeighbours.push_back({ vLastCell.first - 1, vLastCell.second + 1});
	}

	if (aNeighbours.size() == 0)
	{
		m_aVisitedCells.pop_back();
		VisitRandomNeighbour(bAvoidVisitedCells, eDiagonals);
	}
	else
	{
		VisitCell(aNeighbours[rand() % aNeighbours.size()]);
	}
}

void GridView::ApplySmoothing(int smoothingTrigger, bool bDiagonals)
{
	vector<pair<int, int>> aSmoothedCells;

	for (int i = 0; i < m_currentMaxGridHeight; ++i)
	{
		for (int j = 0; j < m_currentMaxGridWidth; ++j)
		{
			if (!m_aaGrid[i][j])
			{
				if (GetNumberOfEmptyNeighbours(i, j, bDiagonals) >= smoothingTrigger)
				{
					aSmoothedCells.push_back({ i, j });
				}
			}
		}
	}

	m_SmoothedCellCount += aSmoothedCells.size();

	for (const pair<int, int>& vCell : aSmoothedCells)
	{
		m_aaGrid[vCell.first][vCell.second] = true;
	}
}

int GridView::GetNumberOfEmptyNeighbours(int x, int y, bool bDiagonals)
{
	int neighbours = 0;

	if (IsValidID(x, y + 1) && m_aaGrid[x][y + 1])
		neighbours++;

	if (IsValidID(x, y - 1) && m_aaGrid[x][y - 1])
		neighbours++;

	if (IsValidID(x + 1, y) && m_aaGrid[x + 1][y])
		neighbours++;

	if (IsValidID(x - 1, y) && m_aaGrid[x - 1][y])
		neighbours++;

	if (bDiagonals)
	{
		if (IsValidID(x + 1, y + 1) && m_aaGrid[x + 1][y + 1])
			neighbours++;

		if (IsValidID(x - 1, y + 1) && m_aaGrid[x - 1][y + 1])
			neighbours++;

		if (IsValidID(x + 1, y - 1) && m_aaGrid[x + 1][y - 1])
			neighbours++;

		if (IsValidID(x - 1, y - 1) && m_aaGrid[x - 1][y - 1])
			neighbours++;
	}

	return neighbours;
}
