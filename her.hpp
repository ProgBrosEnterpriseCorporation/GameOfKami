#define DefaultVideoWidth 1920
#define DefaultVideoHeight 1080
#define DefaultHeroXPos 0
#define DefaultHeroYPos 1080
#define DefaultEnemyXPos 2359
#define DefaultEnemyYPos 1080
#define TalkTime 1
#define v_ 400

class Hero : public sf::Drawable
{
private:
    sf::Vector2f pos;
    sf::Sprite sprite;
    sf::Texture texture;
    float velocity;
    int ActNum = 0;
    float timeDifference = 0;
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(sprite, states);
    }

public:

    Hero(const char *Filename, const sf::Vector2f resize_vector)
//        : lines(sf::TriangleStrip, 7)
    {
        texture.loadFromFile(Filename);
        texture.setSmooth(true);
        sprite.setTexture(texture);
        sprite.setScale(resize_vector);
        sprite.setOrigin(0, texture.getSize().y);
	pos.x = DefaultHeroXPos*resize_vector.x;
	pos.y = DefaultHeroYPos*resize_vector.y;
        sprite.setPosition(pos);


    }
    void doPhysics(float time);
    void sendNetPos(sf::TcpSocket &socket);
    void getNetPos(sf::TcpSocket &socket);

    };

void Hero::doPhysics (float time)
{
	if (sf::Keyboard::isKeyPressed (sf::Keyboard::Right)) 
	{
		pos.x += time * v_;
		if (pos.x > DefaultVideoWidth)
			pos.x = DefaultVideoWidth;
	}
	if (sf::Keyboard::isKeyPressed (sf::Keyboard::Left)) 
	{
		pos.x -= time * v_;
		if (pos.x < 0)
			pos.x = 0;
	}
	if (sf::Keyboard::isKeyPressed (sf::Keyboard::Up)) 
	{
		pos.y += time * v_;
		if (pos.y > DefaultVideoHeight)
			pos.y = DefaultVideoHeight;
	}
	if (sf::Keyboard::isKeyPressed (sf::Keyboard::Down)) 
	{
		pos.y -= time * v_;
		if (pos.y < 0)
			pos.y = 0;
	}

	sprite.setPosition(pos);
	return;
}

void Hero::sendNetPos(sf::TcpSocket &socket)
{
	SData B;
	B.NewX = pos.x; 
	B.NewY = pos.y;
	if(_SendData(socket, &B, 1) == false)
		return;
	return;
}

void Hero::getNetPos(sf::TcpSocket &socket)
{
	SData *B;
	int size;
	if((B = _RecData(socket, size)) == 0)
		return;
	pos.x = B -> NewX;
	pos.y = B -> NewY;
	sprite.setPosition(pos);
	delete [] B;
	return;
}
