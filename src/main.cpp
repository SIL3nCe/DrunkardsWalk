#include "imgui.h"
#include "imgui-SFML.h"

#include "DrunkardsWalk.h"

using namespace std;

#define GRID_WIDTH 1000
#define GRID_HEIGHT 800

/* TODO
    - Should be better with instant gen and animated draw instead of gen/draw one step/frame
*/

int main()
{
    sf::RenderWindow window(sf::VideoMode(GRID_WIDTH, GRID_HEIGHT), "Drunkard's Walk");

    ImGui::SFML::Init(window);
    ImGuiIO& io = ImGui::GetIO();

    DrunkardsWalk drunkardsWalk(GRID_WIDTH, GRID_HEIGHT);
    
    srand(time(nullptr));
    
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Time dtTime = clock.restart();
        float dt = dtTime.asSeconds();

        bool bMouseButtonPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left) && !io.WantCaptureMouse;

        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            switch (event.type)
            {
                case  sf::Event::MouseButtonPressed:
                {
                    if (bMouseButtonPressed)
                    {
                        drunkardsWalk.OnMouseClicked(event.mouseButton.x, event.mouseButton.y);
                    }
                }
                break;

                case sf::Event::Closed:
                {
                    window.close();
                }
                break;
            }
        }

        ImGui::SFML::Update(window, dtTime);
        
        drunkardsWalk.Update(dt);

        window.clear();
        drunkardsWalk.Draw(window);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
