#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>
#include <cmath>

#define DEG_TO_RAD(angle) ((angle) * b2_pi / 180.0f)

Game::Game(int ancho, int alto, std::string titulo)
{
    wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
    wnd->setVisible(true);
    fps = 60;
    wnd->setFramerateLimit(fps);
    frameTime = 1.0f / fps;
    SetZoom();
    InitPhysics();
}

void Game::Loop()
{
    while (wnd->isOpen())
    {
        wnd->clear(clearColor);
        DoEvents();
        CheckCollitions();
        UpdatePhysics();
        DrawGame();
        wnd->display();
    }
}

void Game::UpdatePhysics()
{
    phyWorld->Step(frameTime, 8, 8);
    phyWorld->ClearForces();
    phyWorld->DebugDraw();
}

void Game::DrawGame()
{
    // Dibujamos el suelo inclinado
    float angulo = 30.0f; // Ángulo del plano inclinado en grados
    float longitud = 200.0f; // Longitud del plano inclinado
    float altura = longitud * std::tan(DEG_TO_RAD(angulo)); // Calculamos la altura del plano inclinado
    sf::RectangleShape slopeShape(sf::Vector2f(longitud, 5));
    slopeShape.setFillColor(sf::Color::Green);
    slopeShape.setRotation(-angulo); // Rotamos la forma para simular el plano inclinado
    slopeShape.setPosition(50, 100 + altura); // Posicionamos el plano inclinado
    wnd->draw(slopeShape);

    // Dibujamos la caja
    sf::RectangleShape boxShape(sf::Vector2f(20, 20)); // Tamaño de la caja
    boxShape.setFillColor(sf::Color::Blue);
    boxShape.setPosition(boxBody->GetPosition().x - 10, boxBody->GetPosition().y - 10); // Posición de la caja
    wnd->draw(boxShape);

    // Dibujar texto en la esquina superior izquierda
    sf::Font font;
    if (!font.loadFromFile("font.ttf")) {
        // Si la fuente no se pudo cargar, muestra un mensaje en la consola
        return;
    }
    sf::Text text;
    text.setFont(font);
    text.setString("MAVII");
    text.setCharacterSize(24); // Tamaño del texto
    text.setFillColor(sf::Color::Red);
    text.setPosition(5, 5); // Posición en la esquina superior izquierda

    // Verificar si la posición del texto está dentro de la ventana
    if (text.getPosition().x < 0 || text.getPosition().y < 0 ||
        text.getPosition().x + text.getLocalBounds().width > wnd->getSize().x ||
        text.getPosition().y + text.getLocalBounds().height > wnd->getSize().y) {
        std::cerr << "La posición del texto está fuera de la ventana" << std::endl;
        return;
    }
    wnd->draw(text);
}

void Game::DoEvents()
{
    Event evt;
    while (wnd->pollEvent(evt))
    {
        switch (evt.type)
        {
        case Event::Closed:
            wnd->close();
            break;
        }
    }
}

void Game::CheckCollitions()
{
    // Veremos más adelante
}

// Definimos el área del mundo que veremos en nuestro juego
// Box2D tiene problemas para simular magnitudes muy grandes
void Game::SetZoom()
{
    View camara;
    // Posicion del view
    camara.setSize(100.0f, 100.0f);
    camara.setCenter(50.0f, 50.0f);
    wnd->setView(camara); //asignamos la camara
}

void Game::InitPhysics()
{
    // Inicializamos el mundo con la gravedad por defecto
    phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

    // Creamos el renderer de debug y le seteamos las banderas para que dibuje TODO
    debugRender = new SFMLRenderer(wnd);
    debugRender->SetFlags(UINT_MAX);
    phyWorld->SetDebugDraw(debugRender);

    // Creamos un plano inclinado
    b2Body* slopeBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, longitud, 10);
    slopeBody->SetTransform(b2Vec2(50.0f, 100.0f), DEG_TO_RAD(angulo));

    // Creamos una caja
    boxBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 20, 20, 1.0f);
    boxBody->SetTransform(b2Vec2(50.0f, 80.0f), 0.0f); // Posicionamos la caja en la parte superior del plano inclinado

    // Definimos el coeficiente de rozamiento para la caja
    b2Fixture* boxFixture = boxBody->GetFixtureList();
    b2FrictionJointDef frictionJointDef;
    frictionJointDef.bodyA = boxBody;
    frictionJointDef.bodyB = slopeBody;
    frictionJointDef.collideConnected = true;
    frictionJointDef.localAnchorA.Set(0.0f, 0.0f); // Punto de contacto en la caja
    frictionJointDef.localAnchorB.Set(0.0f, 0.0f); // Punto de contacto en el plano inclinado
    frictionJointDef.maxForce = 10.0f; // Fuerza máxima de fricción
    frictionJointDef.maxTorque = 0.1f; // Torque máximo
    phyWorld->CreateJoint(&frictionJointDef);
}

Game::~Game(void)
{ }
