#include <iostream>

#include <SFML/Audio.hpp>

#include "engine/temp/Hello.h"

int main(void)
{
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile("sound.wav"))
        return -1;

    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();

    std::cout << std::endl;

    ds::Hello();
    std::cin.get();

    return 0;
}
