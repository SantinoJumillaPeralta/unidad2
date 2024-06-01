#include "Game.h"
#include <tchar.h>

using namespace sf;

class CajaControl
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
    CajaControl(b2World* _world, RenderWindow* _wnd)
    {
        // Guardamos puntero a ventana para dibujar luego
        wnd = _wnd;
        _image = new Texture();
        _image->loadFromFile("cubo.jpg"); // Cambiar nombre del archivo de imagen si es necesario
        // Cargamos el sprite
        _sprite = new Sprite(*_image);

        // Definimos el body y lo creamos
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(250.0f, 50.0f); // Cambiar la posición de la caja de control si es necesario
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
        fixtureDef.density = 1.0f; // Ajustar la densidad si es necesario
        fixtureDef.friction = 0.5f; // Ajustar la fricción si es necesario
        fixtureDef.restitution = 0.3f; // Ajustar la restitución si es necesario
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
