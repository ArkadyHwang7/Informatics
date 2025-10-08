#include <SFML/Graphics.hpp>
#include <functional>
#include <cmath>
#include <string>

void drawGraph(sf::RenderWindow& window, std::function<float(float)> func,
    float xMin, float xMax, float scaleX, float scaleY, sf::Color color) {
    sf::VertexArray graph(sf::LinesStrip);

    for (float x = xMin; x <= xMax; x += 0.1f) {
        float y = func(x);
        float screenX = 400 + x * scaleX;
        float screenY = 300 - y * scaleY;
        graph.append(sf::Vertex(sf::Vector2f(screenX, screenY), color));
    }

    window.draw(graph);
}

int getZone(float x, float y) {
    float absX = std::abs(x);
    float lineY = 5.0f; 

    if (y > absX && y > lineY)
        return 1;
    if (y < lineY && y > absX)
        return 2; 
    if (x > 0 && y > lineY && y < absX)
        return 3; 
    if (x < 0 && y > lineY && y < absX)
        return 4; 
    if (y < absX && y < lineY)
        return 5; 
    return 0;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "График y = |x| и y = 5");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1; 
    }

    sf::Text coordinatesText;
    coordinatesText.setFont(font);
    coordinatesText.setCharacterSize(20);
    coordinatesText.setFillColor(sf::Color::White);
    coordinatesText.setPosition(10, 10);

    sf::CircleShape userPoint(5);
    userPoint.setFillColor(sf::Color::Red);
    bool userPointExists = false;

    sf::VertexArray xAxis(sf::Lines, 2);
    xAxis[0].position = sf::Vector2f(50, 300);
    xAxis[1].position = sf::Vector2f(750, 300);
    xAxis[0].color = xAxis[1].color = sf::Color::White;

    sf::VertexArray yAxis(sf::Lines, 2);
    yAxis[0].position = sf::Vector2f(400, 50);
    yAxis[1].position = sf::Vector2f(400, 550);
    yAxis[0].color = yAxis[1].color = sf::Color::White;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {

                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                float mathX = (mousePos.x - 400) / 30.0f;
                float mathY = -(mousePos.y - 300) / 30.0f; 

                userPoint.setPosition(mousePos.x - userPoint.getRadius(),
                    mousePos.y - userPoint.getRadius());
                userPointExists = true;

                int zone = getZone(mathX, mathY);

                coordinatesText.setString(
                    "Coordinates: (" + std::to_string(mathX) + ", " +
                    std::to_string(mathY) + ") | Zone: " + std::to_string(zone));
            }
        }

        window.clear(sf::Color::Black);

        window.draw(xAxis);
        window.draw(yAxis);

        drawGraph(window, [](float x) { return std::abs(x); }, -10, 10, 30, 30, sf::Color::Red); 
        drawGraph(window, [](float x) { return 5.0f; }, -10, 10, 30, 30, sf::Color::Blue);       

        if (userPointExists) {
            window.draw(userPoint);
            window.draw(coordinatesText);
        }

        window.display();
    }

    return 0;
}