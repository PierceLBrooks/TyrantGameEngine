/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Framework/Game.hpp>
#include <Tyrant/Graphics.hpp>
#include <Tyrant/Framework/StateManager.hpp>
#include <vector>
#include <string>
#include <iostream>

namespace TGE
{
    Game* Game::instance = 0;

    Game::Game(std::string windowTitle, bool fullscreen, float width, float height)
    {
        if(fullscreen)
            window = new RenderWindow(VideoMode::getFullscreenModes().front(), windowTitle, Style::Fullscreen);
        else
            window = new RenderWindow(VideoMode(width, height), windowTitle);

        //eventManager = new EventListener();
        stateManager = 0;
        stateManager = new StateManager();

        color = TGE::Color::White;
        renderTexture.create(window->getSize().x,window->getSize().y);
        renderTextureGlobal.create(window->getSize().x,window->getSize().y);
        renderShader = nullptr;
        renderShaderGlobal = nullptr;
        renderPasses = 0;
    }

    void Game::create(std::string windowTitle, bool fullscreen, float width, float height)
    {
        instance = new Game(windowTitle, fullscreen, width, height);
    }

    Game* Game::getInstance()
    {
        return instance;
    }

    RenderWindow* Game::getWindow()
    {
        return window;
    }

    Shader* Game::getRenderShader()
    {
        return renderShader;
    }

    void Game::setRenderShader(Shader* s)
    {
        renderShader = s;
    }

    Shader* Game::getRenderShaderGlobal()
    {
        return renderShaderGlobal;
    }

    void Game::setRenderShaderGlobal(Shader* s)
    {
        renderShaderGlobal = s;
    }

    unsigned int Game::getRenderPasses()
    {
        return renderPasses;
    }

    void Game::setRenderPasses(unsigned int p)
    {
        renderPasses = p;
    }

    View Game::getView()
    {
        return renderTexture.getView();
    }

    void Game::setView(View view)
    {
        renderTexture.setView(view);
        //renderTextureGlobal.setView(view);
    }

    Game::~Game()
    {
        delete window;
        //delete eventManager;
        delete stateManager;
    }

    void Game::init(std::string startingState)
    {
        start(stateManager->findByName(startingState));
    }

    void Game::start(State* state)
    {
        try
        {
            stateManager->changeState(state);

            while(window->isOpen())
            {
                getInput();
                stateManager->getActiveState()->update();
                processEvents();
                drawScreen();
            }
        }
        catch(const char* crashMessage)
        {
            Log() << "ERROR: " << crashMessage << std::endl;

            TGE::Game::getInstance()->getStateManager()->getActiveState()->close();
        }
    }

    void Game::getInput()
    {
        Event event;

        while(window->pollEvent(event))
        {
            switch(event.type)
            {
                case Event::Closed:
                    window->close();
                    break;

                case Event::KeyPressed:
                    stateManager->getActiveState()->inputMap.triggerAction(event.key.code);
                    break;

                case Event::KeyReleased:
                    stateManager->getActiveState()->inputMap.triggerActionReleased(event.key.code);
                    break;

                case Event::MouseButtonPressed:
                    stateManager->getActiveState()->inputMap.triggerAction(event.mouseButton.button);
                    break;

                case Event::MouseButtonReleased:
                    stateManager->getActiveState()->inputMap.triggerActionReleased(event.mouseButton.button);
                    break;

                case Event::MouseWheelMoved:
                    stateManager->getActiveState()->inputMap.triggerAction(event.mouseWheel.delta, event.mouseWheel.x, event.mouseWheel.y);
                    break;

            }
        }
    }

    void Game::processEvents()
    {
    	//
    }

    void Game::drawScreen()
    {
        // the processing power issue is not anywhere in here, i tried returning and nothing dropped
        // ho hi lo ko do mo jo po go yo bo
        if ((renderTexture.getSize().x != window->getSize().x) || (renderTexture.getSize().y != window->getSize().y))
        {
            renderTexture.create(window->getSize().x,window->getSize().y);
            renderTextureGlobal.create(window->getSize().x,window->getSize().y);
        }
        window->clear();
        renderTexture.clear();
        renderTextureGlobal.clear();

        std::vector<Drawable*> tempDrawableStack;
        std::vector<Drawable*> tempDrawableStackOverlay;

        for(std::vector<Drawable*>::iterator itr = stateManager->getActiveState()->drawableStack.begin(); itr != stateManager->getActiveState()->drawableStack.end(); itr++)
        {
            tempDrawableStack.push_back(*itr);
        }

        for(std::vector<Drawable*>::iterator itr = stateManager->getActiveState()->drawableStackOverlay.begin(); itr != stateManager->getActiveState()->drawableStackOverlay.end(); itr++)
        {
            tempDrawableStackOverlay.push_back(*itr);
        }

        unsigned int i = 0;

        while(!tempDrawableStack.empty())
        {
            for(int ii = 0; ii < tempDrawableStack.size(); ii++)
            {
                if (tempDrawableStack[ii]->getDepth() == i)
                {
                    if (tempDrawableStack[ii]->getVisible())
                    {
                        if (tempDrawableStack[ii]->getRenderStates() != nullptr)
                        {
                            renderTexture.draw(*tempDrawableStack[ii],*tempDrawableStack[ii]->getRenderStates());
                        }
                        else
                        {
                            renderTexture.draw(*tempDrawableStack[ii]);
                        }
                    }
                    // crashes at 4 2 here when going to menu state... why?
                    tempDrawableStack.erase(tempDrawableStack.begin()+ii);
                    ii--;
                }
            }
            i++;
        }

        Sprite sprite(renderTexture.getTexture());
        sprite.setOrigin(window->getSize().x/2,window->getSize().y/2);
        sprite.setScale(1,-1);
        sprite.setPosition(window->getView().getCenter());
        if ((renderPasses != 0) && (renderShader != nullptr))
        {
            renderTextureGlobal.setView(getView());
            renderTexture.setView(window->getView());
            for (i = 0; i < renderPasses; i++)
            {
                renderShader->setParameter("pass",i);
                renderTexture.clear();
                renderTexture.draw(sprite,renderShader);
                sprite.setTexture(renderTexture.getTexture());
            }
            renderTexture.setView(renderTextureGlobal.getView());
            renderTextureGlobal.setView(window->getView());
            renderTextureGlobal.draw(sprite);
        }
        else
        {
            if (renderShader != nullptr)
            {
                renderTextureGlobal.draw(sprite,renderShader);
            }
            else
            {
                renderTextureGlobal.draw(sprite);
            }
        }
        renderTextureGlobal.setView(getView());

        i = 0;

        while(!tempDrawableStackOverlay.empty())
        {
            for(int ii = 0; ii < tempDrawableStackOverlay.size(); ii++)
            {
                if (tempDrawableStackOverlay[ii]->getDepth() == i)
                {
                    if (tempDrawableStackOverlay[ii]->getVisible())
                    {
                        if (tempDrawableStackOverlay[ii]->getRenderStates() != nullptr)
                        {
                            renderTextureGlobal.draw(*tempDrawableStackOverlay[ii],*tempDrawableStackOverlay[ii]->getRenderStates());
                        }
                        else
                        {
                            renderTextureGlobal.draw(*tempDrawableStackOverlay[ii]);
                        }
                    }
                    tempDrawableStackOverlay.erase(tempDrawableStackOverlay.begin()+ii);
                    ii--;
                }
            }
            i++;
        }

        renderTextureGlobal.setView(window->getView());
        sprite.setTexture(renderTextureGlobal.getTexture());
        sprite.setPosition(window->getView().getCenter());
        sprite.setColor(color);
        if (renderShaderGlobal != nullptr)
        {
            window->draw(sprite,renderShaderGlobal);
        }
        else
        {
            window->draw(sprite);
        }
        window->display();
    }

    StateManager* Game::getStateManager()
    {
        return stateManager;
    }
}
