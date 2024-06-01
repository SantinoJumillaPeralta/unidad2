#include "Game.h"
#include <tchar.h>
#include <SFML/Graphics.hpp>
using namespace sf;

class Cubos
{
private:
	sf::RenderWindow* wnd;

public:
	Cubos(sf::RenderWindow* _wnd) : wnd(_wnd) {}

	void Dibujar()
	{
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
	}
};

class Pelota
{

private:
	// Sprite y su imagen la para representación gráfica
	Sprite* _sprite;
	Texture* _image;
	RenderWindow* wnd;
	// Body para representación física
	b2Body* _body;
	//...
public:
	Pelota(b2World* _world, RenderWindow* _wnd)
	{
		// Guardamos puntero a ventana para dibujar luego
		wnd = _wnd;
		_image = new Texture();
		_image->loadFromFile("pelota.jpg");
		// Cargamos el sprite
		_sprite = new Sprite(*_image);

		// Definimos el body y lo creamos
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(100.0f, 0.0f);
		_body = _world->CreateBody(&bodyDef);

		// Creamos su figura de colisión
		// en este caso suponemos que la figura de
		// colision es una caja cuadrada
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(20.0f, 20.0f);

		// Creamos el fixture, le seteamos
		// la figura de colision
		// y agregamos el fixture al body
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 10.0f;
		fixtureDef.friction = 0.3f;
		fixtureDef.restitution = 1.0f;
		_body->CreateFixture(&fixtureDef);
	}

	// Metodo que posiciona el sprites
	// segun la posicion del body
	void ActualizarPosiciones()
	{
		b2Vec2 pos = _body->GetPosition();
		_sprite->setPosition(pos.x, pos.y);
	}

	// Metodo que dibuja el sprite
	void Dibujar()
	{
		wnd->draw(*_sprite);
	}

	// Otros metodos
	// relacionados al personaje
};


int _tmain(int argc, _TCHAR* argv[])
{
	Game* Juego;
	Juego = new Game(800, 600, "Esqueleto de Aplicación - MAVII");
	Juego->Loop();

	return 0;
}