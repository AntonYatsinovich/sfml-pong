#include <SFML/Graphics.hpp>
#include <algorithm>

const sf::Vector2u RESOLUTION = {800, 600};
const sf::Vector2f RECTANGLE_SIZE = {60.f, 240.f};
const sf::Vector2f BALL_SIZE = {60.f, 60.f}; // пхаахаха, размер моего яйца

const float MAX_Y = static_cast<float>(RESOLUTION.y) - RECTANGLE_SIZE.y;
const sf::Vector2f SPEED = {0.f, 0.05f};

int main() {
  sf::RenderWindow window(sf::VideoMode(RESOLUTION), "SFML Pong",
                          sf::Style::Close);

  sf::RectangleShape player(RECTANGLE_SIZE);
  sf::RectangleShape enemy(RECTANGLE_SIZE);
  sf::RectangleShape ball(BALL_SIZE);

  player.setPosition({100.f, 100.f});
  enemy.setPosition({640.f, 100.f});
  ball.setPosition({370.f, 270.f});

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

		sf::Vector2f player_pos = player.getPosition();
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
    	player_pos.y -= SPEED.y;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
    	player_pos.y += SPEED.y;
    }

    player_pos.y = std::clamp(player_pos.y, 0.f, MAX_Y);
    player.setPosition(player_pos);
	   
    window.clear();

    window.draw(player);
    window.draw(enemy);
    window.draw(ball);

    window.display();
  }
}
