#include "DrunkardsWalk.h"

#include "imgui.h"
#include "imgui-SFML.h"

using namespace std;

DrunkardsWalk::DrunkardsWalk(int GridWidth, int GridHeight)
: m_grid(GridWidth, GridHeight)
, m_steps(0)
, m_maxSteps(5000)
, m_bAvoidVisitedCells(false)
, m_eDiagonalsUse(EDiagonalUse::no_diagonals)
, m_bSmoothing(false)
, m_bSmoothingDiagonals(true)
, m_smoothingSteps(2)
, m_smoothingTrigger(4)
, m_bPlay(false)
, m_fStepCounter(0.0f)
, m_fStepDt(0.0f)
{
}

void DrunkardsWalk::Update(float dt)
{
    if (m_bPlay)
    {
        m_fStepCounter += dt;
        if (m_fStepCounter >= m_fStepDt)
        {
            m_fStepCounter = 0.0f;
    
            m_grid.VisitRandomNeighbour(m_bAvoidVisitedCells, m_eDiagonalsUse);

            m_steps++;
            if (m_steps >= m_maxSteps)
            {
                m_bPlay = false;
                if (m_bSmoothing)
                {
                    ApplySmoothing();
                }
            }
        }
    }

    DrawGUI();
    m_grid.DrawGUI();
}

void DrunkardsWalk::Draw(sf::RenderWindow& window)
{
    m_grid.Draw(window);
}

void DrunkardsWalk::DrawGUI()
{
    if (ImGui::Begin("Drunkard's Walk"))
    {
        if (ImGui::Button("Start"))
        {
            m_bPlay = true;
            m_steps = 01;
            m_fStepCounter = 0.0f;
        }
        ImGui::SameLine();
        if (ImGui::Button("Pause"))
        {
            m_bPlay = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset"))
        {
            m_bPlay = false;
            m_steps = 0;
            m_grid.Reset();
        }

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::SliderInt("Max steps", &m_maxSteps, 1, 20000);
        ImGui::Text("Steps: %d/%d", m_steps, m_maxSteps);

        ImGui::Spacing();
        ImGui::SliderFloat("Speed", &m_fStepDt, 0.0f, 0.5f);

        ImGui::Spacing();
        ImGui::Checkbox("Avoid already visited cells", &m_bAvoidVisitedCells);

        ImGui::Spacing();
        if (ImGui::RadioButton("No diagonals", m_eDiagonalsUse == EDiagonalUse::no_diagonals))
            m_eDiagonalsUse = EDiagonalUse::no_diagonals;
        if (ImGui::RadioButton("With diagonals", m_eDiagonalsUse == EDiagonalUse::with_diagonals))
            m_eDiagonalsUse = EDiagonalUse::with_diagonals;
        if (ImGui::RadioButton("Only diagonals", m_eDiagonalsUse == EDiagonalUse::only_diagonals))
            m_eDiagonalsUse = EDiagonalUse::only_diagonals;

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Checkbox("Smoothing", &m_bSmoothing);
        ImGui::Checkbox("Diagonals", &m_bSmoothingDiagonals);
        ImGui::InputInt("Steps", &m_smoothingSteps);
        ImGui::InputInt("Trigger", &m_smoothingTrigger);
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 30.0f);
            ImGui::TextUnformatted("Any wall will be marked visited if it has at least Trigger of empty neigbours");
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
        if (ImGui::Button("Manually smooth"))
        {
            ApplySmoothing();
        }
    }
    ImGui::End();
}

void DrunkardsWalk::ApplySmoothing()
{
    for (int i = 0; i < m_smoothingSteps; ++i)
    {
        m_grid.ApplySmoothing(m_smoothingTrigger, m_bSmoothingDiagonals);
    }
}

void DrunkardsWalk::OnMouseClicked(int posX, int posY)
{
    if (!m_bPlay)
    {
        if (m_grid.OnMouseClicked(posX, posY))
        {
            m_bPlay = true;
            m_steps = 1;
            m_fStepCounter = 0.0f;
        }
    }
}
