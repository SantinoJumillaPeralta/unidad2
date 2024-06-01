#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

Game::Game(int ancho, int alto, std::string titulo) {
    wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
    wnd->setVisible(true);
    fps = 60;
    wnd->setFramerateLimit(fps);
    frameTime = 1.0f / fps;
    InitPhysics();
}

void Game::Loop() {
    while (wnd->isOpen()) {
        wnd->clear(clearColor);
        DoEvents();
        UpdatePhysics();
        DrawGame();
        wnd->display();
    }
}

void Game::UpdatePhysics() {
    // Step del mundo de física
    phyWorld->Step(frameTime, 8, 8);
    phyWorld->ClearForces();
}

void Game::DrawGame() {
    // Dibujar el suelo, las paredes y la caja de control
    wnd->draw(groundShape);
    wnd->draw(leftWallShape);
    wnd->draw(rightWallShape);
    wnd->draw(controlShape);
}

void Game::DoEvents() {
    Event evt;
    while (wnd->pollEvent(evt)) {
        switch (evt.type) {
        case Event::Closed:
            wnd->close();
            break;
        }
    }

    // Aplicar fuerza horizontal cuando se presionan las teclas izquierda/derecha
    controlBody->SetAwake(true);
    if (Keyboard::isKeyPressed(Keyboard::Left))
        controlBody->ApplyForceToCenter(b2Vec2(-50.0f, 0.0f), true);
    if (Keyboard::isKeyPressed(Keyboard::Right))
        controlBody->ApplyForceToCenter(b2Vec2(50.0f, 0.0f), true);
}

void Game::InitPhysics() {
    // Inicializar el mundo de física
    phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

    // Crear el suelo
    b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 500, 5);
    groundBody->SetTransform(b2Vec2(250.0f, 95.0f), 0.0f);
    groundShape.setSize(sf::Vector2f(500, 5));
    groundShape.setFillColor(sf::Color::Red);
    groundShape.setPosition(0, 95);

    // Crear las paredes
    b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 200);
    leftWallBody->SetTransform(b2Vec2(0.0f, 100.0f), 0.0f);
    leftWallShape.setSize(sf::Vector2f(10, 200));
    leftWallShape.setFillColor(sf::Color::Blue);
    leftWallShape.setPosition(0, 0);

    b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 200);
    rightWallBody->SetTransform(b2Vec2(500.0f, 100.0f), 0.0f);
    rightWallShape.setSize(sf::Vector2f(10, 200));
    rightWallShape.setFillColor(sf::Color::Cyan);
    rightWallShape.setPosition(490, 0);

    // Crear la caja de control
    controlBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 20, 20, 1.0f, 0.5f, 0.3f);
    controlBody->SetTransform(b2Vec2(250.0f, 50.0f), 0.0f);
    controlShape.setSize(sf::Vector2f(20, 20));
    controlShape.setFillColor(sf::Color::Magenta);
}

Game::~Game() {
}
