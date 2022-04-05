/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_GAME_HPP
#define TGE_GAME_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>
#include <Tyrant/Graphics.hpp>
//#include <Tyrant/Framework/Event.hpp>
#include <Tyrant/Framework/State.hpp>
#include <Tyrant/Framework/StateManager.hpp>
#include <vector>
#include <string>

namespace TGE
{
    ////////////////////////////////////////////////////////////
    /// \brief Object which handles game states and logic.
    ////////////////////////////////////////////////////////////
    class TGE_API Game
    {
        public:
            ////////////////////////////////////////////////////
            /// \brief Default destructor
            ////////////////////////////////////////////////////
            ~Game();

            static void create(std::string windowTitle, bool fullscreen, float width = 0, float height = 0);

            static Game* getInstance();

            RenderWindow* getWindow();

            Shader* getRenderShader();

            void setRenderShader(Shader* s);

            Shader* getRenderShaderGlobal();

            void setRenderShaderGlobal(Shader* s);

            unsigned int getRenderPasses();

            void setRenderPasses(unsigned int p);

            View getView();

            void setView(View view);

            Color getColor();

            void setColor(Color c);

            ////////////////////////////////////////////////////
            /// \brief Creates window and StateManager then calls
            /// start(std::string startingState).
            ////////////////////////////////////////////////////
            void init(std::string startingState);

            ////////////////////////////////////////////////////
            /// \brief Takes user input and checks it against
            /// input map.
            ////////////////////////////////////////////////////
            void getInput();

            ////////////////////////////////////////////////////
            /// \brief Polls for events (sf::Event defined and
            /// user-defined) and if triggered executes.
            ////////////////////////////////////////////////////
            void processEvents();

            ////////////////////////////////////////////////////
            /// \brief Runs through the stack of sf::Drawable
            /// objects and displays them with proper depth.
            ////////////////////////////////////////////////////
            void drawScreen();

            ////////////////////////////////////////////////////
            /// \brief Starts executing the main game loop in
            /// the state you pass to it.
            ////////////////////////////////////////////////////
            void start(State* state);

            StateManager* getStateManager();

        private:
            Game(std::string windowTitle, bool fullscreen, float width = 0, float height = 0);
            StateManager* stateManager; ///< The game's stateManager
            //EventListener* eventManager; ///< The game's eventListener
            RenderWindow* window; ///< The game's window
            Shader* renderShader;
            Shader* renderShaderGlobal;
            RenderTexture renderTexture, renderTextureGlobal;
            Color color;
            unsigned int renderPasses;
            static Game* instance;
    };
}

#endif // GAME_HPP

////////////////////////////////////////////////////////////
/// \class TGE::Game
/// \ingroup framework
///
/// Game is the bread and butter of TGE. It creates the
/// games sf::RenderWindow and TGE::StateManager then loads
/// the initial game state. From there, all game logic is
/// loaded from external TGE::State classes and scripts.
///
/// Example:
/// \code
/// #include <Tyrant/Framework.hpp>
///
/// class MenuState : TGE::State
/// {
///     TGE::DECLARE_STATE_CLASS(MenuState)
/// }
///
/// int main()
/// {
///     Game app();
///
///     MenuState::create(app.stateManager, "MenuState");
///
///     try
///     {
///         app.init("MenuState");
///     }
///     catch(std::exception& e)
///     {
///         #if __Win32
///             MessageBoxA(NULL, e.what(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
///         #else
///             fprintf(stderr, "An exception has occurred: %s\n", e.what());
///         #endif
///     }
///
///     return 0;
/// }
/// \endcode
///
/// \see TGE::StateManager
///
////////////////////////////////////////////////////////////
