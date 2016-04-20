#include <iostream>

#include <SFML/Audio.hpp>

int main(void)
{
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile("../assets/sound.wav"))
        return -1;

    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();

    char a;
    std::cin >> &a;

    return 0;
}
