#include <iostream>
#include "SFML/Graphics.hpp"
#include <fstream>
#include <sstream>

void TryLoadFont(const std::string&& fontName, sf::Font& font);

class MovingEntity
{
public:
    std::unique_ptr<sf::Shape> m_shape;
    std::unique_ptr<sf::Text> m_text;
    sf::Vector2f m_velocity;

    MovingEntity(std::unique_ptr<sf::Shape> shape, std::unique_ptr<sf::Text> text, sf::Vector2f velocity)
            : m_shape(std::move(shape)), m_text(std::move(text)), m_velocity(velocity)
    {
        m_text->setOrigin(m_text->getGlobalBounds().getSize() / 2.f + m_text->getLocalBounds().getPosition());
    }


    // Move Constructor
    MovingEntity(MovingEntity&& other) noexcept
            : m_shape(std::move(other.m_shape)), m_text(std::move(other.m_text)), m_velocity(other.m_velocity)
    {
    }

    void Tick(sf::RenderWindow& window)
    {
        Move();
        window.draw(*m_shape);
        window.draw(*m_text);
    }

    void Move()
    {
        if (m_shape->getPosition().x + m_shape->getGlobalBounds().width > 800 || m_shape->getPosition().x < 0)
            m_velocity.x = m_velocity.x * -1;
        if(m_shape->getPosition().y + m_shape->getGlobalBounds().height > 600 || m_shape->getPosition().y < 0)
            m_velocity.y = m_velocity.y * -1;
        m_shape->setPosition(m_shape->getPosition().x + m_velocity.x, m_shape->getPosition().y + m_velocity.y);
        CenterTextInsideShape();
    }

    void CenterTextInsideShape()
    {
        float x = m_shape->getPosition().x + m_shape->getGlobalBounds().width / 2;
        float y = m_shape->getPosition().y + m_shape->getGlobalBounds().height / 2;
        m_text->setPosition(x, y);
    }
};

int main()
{
    std::vector<MovingEntity> movingEntities;
    sf::Font font;
    TryLoadFont("CotaneBeach.otf", font);


    std::string str;
    std::ifstream inputFile;
    inputFile.open("../Shapes.txt");
    if (inputFile.is_open())
    {
        while (std::getline(inputFile, str))
        {
            std::istringstream iss(str);
            std::vector<std::string> tokens;

            std::string token;
            while (iss >> token)
            {
                tokens.push_back(token);
            }

            if (tokens[0] == "Circle")
            {
                std::unique_ptr<sf::CircleShape> circ = std::make_unique<sf::CircleShape>();
                std::unique_ptr<sf::Text> text = std::make_unique<sf::Text>(tokens[1], font, 18);
                circ->setPosition(std::stof(tokens[2]), std::stof(tokens[3]));
                circ->setRadius(std::stof(tokens[9]));
                circ->setFillColor(sf::Color(std::stoi(tokens[6]), std::stoi(tokens[7]), std::stoi(tokens[8])));

                MovingEntity entity(std::move(circ),
                                    std::move(text),
                                    sf::Vector2f(std::stof(tokens[4]),std::stof(tokens[4])));

                movingEntities.push_back(std::move(entity));
            }
            else if (tokens[0] == "Rectangle")
            {
                std::unique_ptr<sf::RectangleShape> rect = std::make_unique<sf::RectangleShape>();
                std::unique_ptr<sf::Text> text = std::make_unique<sf::Text>(tokens[1], font, 18);
                rect->setPosition(std::stof(tokens[2]), std::stof(tokens[3]));
                rect->setSize(sf::Vector2f(std::stof(tokens[9]), std::stof(tokens[10])));
                rect->setFillColor(sf::Color(std::stoi(tokens[6]), std::stoi(tokens[7]), std::stoi(tokens[8])));

                MovingEntity entity(std::move(rect),
                                    std::move(text),
                                    sf::Vector2f(std::stof(tokens[4]),std::stof(tokens[4])));

                movingEntities.push_back(std::move(entity));
            }
        }

        inputFile.close();
    }

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Application");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event event{};

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        for (auto& entity:movingEntities)
        {
            entity.Tick(window);
        }
        window.display();
    }
}

void TryLoadFont(const std::string&& fontName, sf::Font& font)
{
    if(!font.loadFromFile(fontName))
    {
        std::cout << "couldn't load the font" << std::endl;
        exit(-1);
    }
}

