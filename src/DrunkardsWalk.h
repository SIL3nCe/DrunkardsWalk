#pragma once

#include "GridView.h"

class DrunkardsWalk
{
public:
	DrunkardsWalk(int GridWidth, int GridHeight);

	void Update(float dt);

	void Draw(sf::RenderWindow& window);

	void OnMouseClicked(int posX, int posY);

private:
	void DrawGUI();

	void ApplySmoothing();

private:
	GridView m_grid;

	int m_steps;
	int m_maxSteps;

	bool m_bAvoidVisitedCells;

	EDiagonalUse m_eDiagonalsUse;

	bool m_bSmoothing;
	bool m_bSmoothingDiagonals;
	int m_smoothingSteps;
	int m_smoothingTrigger;

	bool m_bPlay;
	float m_fStepCounter;
	float m_fStepDt;
};
