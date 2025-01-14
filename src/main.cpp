#include <iostream>
#include <random>
#include <climits>
#include <unistd.h>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

sf::CircleShape createCircle(float radius, sf::Color baseColor, float pos_x, float pos_y);

sf::Color generateRandomColor(std::mt19937 gen);

int main() {
    // Création de la fenêtre
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Interface Graphique", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(165);

    if (char cwd[PATH_MAX]; getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << cwd << std::endl;
    }


    float radius = 50.0f;

    sf::CircleShape circle = createCircle(radius, sf::Color::Green, 375.0f, 275.0f);

    sf::Font font;

    if (!font.loadFromFile("./font.ttf")) {
        std::cerr << "Erreur lors du chargement de la police " << std::endl;
    }

    int character_size = 24;
    sf::Text text;
    text.setFont(font);
    text.setString("Simulateur de drift");
    text.setCharacterSize(character_size);
    text.setFillColor(sf::Color::White);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib_50_50(-50, 50);


    // Variables pour le déplacement
    sf::Clock clock; // Pour calculer le delta time

    bool txt_sel = false;

    // Boucle principale
    while (window.isOpen()) {
        float speed = 500.0f;
        sf::Event event = {};
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
                std::cout << "charsize = " << character_size << std::endl;
            } else {
                radius += 5;
                circle.setRadius(radius);
            }

        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) {
            if (txt_sel) {
                if (character_size > 0) text.setCharacterSize(--character_size);
            } else {
                if (radius >= 5) radius -= 5;
                else radius = 0;
                circle.setRadius(radius);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
            txt_sel = !txt_sel;
            std::cout << "Changing selection to " << (txt_sel ? "text" : "circle") << std::endl;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
            circle.setFillColor(generateRandomColor(gen));
        }

        if (txt_sel) {
            text.move(movement);
        } else {
            circle.move(movement);
        }
        // Déplacer le cercle

        // Effacer l'écran
        window.clear(sf::Color::Black);

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
    int r = distrib_0_255(gen), g = distrib_0_255(gen), b = distrib_0_255(gen);
    sf::Color col(r, g, b);
    return col;
}
