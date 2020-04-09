#include <SFML/Graphics.hpp>
#include <random>
#include <ctime>
#include <cmath>
std::mt19937_64 rng(std::time(0));
const int numberOfArrows = 11;
const int dogSpeed = 12;
const int arrowsSpeed = 10;
const int numberOfBombs = 12;
const int minScore = -3;
const unsigned int bombsDuration = 3; // in seconds
unsigned int bombsDigits = numberOfBombs ? (unsigned int)std::log(numberOfBombs) + 1 : 1;
sf::Vector2f generateRandomPosition(sf::Sprite const& sprite, sf::Vector2u const& windowDimensions){
    unsigned int width = sprite.getGlobalBounds().width;
    unsigned int height = sprite.getGlobalBounds().height;
    return sf::Vector2f(rng() % (windowDimensions.x - width), rng() % (windowDimensions.y - height));
}
bool isTouching(sf::Sprite const& a, sf::Sprite const& b) {
    return a.getPosition().x < b.getPosition().x + b.getGlobalBounds().width &&
           a.getPosition().x + a.getGlobalBounds().width > b.getPosition().x &&
           a.getPosition().y < b.getPosition().y + b.getGlobalBounds().height &&
           a.getPosition().y + a.getGlobalBounds().height > b.getPosition().y;
}
void avanceArrow(sf::Sprite &sprite, sf::Vector2u const& windowDimensions){
    sprite.move(-0.02*arrowsSpeed, 0);
    if(sprite.getPosition().x < 0){
        sprite.setPosition(generateRandomPosition(sprite, windowDimensions));
        sprite.setPosition(windowDimensions.x - 50, sprite.getPosition().y);
    }
}

void stop(sf::Window &window, sf::Text quitButton){
    sf::Event event;
    while(window.isOpen()){
        if(window.waitEvent(event)){
            if(event.type == sf::Event::Closed) window.close();
            else if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && // quit button is pressed
                   event.mouseButton.x < quitButton.getPosition().x + quitButton.getGlobalBounds().width &&
                   event.mouseButton.x > quitButton.getPosition().x &&
                   event.mouseButton.y < quitButton.getPosition().y + quitButton.getGlobalBounds().height &&
                   event.mouseButton.y > quitButton.getPosition().y) window.close();
            else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q) window.close();
        }
    }
}

int main(){
    sf::RenderWindow window;
    window.create( sf::VideoMode(1024, 600), "Dog play", sf::Style::Fullscreen);
    int score = 0, i, bombs = numberOfBombs;
    bool loose, isUsingABomb = false;
    sf::Font ubuntuFont;
    if(!ubuntuFont.loadFromFile("res/Ubuntu-Medium.ttf")){
        fprintf(stderr, "Error while loading Ubuntu-Medium Font\n");
        return EXIT_FAILURE;
    }
    sf::Text bombsDisplay("bombes:3", ubuntuFont, 40);
    bombsDisplay.setFillColor(sf::Color::Black);
    sf::Text scoreDisplay("score:00", ubuntuFont, 40);
    scoreDisplay.move(sf::Vector2f(bombsDisplay.getGlobalBounds().width + 10, 0));
    sf::Text quitButton("quit(q)", ubuntuFont, 40);
    quitButton.setFillColor(sf::Color::Red);
    quitButton.setPosition(scoreDisplay.getPosition());
    quitButton.move(scoreDisplay.getGlobalBounds().width + 10, 0);
    scoreDisplay.setFillColor(sf::Color::Black);
    sf::Texture dogTexture, ballTexture, arrowTexture;
    if(!dogTexture.loadFromFile("res/dog.png")){
        fprintf(stderr, "Error while loading Dog Image\n");
        return EXIT_FAILURE;
    }
    if(!ballTexture.loadFromFile("res/ball.png")){
        fprintf(stderr, "Error while loading Ball Image\n");
        return EXIT_FAILURE;
    }
    if(!arrowTexture.loadFromFile("res/arrow.png")){
        fprintf(stderr, "Error while loading Arrow Image\n");
        return EXIT_FAILURE;
    }
    sf::Sprite dog(dogTexture), ball(ballTexture);
    dog.setPosition(generateRandomPosition(dog, window.getSize()));
    ball.setPosition(generateRandomPosition(ball, window.getSize()));
    sf::Sprite arrows[numberOfArrows];
    for(i = 0;i < numberOfArrows;i++){
        arrows[i] = sf::Sprite(arrowTexture);
        arrows[i].setScale(0.08f, 0.07f);
        arrows[i].setPosition(generateRandomPosition(arrows[i], window.getSize()));
    }
    ball.setScale(0.08f, 0.08f);
    dog.setScale(0.8f, 0.8f);
    sf::Event event;
    sf::Clock chrono;
    while(window.isOpen()){
        while(window.pollEvent(event)){
            switch(event.type){
            case sf::Event::Closed:
                window.close();
                return 0;
                break;
            case sf::Event::KeyPressed:
                switch(event.key.code){
                case sf::Keyboard::Up:
                    if(dog.getPosition().y > 0) dog.move(sf::Vector2f(0, -dogSpeed));
                    break;
                case sf::Keyboard::Down:
                    if(dog.getPosition().y < window.getSize().y - dog.getGlobalBounds().height) dog.move(sf::Vector2f(0, dogSpeed));
                    break;
                case sf::Keyboard::Left:
                    if(dog.getPosition().x > 0) dog.move(sf::Vector2f(-dogSpeed, 0));
                    break;
                case sf::Keyboard::Right:
                    if(dog.getPosition().x < window.getSize().x - dog.getGlobalBounds().width) dog.move(sf::Vector2f(dogSpeed, 0));
                    break;
                case sf::Keyboard::Q:
                	window.close();
                	break;
                default:
                    break;
                }
                break;  
            case sf::Event::KeyReleased:
                if(event.key.code == sf::Keyboard::Space && bombs){
                    isUsingABomb = true;
                    bombs--;
                    chrono.restart();
                }
                break;
            case sf::Event::MouseButtonPressed:
                if(event.mouseButton.button == sf::Mouse::Left && // quit button is pressed
                   event.mouseButton.x < quitButton.getPosition().x + quitButton.getGlobalBounds().width &&
                   event.mouseButton.x > quitButton.getPosition().x &&
                   event.mouseButton.y < quitButton.getPosition().y + quitButton.getGlobalBounds().height &&
                   event.mouseButton.y > quitButton.getPosition().y) window.close();
                   break;
            default:
                break;
            }
        }
        if(!isUsingABomb){
            for(i = 0;i < numberOfArrows;i++){
                avanceArrow(arrows[i], window.getSize());
                if(isTouching(dog, arrows[i])){
                    score--;
                    dog.setPosition(generateRandomPosition(dog, window.getSize()));
                }
                if(isTouching(ball, arrows[i])){
                    ball.setPosition(generateRandomPosition(ball, window.getSize()));
                }
            }
        } else if(chrono.getElapsedTime().asSeconds() > bombsDuration) isUsingABomb = false;
        if(isTouching(dog, ball)){
            score++;
            ball.setPosition(generateRandomPosition(ball, window.getSize()));
        }
        window.clear(sf::Color::White);
        if(score < minScore){
            scoreDisplay.setString("Game Over");
            scoreDisplay.setCharacterSize(150);
            scoreDisplay.setPosition(
                window.getSize().x / 2 - scoreDisplay.getGlobalBounds().width / 2,
                window.getSize().y / 2 - scoreDisplay.getGlobalBounds().height / 2
            );
            loose = true;
        } else {
            char scoreString[score ? (unsigned int)std::log(score) + 1 : 1];
            sprintf(scoreString, "score:%d", score);
            scoreDisplay.setString(scoreString);
        }
        window.draw(dog);
        window.draw(ball);
        for(i = 0;i < numberOfArrows;i++){
            window.draw(arrows[i]);
        }
        char bombsString[bombsDigits + 7];
        sprintf(bombsString, "bombs:%d", bombs);
        bombsDisplay.setString(bombsString);
        window.draw(bombsDisplay);
        window.draw(scoreDisplay);
        window.draw(quitButton);
        window.display();
        if(loose) stop(window, quitButton);
    }
}
