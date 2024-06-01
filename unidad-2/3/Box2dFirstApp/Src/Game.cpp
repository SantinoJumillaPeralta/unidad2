#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

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
	// Dibujamos el suelo
	sf::RectangleShape groundShape(sf::Vector2f(500, 5));
	groundShape.setFillColor(sf::Color::Red);
	groundShape.setPosition(0, 95);
	wnd->draw(groundShape);

	// Dibujamos los cubos
	// Cubo izquierdo (más grande)
	sf::RectangleShape leftCubeShape(sf::Vector2f(30, 30));
	leftCubeShape.setFillColor(sf::Color::Green);
	leftCubeShape.setPosition(20, 50);
	wnd->draw(leftCubeShape);

	// Cubo central
	sf::RectangleShape centerCubeShape(sf::Vector2f(20, 20));
	centerCubeShape.setFillColor(sf::Color::Yellow);
	centerCubeShape.setPosition(50, 40);
	wnd->draw(centerCubeShape);

	// Cubo derecho arriba
	sf::RectangleShape rightUpCubeShape(sf::Vector2f(15, 15));
	rightUpCubeShape.setFillColor(sf::Color::Blue);
	rightUpCubeShape.setPosition(80, 30);
	wnd->draw(rightUpCubeShape);

	// Cubo derecho abajo
	sf::RectangleShape rightDownCubeShape(sf::Vector2f(25, 25));
	rightDownCubeShape.setFillColor(sf::Color::Magenta);
	rightDownCubeShape.setPosition(110, 60);
	wnd->draw(rightDownCubeShape);

	// Cubo derecho medio
	sf::RectangleShape rightMiddleCubeShape(sf::Vector2f(25, 25));
	rightMiddleCubeShape.setFillColor(sf::Color::Cyan);
	rightMiddleCubeShape.setPosition(110, 40);
	wnd->draw(rightMiddleCubeShape);

	// Dibujamos las paredes
	sf::RectangleShape leftWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
	leftWallShape.setFillColor(sf::Color::Blue);
	leftWallShape.setPosition(100, 0); // X = 100 para que comience donde termina el suelo
	wnd->draw(leftWallShape);

	sf::RectangleShape rightWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
	rightWallShape.setFillColor(sf::Color::Cyan);
	rightWallShape.setPosition(90, 0); // X = 90 para que comience donde termina el suelo
	wnd->draw(rightWallShape);

	// Dibujamos el cuerpo de control (círculo)
	sf::CircleShape controlShape(5);
	controlShape.setFillColor(sf::Color::Magenta);
	controlShape.setPosition(controlBody->GetPosition().x - 5, controlBody->GetPosition().y - 5);
	wnd->draw(controlShape);

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
		case Event::MouseButtonPressed:
			b2Body* body = Box2DHelper::CreateTriangularDynamicBody(phyWorld, b2Vec2(0.0f, 0.0f), 10.0f, 1.0f, 4.0f, 0.1f);
			// Transformamos las coordenadas segun la vista activa
			Vector2f pos = wnd->mapPixelToCoords(Vector2i(evt.mouseButton.x, evt.mouseButton.y));
			body->SetTransform(b2Vec2(pos.x, pos.y), 0.0f);
			break;
		}
	}

	// Movemos el cuerpo
	controlBody->SetAwake(true);
	if (Keyboard::isKeyPressed(Keyboard::Left))
		controlBody->SetLinearVelocity(b2Vec2(-50.0f, 0.0f));
	if (Keyboard::isKeyPressed(Keyboard::Right))
		controlBody->SetLinearVelocity(b2Vec2(50.0f, 0.0f));
	if (Keyboard::isKeyPressed(Keyboard::Down))
		controlBody->SetLinearVelocity(b2Vec2(0.0f, 50.0f));
	if (Keyboard::isKeyPressed(Keyboard::Up))
		controlBody->SetLinearVelocity(b2Vec2(0.0f, -50.0f));
}

void Game::CheckCollitions()
{
	// Veremos mas adelante
}

// Definimos el area del mundo que veremos en nuestro juego
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

	// Creamos un material para los cubos
	b2FixtureDef cubeFixtureDef;
	b2PolygonShape cubeShape;
	cubeFixtureDef.shape = &cubeShape;
	cubeFixtureDef.density = 1.0f; // Densidad de los cubos
	cubeFixtureDef.restitution = 0.5f; // Restitución (elasticidad) de los cubos

	// Asignamos el material a los cubos
	b2Material cubeMaterial;
	cubeMaterial.density = 1.0f;
	cubeMaterial.restitution = 0.5f;

	// Creamos los cubos
	b2Body* leftCubeBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 30, 30);
	leftCubeBody->SetTransform(b2Vec2(20.0f, 50.0f), 0.0f);
	leftCubeBody->CreateFixture(&cubeFixtureDef); // Agregamos el fixture al cuerpo

	b2Body* centerCubeBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 20, 20);
	centerCubeBody->SetTransform(b2Vec2(50.0f, 40.0f), 0.0f);
	centerCubeBody->CreateFixture(&cubeFixtureDef); // Agregamos el fixture al cuerpo

	// Cubo derecho arriba
	b2Body* rightUpCubeBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 15, 15);
	rightUpCubeBody->SetTransform(b2Vec2(80.0f, 30.0f), 0.0f);
	rightUpCubeBody->CreateFixture(&cubeFixtureDef); // Agregamos el fixture al cuerpo

	// Cubo derecho abajo
	b2Body* rightDownCubeBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 25, 25);
	rightDownCubeBody->SetTransform(b2Vec2(110.0f, 60.0f), 0.0f);
	rightDownCubeBody->CreateFixture(&cubeFixtureDef); // Agregamos el fixture al cuerpo

	// Cubo derecho medio
	b2Body* rightMiddleCubeBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 25, 25);
	rightMiddleCubeBody->SetTransform(b2Vec2(110.0f, 40.0f), 0.0f);
	rightMiddleCubeBody->CreateFixture(&cubeFixtureDef); // Agregamos el fixture al cuerpo


	// Creamos un piso y paredes
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, alto);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, alto);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);


	// Creamos un círculo que controlaremos con el teclado
	controlBody = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.5, 1.0f);
	controlBody->SetTransform(b2Vec2(50.0f, 50.0f), 0.0f);
}

Game::~Game(void)
{ }