/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_STATE_HPP
#define TGE_STATE_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>
//#include <Tyrant/Framework/Event.hpp>
#include <Tyrant/Framework/InputMap.hpp>
#include <Tyrant/Graphics.hpp>
#include <vector>
#include <string>

namespace TGE
{
    // Forward declaration since we use State in StateListener
    class TGE_API State;

    ////////////////////////////////////////////////////////
    /// \brief Abstract class for managing game states.
    ////////////////////////////////////////////////////////
    class TGE_API StateListener
    {
        public:
            ////////////////////////////////////////////////
            /// \brief Default constructor
            ////////////////////////////////////////////////
            StateListener() {};

            ////////////////////////////////////////////////
            /// \brief Virtual destructor
            ////////////////////////////////////////////////
            virtual ~StateListener() {};

            ////////////////////////////////////////////////
            /// \brief Virtual function for telling the
            /// state manager to manage a state.
            ////////////////////////////////////////////////
            virtual void manageState(std::string stateName, State* state) = 0;


            ////////////////////////////////////////////////
            /// \brief Returns the state whose identifier
            /// matches the given string.
            ////////////////////////////////////////////////
            virtual State*	    findByName(std::string stateName) = 0;

            ////////////////////////////////////////////////
            /// \brief Virtual function which returns the
            /// currently running state.
            ////////////////////////////////////////////////
            virtual State*      getActiveState() = 0;

            ////////////////////////////////////////////////
            /// \brief Changes the state to the given State.
            ////////////////////////////////////////////////
            virtual void		changeState(State *state) = 0;

            ////////////////////////////////////////////////
            /// \brief Pushes the given State to the top
            /// of the state stack.
            ////////////////////////////////////////////////
            virtual bool		pushState(State* state) = 0;

            ////////////////////////////////////////////////
            /// \brief Pops the state at the top of the
            /// state stack.
            ////////////////////////////////////////////////
            virtual void		popState() = 0;

            ////////////////////////////////////////////////
            /// \brief Pauses the currently active state.
            ////////////////////////////////////////////////
            virtual void		pauseState() = 0;

            ////////////////////////////////////////////////
            /// \brief Pops all states in the stack then
            /// pushes the given state to the state stack.
            ////////////////////////////////////////////////
            virtual void        popAllAndPushState(State* state) = 0;
    };

    ////////////////////////////////////////////////////////
    /// \brief Base class which is used to provide a basis
    /// on how game states should be designed.
    ////////////////////////////////////////////////////////
    class TGE_API State
    {
        public:
            ////////////////////////////////////////////////
            /// \brief DO NOT USE THIS FUNCTION\nUse the
            /// macro DECLARE_STATE_CLASS(ClassName) to set
            /// the state manager.
            ////////////////////////////////////////////////
            static void create(StateListener* Parent, const std::string name) {};

            ////////////////////////////////////////////////
            /// \brief Deletes the state from memory.
            ////////////////////////////////////////////////
            void destroy()
            {
                delete this;
            }

            ////////////////////////////////////////////////
            /// \brief Executes when the state is first called.
            ////////////////////////////////////////////////
            void close()
            {
                exit();
                /*for (unsigned int i = 0; i < drawableStack.size(); i++)
                {
                    delete drawableStack[i];
                }*/
                drawableStack.clear();
                /*for (unsigned int i = 0; i < drawableStackOverlay.size(); i++)
                {
                    delete drawableStackOverlay[i];
                }*/
                drawableStackOverlay.clear();
            }

            ////////////////////////////////////////////////
            /// \brief Virtual function, executed when the
            /// state is first called. This can be for
            /// loading a save file etc.
            ////////////////////////////////////////////////
            virtual void enter() = 0;

            ////////////////////////////////////////////////
            /// \brief Virtual function, executed when the
            /// state is closed. This can be for things like
            /// calling save functions etc.
            ////////////////////////////////////////////////
            virtual void exit() = 0;

            virtual void update() = 0;

            ////////////////////////////////////////////////
            /// \brief Virtual function, pauses execution
            /// of the game state. Used for things like
            /// calling a menu.
            ////////////////////////////////////////////////
            virtual bool pause()
            {
                return true;
            }

            ////////////////////////////////////////////////
            /// \brief Virtual function, resumes execution
            /// of the game state.
            ////////////////////////////////////////////////
            virtual void resume() {};

            //EventListener* eventManager;
            std::vector<Drawable*> drawableStack;
            std::vector<Drawable*> drawableStackOverlay;
            InputMap inputMap;

        protected:
            ////////////////////////////////////////////////
            /// \brief Default constructor
            ////////////////////////////////////////////////
            State() { /*eventManager = new EventListener();*/ };

            ////////////////////////////////////////////////
            /// \brief Calls the state manager which is
            /// managing this state to run findByName.
            ////////////////////////////////////////////////
            State* findByName(std::string stateName)
            {
                return parent->findByName(stateName);
            }

            ////////////////////////////////////////////////
            /// \brief Calls the state manager which is
            /// managing this state to change states.
            ////////////////////////////////////////////////
            void changeState(State* state)
            {
                parent->changeState(state);
            }

            ////////////////////////////////////////////////
            /// \brief Calls the state manager which is
            /// managing this state to push a state to the
            /// top of the state stack.
            ////////////////////////////////////////////////
            bool pushState(State* state)
            {
                return parent->pushState(state);
            }

            ////////////////////////////////////////////////
            /// \brief Calls the state manager which is
            /// managing this state to pop the state on the
            /// top of the state stack.
            ////////////////////////////////////////////////
            void popState()
            {
                parent->popState();
            }

            ////////////////////////////////////////////////
            /// \brief Calls the state manager which is
            /// managing this state to pop all states in
            /// the stack and push a given state.
            ////////////////////////////////////////////////
            void popAllAndPushState(State* state)
            {
                parent->popAllAndPushState(state);
            }

            /*template <typename Function>
            void newEvent(Function onRun, std::string eventName)
            {
                eventManager->newEvent([=](){onRun();}, eventName);
            }

            void triggerEvent(std::string eventName)
            {
                eventManager->triggerEvent(eventName);
            }*/

            template<typename functionOnClick>
            void bindAction(Keyboard::Key hotkey, functionOnClick onClick)
            {
                inputMap.bindAction(hotkey, [=](){onClick();}, [=](){});
            }

            template<typename functionOnClick, typename functionOnRelease>
            void bindAction(Keyboard::Key hotkey, functionOnClick onClick, functionOnRelease onRelease)
            {
                inputMap.bindAction(hotkey, [=](){onClick();}, [=](){onRelease();});
            }

            template<typename functionOnClick>
            void bindAction(Mouse::Button button, functionOnClick onClick)
            {
                inputMap.bindAction(button, [=](){onClick();}, [=](){});
            }

            template<typename functionOnClick, typename functionOnRelease>
            void bindAction(Mouse::Button button, functionOnClick onClick, functionOnRelease onRelease)
            {
                inputMap.bindAction(button, [=](){onClick();}, [=](){onRelease();});
            }

            template<typename functionOnPosScroll, typename functionOnNegScroll>
            void bindScrollAction(functionOnPosScroll functionPos, functionOnNegScroll functionNeg)
            {
                inputMap.bindScrollAction(functionPos, functionNeg);
            }

            StateListener* parent; ///< StateListener which is managing this state
    };

////////////////////////////////////////////////////////////
/// \brief Sets the states parent TGE::StateListener and
/// sets the string identifier of the state.
////////////////////////////////////////////////////////////
#define DECLARE_STATE_CLASS(T)                                            \
static void create(TGE::StateListener* Parent)                            \
{                                                                         \
    T* myState = new T();                                                 \
    myState->parent = Parent;                                             \
    Parent->manageState(#T, myState);                                     \
}
}

#endif // STATE_HPP

////////////////////////////////////////////////////////////
/// \class TGE::State
/// \ingroup framework
///
/// All game states inherit TGE::State. It provides a basis
/// on how a state class should be designed.
///
/// Example:
/// \code
/// #include <Tyrant/Framework.hpp>
///
/// class GameState : TGE::State
/// {
///     public:
///         GameState() {};
///
///         TGE::DECLARE_STATE_CLASS(GameState)
///
///         void enter()
///         {
///             // ...
///         }
///
///         void exit()
///         {
///             /*
///             ** Instead of having the user interact the with
///             ** game's state manager directly, all of the methods
///             ** of the state manager can be called within a
///             ** a state.
///             */
///             changeState(findByName("MenuState"));
///         }
///
///         void resume()
///         {
///             // ...
///         }
///
///     private:
///         void create();
/// }
///
/// int main()
/// {
///     Game app;
///     GameState::create(app->stateManager, "GameState");
///
///     try
///     {
///         app.init("GameState")
///     }
///     catch(std::exception& e)
///     {
///         #if __Win32
///             MessageBoxA(NULL, e.what(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
///         #else
///             fprintf(stderr, "An exception has occurred: %s\n", e.what());
///         #endif
///     }
/// }
/// \endcode
///
/// \see TGE::StateManager
///
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
/// \class TGE::StateListener
/// \ingroup framework
///
/// StateListener provide the base for StateManagers. They define
/// a few virtual functions for the StateManager to inherit.
/// so it can preform it's role.
///
/// \see TGE::StateManager
///
////////////////////////////////////////////////////////////
