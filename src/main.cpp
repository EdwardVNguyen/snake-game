#include <SFML/Graphics.hpp>
#include <cstdlib>

constexpr uint8_t MAP_WIDTH{120};
constexpr uint8_t MAP_HEIGHT{120};

// checks if map border was hit or snake head hits its body
bool checkCollision(sf::Sprite* head, std::vector<sf::Sprite*>& snake);

// randomly sets new apple position after snake eats apple
void newApplePosition(sf::Sprite& appleSprite);

int main()
{
  sf::RenderWindow window(sf::VideoMode({120,120}), "Snake Game");

  // snake entity 
  sf::Texture snakeTex("images/snake.png");
  sf::Sprite* snakeHead = new sf::Sprite(snakeTex, sf::IntRect( {0,0}, {8,8}));

  std::vector<sf::Sprite*> snakeList;
  snakeList.push_back(snakeHead);

  // apple sprite
  sf::Texture appleTex("images/apple.png");
  sf::Sprite apple(appleTex);
  newApplePosition(apple);

  // Game Over pop-up screen
  sf::RectangleShape gameOverRect( { static_cast<float>(MAP_WIDTH), static_cast<float>(MAP_HEIGHT)} );
  gameOverRect.setFillColor(sf::Color::Black);

  sf::Font font("images/gameOverFont.ttf");
  sf::Text gameOverText(font);
  gameOverText.setString("Game Over");
  gameOverText.setFillColor(sf::Color::Red);
  gameOverText.setCharacterSize(16);
  gameOverText.setPosition( {MAP_WIDTH / 4.f, MAP_HEIGHT / 4.f} );
  
  // determines movement w.r.t direction 
  float dx{1.f}, dy{0.f};

  // determines head's rotation w.r.t movement
  enum Rotation { up, right, down, left };
  enum Rotation rotate{ right };
  
  sf::Clock clock;

  while (window.isOpen())
  {
    while (const std::optional event = window.pollEvent())
    {
      if (event->is<sf::Event::Closed>())
        window.close();
      if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
      {
        switch (keyPressed->scancode)
        {
        case sf::Keyboard::Scan::Right:
          dx = 1.f;
          dy = 0.f;
          rotate = right;
          break;
        case sf::Keyboard::Scan::Left:
          dx = -1.f;
          dy =  0.f;
          rotate = left;
          break;
        case sf::Keyboard::Scan::Down:
          dx = 0.f;
          dy = 1.f;
          rotate = down;
          break;
        case sf::Keyboard::Scan::Up:
          dx =  0.f;
          dy = -1.f;
          rotate = up;
          break;
        default:
          break;
        }
      }
    }

    // rotate head
    switch(rotate)
    {
    case up:
      snakeHead->setRotation(sf::degrees(0));
      break;
    case right:
      snakeHead->setRotation(sf::degrees(90));
      break;
    case down:
      snakeHead->setRotation(sf::degrees(180));
      break;
    case left:
      snakeHead->setRotation(sf::degrees(270));
      break;
    }

    // Move until border or snake body collision occurs
    if (clock.getElapsedTime().asSeconds() >= 0.3f && !checkCollision(snakeHead, snakeList))
    {
      // set full snake body's position (excluding head)
      for (int i = snakeList.size() - 1; i > 0; --i)
      {
        snakeList.at(i)->setPosition(snakeList.at(i-1)->getPosition()); 
      }

      snakeHead->move({8.f * dx,8.f * dy});
      clock.restart();

      // apple collision: randomize new apple loc, snake gets longer
      sf::FloatRect snakeBoundingBox = snakeHead->getGlobalBounds();
      sf::FloatRect appleBoundingBox = apple.getGlobalBounds();

      if (const std::optional intersection = snakeBoundingBox.findIntersection(appleBoundingBox))
      {
        newApplePosition(apple);

        sf::Sprite* snakeBody = new sf::Sprite(snakeTex, sf::IntRect( {8,0}, {8,8} )); 
        snakeList.push_back(snakeBody);
     }
    }
    
    

    /////////////////////
    //// Draw window ////
    window.clear(sf::Color::Black);

    window.draw(apple);

    // Game over Screen
    if (checkCollision(snakeHead, snakeList))
    {
      window.draw(gameOverRect);
      window.draw(gameOverText);
    }
    
    // draw snake 
    for (int i = 0; i < snakeList.size(); ++i)
    {
      if (i == 0)
      {
        window.draw(*snakeHead);
      }
      else
      {
        window.draw(*snakeList.at(i));
      }
    }

    window.display();
  }
  return 0;
}

bool checkCollision(sf::Sprite* head, std::vector<sf::Sprite*>& snake)
{
  if (head->getPosition().x > MAP_WIDTH || head->getPosition().x < 0 || head->getPosition().y > MAP_HEIGHT || head->getPosition().y < 0)
  {
    return true;
  }
  for (int i = 1; i < snake.size(); ++i)
  {
    if (head->getPosition() == snake.at(i)->getPosition())
    {
      return true;
    }
  }
  return false;
}

void newApplePosition(sf::Sprite& appleSprite)
{
  srand(time(0));
  int newPosX{ rand() % (MAP_WIDTH / 8) };
  int newPosY{ rand() % (MAP_HEIGHT / 8) };

  appleSprite.setPosition({newPosX * 8.f,newPosY* 8.f});
}

