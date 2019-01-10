#include "AudioManager.h"

sf::Music music;
void AudioManager::PlayBackGroundMusic()
{
    printf("Shit");
    music.openFromFile("zombie_Background_Music.ogg");
    music.setVolume(100.0f);
    music.play();
}
void AudioManager::PlayGunShot()
{
    sf::SoundBuffer buffer;
    sf::Sound sound;
    buffer.loadFromFile("Gunshot.wav");
    sound.setBuffer(buffer);
    sound.setVolume(100.0f);
    sound.play();
}
void AudioManager::PlayFootStep()
{
    sf::SoundBuffer buffer;
    sf::Sound sound;
    buffer.loadFromFile("Footsteps.wav");
    sound.setBuffer(buffer);
    sound.setVolume(100.0f);
    sound.play();
}
void AudioManager::PlayScream()
{
    sf::SoundBuffer buffer;
    sf::Sound sound;
    buffer.loadFromFile("Scream.wav");
    sound.setBuffer(buffer);
    sound.setVolume(100.0f);
    sound.play();
}