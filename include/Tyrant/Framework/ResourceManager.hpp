/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_RESOURCEMANAGER_HPP
#define TGE_RESOURCEMANAGER_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>
#include <Tyrant/Graphics.hpp>
#include <Tyrant/Audio.hpp>
#include <map>

//#define getTexture *TGE::ResourceManager::getResourceManager()->requestTexture

namespace TGE
{
    class TGE_API ResourceManager
    {
        public:
            ~ResourceManager();
            static ResourceManager* getInstance();

            Texture* requestTexture(std::string pathToTexture = "");

            Font* requestFont(std::string pathToFont);

            void setSoundVolume(float volume);
            void setMusicVolume(float volume);

            void playSound(std::string pathToSound, bool loop = false);
            void pauseSound(std::string pathToSound);
            void stopSound(std::string pathToSound);
            void loopSound(std::string pathToSound, bool loop = true);
            Time getSoundDuration(std::string pathToSound);

            void playMusic(std::string pathToMusic, bool loop = false);
            void pauseMusic(std::string pathToMusic);
            void stopMusic(std::string pathToMusic);
            void loopMusic(std::string pathToMusic, bool loop = true);
            Time getMusicDuration(std::string pathToMusic);

        private:
            static ResourceManager* instance;
            ResourceManager();
            bool loadTexture(std::string pathToTexture);
            std::map<std::string, Texture*> textureMap;
            std::map<std::string, Font*> fontMap;
            std::map<std::string, SoundBuffer*> soundBufferMap;
            std::map<std::string, Sound*> soundMap;
            std::map<std::string, Music*> musicMap;
            float* soundVolume;
            float* musicVolume;
    };
} // namespace TGE

#endif // TGE_RESOURCEMANAGER_HPP
