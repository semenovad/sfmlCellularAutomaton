#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

#define TILE_SIZE 10

using namespace sf;

int main()
{
    Clock clock;
    float time;
    int neighbours;
    float interval = 0.1;

    std::cout << "Hello, this is cellular automaton. My rules are very simple: \n1) Click the left mouse button into the window to place alive cell.\n2) Press space to see the next iteration. ";
    std::cout << "\nBut first of all enter the main characteristics - height and width of the window, rules of the game (cell to born, cell to survive).";

    int width, height;
    std::cout << "\n\nEnter width and height of the field: ";
    std::cin >> width >> height;

    std::vector<std::vector<bool>> world(width, std::vector<bool>(height, 0));
    std::vector<std::vector<bool>> nextStep(width, std::vector<bool>(height, 0));

    std::string rule;
    std::vector<bool> ruleB(9, 0), ruleS(9, 0);
    std::cout << "\nEnter rules for the game.\nYour input should look like \"B3S23\".\nAfter 'B' you should write how many cells should be around one cell to born new one.\nAfter 'S' you should write how many cells should be around one cell to survive.";
    std::cout << "\nInteresting variants are: B3S012345678, B5678S45678, B1S012345678.";
    std::cout << "\nEnter: ";
    std::cin >> rule;

    int i = 1;
    int size = rule.size();
    while (rule[i] != 'S') {
        ruleB[rule[i] - '0'] = true;
        ++i;
    }
    ++i;
    while (i < size) {
        ruleS[rule[i] - '0'] = true;
        ++i;
    }

    RectangleShape alive(Vector2f(TILE_SIZE, TILE_SIZE)); 
    RenderWindow window(VideoMode(width * TILE_SIZE, height * TILE_SIZE), "CellularAutomaton");
    alive.setFillColor(Color::Black);

    while (window.isOpen()) {
        time = clock.getElapsedTime().asSeconds();

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
        }

        Vector2i pixelPos = Mouse::getPosition(window); 
        Vector2f pos = window.mapPixelToCoords(pixelPos);
       
        if (Mouse::isButtonPressed(Mouse::Left)) { 
            int x = int(pos.x / TILE_SIZE);
            int y = int(pos.y / TILE_SIZE);
            if (x < width && x >= 0 && y >= 0 && y < height) {
                world[x][y] = true;
                nextStep[x][y] = true;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Space) && interval <= time) {
            clock.restart();
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) { 
                    neighbours = world[(i + width - 1) % width][j] + 
                        world[(i + width + 1) % width][j] + 
                        world[i][(j + height - 1) % height] + world[i][(j + height + 1) % height] + 
                        world[(i + width - 1) % width][(j + height - 1) % height] + 
                        world[(i + width + 1) % width][(j + height - 1) % height] + 
                        world[(i + width - 1) % width][(j + height + 1) % height] + 
                        world[(i + width + 1) % width][(j + height + 1) % height];
                    if (world[i][j] == true) {
                        if (!ruleS[neighbours]) nextStep[i][j] = false;
                    } else {
                        if (ruleB[neighbours]) nextStep[i][j] = true;
                    }
                }
            }
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                    world[i][j] = nextStep[i][j];
                }
            }
        }
        window.clear(Color::White); 

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                if (world[i][j] == true) {
                    alive.setPosition(Vector2f(i * TILE_SIZE, j * TILE_SIZE));
                    window.draw(alive); 
                }
            }
        }
        window.display();
    }
    return 0;
}