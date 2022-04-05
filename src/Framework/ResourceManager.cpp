/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Framework/ResourceManager.hpp>

namespace TGE
{
    ResourceManager* ResourceManager::instance = 0;

    ResourceManager::ResourceManager() : soundVolume(new float(100)), musicVolume(new float(100)) {}

    ResourceManager::~ResourceManager()
    {
        for(std::map<std::string, Texture*>::iterator itr = textureMap.begin(); itr != textureMap.end(); itr++)
        {
            delete itr->second;
        }

        textureMap.clear();

        for(std::map<std::string, Font*>::iterator itr = fontMap.begin(); itr != fontMap.end(); itr++)
        {
            delete itr->second;
        }

        fontMap.clear();

        for(std::map<std::string, SoundBuffer*>::iterator itr = soundBufferMap.begin(); itr != soundBufferMap.end(); itr++)
        {
            delete itr->second;
        }

        soundBufferMap.clear();

        for(std::map<std::string, Sound*>::iterator itr = soundMap.begin(); itr != soundMap.end(); itr++)
        {
            delete itr->second;
        }

        soundMap.clear();

        for(std::map<std::string, Music*>::iterator itr = musicMap.begin(); itr != musicMap.end(); itr++)
        {
            delete itr->second;
        }

        musicMap.clear();

        delete soundVolume;

        delete musicVolume;
    }

    ResourceManager* ResourceManager::getInstance()
    {
        if(instance == NULL)
            instance = new ResourceManager();

        return instance;
    }

    Texture* ResourceManager::requestTexture(std::string pathToTexture)
    {
        if(textureMap.find(pathToTexture) == textureMap.end())
        {
            Texture* newTexture = new Texture();
            if(!newTexture->loadFromFile(pathToTexture))
            {
                delete newTexture;
                return requestTexture("data/graphics/misc/unknown.png");
            }

            textureMap[pathToTexture] = newTexture;
        }

        return textureMap[pathToTexture];
    }

    Font* ResourceManager::requestFont(std::string pathToFont)
    {
        if(fontMap.find(pathToFont) == fontMap.end())
        {
            Font* newFont = new Font();
            if(!newFont->loadFromFile(pathToFont))
                throw("FONT_NOT_FOUNT " + pathToFont);

            fontMap[pathToFont] = newFont;
        }

        return fontMap[pathToFont];
    }

    void ResourceManager::setMusicVolume(float volume)
    {
        *musicVolume = volume;
    }

    void ResourceManager::setSoundVolume(float volume)
    {
        *soundVolume = volume;
    }

    void ResourceManager::playSound(std::string pathToSound, bool loop)
    {
        if(soundMap.find(pathToSound) == soundMap.end())
        {
            soundBufferMap[pathToSound] = new SoundBuffer();

            if(!soundBufferMap[pathToSound]->loadFromFile(pathToSound))
                throw("SOUND_NOT_FOUND " + pathToSound);

            soundMap[pathToSound] = new Sound();
            soundMap[pathToSound]->setBuffer(*soundBufferMap[pathToSound]);
            soundMap[pathToSound]->setVolume(*soundVolume);
        }

        soundMap[pathToSound]->setLoop(loop);
        soundMap[pathToSound]->play();
    }

    void ResourceManager::pauseSound(std::string pathToSound)
    {
        if(soundMap.find(pathToSound) != soundMap.end())
            soundMap[pathToSound]->pause();
    }

    void ResourceManager::stopSound(std::string pathToSound)
    {
        if(soundMap.find(pathToSound) != soundMap.end())
            soundMap[pathToSound]->stop();
    }

    void ResourceManager::loopSound(std::string pathToSound, bool loop)
    {
        if(soundMap.find(pathToSound) == soundMap.end())
        {
            soundBufferMap[pathToSound] = new SoundBuffer();
            if(!soundBufferMap[pathToSound]->loadFromFile(pathToSound))
                throw("SOUND_NOT_FOUND " + pathToSound);

            soundMap[pathToSound] = new Sound();
            soundMap[pathToSound]->setBuffer(*soundBufferMap[pathToSound]);
            soundMap[pathToSound]->setVolume(*soundVolume);
        }

        soundMap[pathToSound]->setLoop(loop);
    }

    Time ResourceManager::getSoundDuration(std::string pathToSound)
    {
        if(soundMap.find(pathToSound) == soundMap.end())
        {
            soundBufferMap[pathToSound] = new SoundBuffer();
            if(!soundBufferMap[pathToSound]->loadFromFile(pathToSound))
                throw("SOUND_NOT_FOUND " + pathToSound);

            soundMap[pathToSound] = new Sound();
            soundMap[pathToSound]->setBuffer(*soundBufferMap[pathToSound]);
            soundMap[pathToSound]->setVolume(*soundVolume);
        }

        return soundBufferMap[pathToSound]->getDuration();
    }

    void ResourceManager::playMusic(std::string pathToMusic, bool loop)
    {
        if(musicMap.find(pathToMusic) == musicMap.end())
        {
            musicMap[pathToMusic] = new Music();

            if(!musicMap[pathToMusic]->openFromFile(pathToMusic))
                throw("MUSIC_NOT_FOUND " + pathToMusic);

            musicMap[pathToMusic]->setLoop(loop);
            musicMap[pathToMusic]->setVolume(*musicVolume);
        }

        musicMap[pathToMusic]->play();
    }

    void ResourceManager::pauseMusic(std::string pathToMusic)
    {
        if(musicMap.find(pathToMusic) != musicMap.end())
            musicMap[pathToMusic]->pause();
    }

    void ResourceManager::stopMusic(std::string pathToMusic)
    {
        if(musicMap.find(pathToMusic) != musicMap.end())
            musicMap[pathToMusic]->stop();
    }

    void ResourceManager::loopMusic(std::string pathToMusic, bool loop)
    {
        if(musicMap.find(pathToMusic) == musicMap.end())
        {
            musicMap[pathToMusic] = new Music();

            if(!musicMap[pathToMusic]->openFromFile(pathToMusic))
                throw("MUSIC_NOT_FOUND " + pathToMusic);

            musicMap[pathToMusic]->setVolume(*musicVolume);
        }

        musicMap[pathToMusic]->setLoop(loop);
    }

    Time ResourceManager::getMusicDuration(std::string pathToMusic)
    {
        if(musicMap.find(pathToMusic) == musicMap.end())
        {
            musicMap[pathToMusic] = new Music();

            if(!musicMap[pathToMusic]->openFromFile(pathToMusic))
                throw("MUSIC_NOT_FOUND " + pathToMusic);

            musicMap[pathToMusic]->setLoop(false);
            musicMap[pathToMusic]->setVolume(*musicVolume);
        }

        return musicMap[pathToMusic]->getDuration();
    }
}
