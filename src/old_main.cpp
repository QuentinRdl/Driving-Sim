/*
#include <iostream>
#include <random>
#include <climits>
#include <unistd.h>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

/**
 * @brief Create a circle with a given radius, color and position
 * @param radius The radius of the circle
 * @param baseColor The base color of the circle
 * @param pos_x The position on the x axis
 * @param pos_y The position on the y axis
 * @return sf::CircleShape The circle created
 #1#
sf::CircleShape createCircle(float radius, sf::Color baseColor, float pos_x, float pos_y);

/**
 * @brief Generate a random color
 * @param gen The random number generator
 * @return sf::Color The generated color
 #1#
sf::Color generateRandomColor(std::mt19937 gen);

int main() {
    /**
     * @brief Création de la fenêtre
     * @param sf::VideoMode::getDesktopMode() Récupère la résolution de l'écran
     * @param "Interface Graphique" Titre de la fenêtre
     * @param sf::Style::Titlebar | sf::Style::Close Style de la fenêtre, interdit le redimensionnement
     *#1#

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Interface Graphique", sf::Style::Titlebar | sf::Style::Close);

    float baseRadius = 50.0f;
    sf::CircleShape circle = createCircle(baseRadius, sf::Color::Green, 375.0f, 275.0f);

    sf::Font font;
    /**
     * @brief Chargement de la police de caractères pour le texte. NB: Si aucune police n'est chargé, le texte ne s'affichera pas
     #1#
    if (!font.loadFromFile("./font.ttf")) std::cerr << "Erreur lors du chargement de la police " << std::endl;

    int character_size = 24;
    sf::Text text;
    text.setFont(font);
    text.setString("Simulateur de drift");
    text.setCharacterSize(character_size);
    text.setFillColor(sf::Color::White);

    std::random_device rd;
    std::mt19937 gen(rd());

    sf::Clock clock; // Pour calculer le delta time

    bool txt_sel = false;

    /**
     * @brief Boucle principale de jeu.
     * Each iteration is a frame.
     #1#
    while (window.isOpen()) {
        float speed = 500.0f;
        sf::Event event = {};
        /**
         * @brief Gestion des événements
         #1#
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::KeyPressed:
                    if (event.key.code != sf::Keyboard::Escape) break;
                case sf::Event::Closed:
                    window.close();
                    break;
                default: break;
            }
        }


        // Calculer le delta time
        float deltaTime = clock.restart().asSeconds();

        // Gestion des entrées utilisateur
        sf::Vector2f movement(0.0f, 0.0f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            movement.y -= speed * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            movement.y += speed * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            movement.x -= speed * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            movement.x += speed * deltaTime;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
            if (txt_sel) {
                text.setCharacterSize(++character_size);
                std::cout << "char size = " << character_size << std::endl;
            } else {
                baseRadius += 5;
                circle.setRadius(baseRadius);
            }

        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) {
            if (txt_sel) {
                if (character_size > 0) text.setCharacterSize(--character_size);
            } else {
                if (baseRadius >= 5) baseRadius -= 5;
                else baseRadius = 0;
                circle.setRadius(baseRadius);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
            txt_sel = !txt_sel;
            std::cout << "Changing selection to " << (txt_sel ? "text" : "circle") << std::endl;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
            if (txt_sel) {
                text.setFillColor(generateRandomColor(gen));
            } else {
                circle.setFillColor(generateRandomColor(gen));
            }
        }

        if (txt_sel) {
            text.move(movement);
        } else {
            circle.move(movement);
        }
        // Déplacer le cercle

        // Effacer l'écran
        window.clear(sf::Color(43, 45, 48));

        // Dessiner le cercle
        window.draw(circle);
        window.draw(text);

        // Afficher les modifications
        window.display();
    }

    return 0;
}

sf::CircleShape createCircle(const float radius, const sf::Color baseColor, const float pos_x, const float pos_y) {
    sf::CircleShape circle(radius);
    circle.setFillColor(baseColor);
    circle.setPosition(pos_x, pos_y);
    return circle;
}

sf::Color generateRandomColor(std::mt19937 gen) {
    std::uniform_int_distribution<> distrib_0_255(0, 255);
    const int r = distrib_0_255(gen), g = distrib_0_255(gen), b = distrib_0_255(gen);
    std::cout << "Generated color: r=" << r << ", g=" << g << ", b=" << b << std::endl;
    return {r, g, b};
}
*/
