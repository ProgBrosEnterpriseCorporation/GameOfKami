#define DefaultVideoWidth 1920
#define DefaultVideoHeight 1080

class Background : public sf::Drawable
{
private:
    sf::Sprite sprite;
    sf::Texture texture;
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(sprite, states);
    }

public:
    Background(const char *Filename, const sf::Vector2f resize_vector)
    {
        texture.loadFromFile(Filename);
        texture.setSmooth(true);
//        texture.setRepeated(true);
        sprite.setTexture(texture);
        sprite.setScale(resize_vector);
    }
};
