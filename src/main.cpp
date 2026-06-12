#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath> 
#include <string>

const sf::Vector2u RESOLUTION = {800, 600};
const sf::Vector2f RECTANGLE_SIZE = {40.f, 180.f};
const sf::Vector2f BALL_SIZE = {40.f, 40.f};

// Ограниченная высота для игрока и для мяча соответственно
const float MAX_Y = static_cast<float>(RESOLUTION.y) - RECTANGLE_SIZE.y;
const float MAX_BALL_Y = static_cast<float>(RESOLUTION.y) - BALL_SIZE.y; 

// Скорости адаптированы под 60 FPS
const sf::Vector2f SPEED = {0.f, 6.f};
const float BLIND_ZONE = 30.f;
const sf::Vector2f BALL_START_POS(380.f, 280.f);

int main() {
  unsigned int player_score = 0;
  unsigned int enemy_score = 0;
  
  sf::RenderWindow window(sf::VideoMode(RESOLUTION), "SFML Pong", sf::Style::Close);
  
  // Ограничиваем FPS, чтобы игра у всех работала с одинаковой скоростью
  window.setFramerateLimit(60);

  sf::Font font("assets/fonts/Arial.ttf");
  std::string score_text = "Player: " + std::to_string(player_score) +  " | Enemy: " + std::to_string(enemy_score);
  sf::Text scores(font, score_text);
  
  // Центрируем текст по горизонтали
  scores.setOrigin({scores.getLocalBounds().size.x / 2.f, 0.f});
  scores.setPosition({400.f, 10.f});

  sf::RectangleShape player(RECTANGLE_SIZE); player.setPosition({100.f, 100.f}); 
  sf::RectangleShape enemy(RECTANGLE_SIZE); enemy.setPosition({640.f, 100.f});
  sf::RectangleShape ball(BALL_SIZE); ball.setPosition({370.f, 270.f});

  // Скорости мяча и бота увеличены для 60 FPS
  sf::Vector2f ball_velocity = {-5.f, 4.f}; 
  sf::Vector2f enemy_velocity = {0.f, 5.f};
  
  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    // === === === === Логика === === === ===
    
    // Переменные позиции и центров объектов
    sf::Vector2f player_pos = player.getPosition();
    sf::Vector2f ball_pos = ball.getPosition();
    sf::Vector2f ball_centre = ball_pos + sf::Vector2f(20.f, 20.f);
    sf::Vector2f enemy_pos = enemy.getPosition();
    sf::Vector2f enemy_centre = enemy_pos + sf::Vector2f(20.f, 90.f);
    

    // --- Логика Игрока
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
      player_pos.y -= SPEED.y;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
      player_pos.y += SPEED.y;
    }

    player_pos.y = std::clamp(player_pos.y, 0.f, MAX_Y);
    player.setPosition(player_pos);


    // --- Логика мяча

    ball_pos += ball_velocity;
    ball.setPosition(ball_pos);

    // Коллизии с ракетками
    if (ball.getGlobalBounds().findIntersection(enemy.getGlobalBounds())) {
      ball_velocity.x = -std::abs(ball_velocity.x);
      ball.setPosition({enemy.getPosition().x - BALL_SIZE.x, ball.getPosition().y});
    }

    if (ball.getGlobalBounds().findIntersection(player.getGlobalBounds())) {
      ball_velocity.x = std::abs(ball_velocity.x);
      ball.setPosition({player.getPosition().x + RECTANGLE_SIZE.x, ball.getPosition().y});
    }

    // Коллизии с верхним и нижним краем экрана
    if (ball.getPosition().y <= 0.f) {
      ball_velocity.y = std::abs(ball_velocity.y);
    } 
    if (ball.getPosition().y >= MAX_BALL_Y) {
      ball_velocity.y = -std::abs(ball_velocity.y);
    } 

    // ГОООЛЛ
    bool score_changed = false;
    if (ball_pos.x >= static_cast<float>(RESOLUTION.x)) { // Забил Игрок (мяч улетел на сторону врага)
      ball.setPosition(BALL_START_POS);
      player_score++; 
      score_changed = true;
      
      // Делаем скорость отрицательной, чтобы мяч полетел влево (к Игроку)
      ball_velocity.x = -std::abs(ball_velocity.x);
    }
    
    if (ball_pos.x <= 0.f) { // Забил Противник (мяч улетел на сторону игрока)
      ball.setPosition(BALL_START_POS);
      enemy_score++;   
      score_changed = true;
      
      // Делаем скорость положительной, чтобы мяч полетел вправо (к Противнику)
      ball_velocity.x = std::abs(ball_velocity.x);
    }

    if (score_changed) {
      score_text = "Player: " + std::to_string(player_score) +  " | Enemy: " + std::to_string(enemy_score);
      scores.setString(score_text);
      // Пересчитываем центр текста, так как количество цифр могло измениться
      scores.setOrigin({scores.getLocalBounds().size.x / 2.f, 0.f});
    }

    // --- Логика противника

    if (ball_pos.x >= 380.f) {
      if (enemy_centre.y > ball_centre.y + BLIND_ZONE) { // Если мячик сверху
        enemy_pos -= enemy_velocity;
      }
      if (enemy_centre.y < ball_centre.y - BLIND_ZONE) { // Если мячик снизу
        enemy_pos += enemy_velocity;
      }
      
      // Защищаем бота от вылета за границы экрана
      enemy_pos.y = std::clamp(enemy_pos.y, 0.f, MAX_Y);
      enemy.setPosition(enemy_pos);
    }
  
    
    // === === === === Графика === === === ===

    window.clear();
    window.draw(player);
    window.draw(enemy);
    window.draw(ball);
    window.draw(scores);
    window.display();
  }
}
