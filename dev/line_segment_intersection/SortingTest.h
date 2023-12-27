#pragma once
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>

class SortingTest
{
public:
    sf::RenderWindow m_window;
    sf::Vector2f center;

    SortingTest()
        :m_window(sf::VideoMode(800, 600), "My window")
    {}

    void Run()
    {
        center = sf::Vector2f(250, 350);
        sf::CircleShape centerCircle;
        centerCircle.setPosition(center);
        centerCircle.setRadius(4);
        centerCircle.setOrigin(4,4);
        centerCircle.setOutlineThickness(1);
        centerCircle.setOutlineColor(sf::Color::Red);
        centerCircle.setFillColor(sf::Color::Transparent);

        std::vector<sf::Vector2f> points;
        auto p0 = sf::Vector2f(100, 400);
        auto p1 = sf::Vector2f(200, 300);
        auto p2 = sf::Vector2f(300, 300);
        auto p3 = sf::Vector2f(400, 400);
        auto p4 = sf::Vector2f(500, 500);
        auto p5 = sf::Vector2f(150, 150);
        points.push_back(p0);
        points.push_back(p1);
        points.push_back(p2);
        points.push_back(p3);
        points.push_back(p4);
        points.push_back(p5);

        SortPoints(points);
        std::cout << "///////////////" << std::endl;
        for (int j = 0; j < points.size(); ++j)
        {
            auto currentAngle = CalculateAngle(points[j], center);
            std::cout << currentAngle << std::endl;
        }

        sf::Font font;
        if (!font.loadFromFile("CotaneBeach.otf"))
        {
            // error...
        }
        std::vector<sf::Text> texts;
        std::vector<sf::CircleShape> circles;
        for (int i = 0; i < points.size(); i++)
        {
            sf::CircleShape c;
            c.setPosition(points[i]);
            c.setRadius(4);
            c.setOrigin(4,4);
            c.setOutlineThickness(1);
            c.setFillColor(sf::Color::Transparent);

            auto currentAngle = CalculateAngle(points[i], center);

            sf::Text text;
            text.setFont(font);
            text.setString(std::to_string(i));
            text.setCharacterSize(12);
            text.setPosition(points[i]);
            text.setColor(sf::Color::Red);
            texts.push_back(text);

            sf::Text textAngle;
            textAngle.setFont(font);
            textAngle.setString(std::to_string(currentAngle));
            textAngle.setCharacterSize(12);
            textAngle.setPosition(points[i].x, points[i].y - 20);
            textAngle.setColor(sf::Color::Red);
            texts.push_back(textAngle);

            circles.push_back(c);
        }


        sf::CircleShape circle;
        circle.setRadius(4);
        circle.setOrigin(4,4);
        circle.setOutlineThickness(1);
        circle.setFillColor(sf::Color::Transparent);
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(22);
        text.setFillColor(sf::Color::Red);

        while (m_window.isOpen())
        {
            SInput();

            auto mousePos = sf::Vector2f(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y);
            auto angle = CalculateAngle(mousePos, center);
            circle.setPosition(mousePos);
            text.setString(std::to_string(angle));
            text.setPosition(mousePos.x - 20, mousePos.y);


            m_window.clear(sf::Color(25, 35,25));

            for(auto& c : circles)
            {
                m_window.draw(c);
            }

            for(auto& t : texts)
            {
                m_window.draw(t);
            }

            m_window.draw(circle);
            m_window.draw(text);

            m_window.draw(centerCircle);

            m_window.display();
        }
    }
private:
    float CalculateAngle(const sf::Vector2f point, const sf::Vector2f center)
    {
        float dx = point.x - center.x;
        float dy = point.y - center.y;
        return std::atan2(dy, dx) * (180.0 / M_PI);
    }

    void SortPoints(std::vector<sf::Vector2f>& points)
    {
        for (int j = 0; j < points.size(); ++j)
        {
            int indexOfSmallest = 0;
            float lastAngle = 999;

            for (int i = j; i < points.size(); ++i)
            {
                auto currentAngle = CalculateAngle(points[i], center);

                if (currentAngle < lastAngle)
                {
                    lastAngle = currentAngle;
                    indexOfSmallest = i;
                }
            }

            sf::Vector2f tmp;
            tmp = points[j];
            points[j] = points[indexOfSmallest];
            points[indexOfSmallest] = tmp;
            std::cout << lastAngle << std::endl;
        }
    }

    void SInput()
    {
        sf::Event event {};
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                m_window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape)
                {
                    m_window.close();
                }
                if(event.key.code == sf::Keyboard::Escape)
                {
                    m_window.close();
                }
                if(event.key.code == sf::Keyboard::Space)
                {
                }
            }
            if (event.type == sf::Event::KeyReleased)
            {

            }
        }
    }
};