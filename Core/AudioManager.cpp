#include "AudioManager.h"

void PlayBackGroundMusic()
{
    sf::Music music;
    music.openFromFile("zombie_Background_Music.ogg");
    music.play();
}
void PlayGunShot()
{
    sf::SoundBuffer buffer;
    sf::Sound sound;
    buffer.loadFromFile("Gunshot.wav");
    sound.setBuffer(buffer);
    sound.play();
}
void PlayFootStep()
{
    sf::SoundBuffer buffer;
    sf::Sound sound;
    buffer.loadFromFile("Footsteps.wav");
    sound.setBuffer(buffer);
    sound.play();
}
void PlayScream()
{
    sf::SoundBuffer buffer;
    sf::Sound sound;
    buffer.loadFromFile("Scream.wav");
    sound.setBuffer(buffer);
    sound.play();
}