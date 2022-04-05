/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_STATE_MANAGER_HPP
#define TGE_STATE_MANAGER_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>
#include <Tyrant/Framework/State.hpp>
#include <Tyrant/Framework/ResourceManager.hpp>
#include <Tyrant/Graphics.hpp>
#include <vector>

namespace TGE
{
    ////////////////////////////////////////////////////////
    /// \brief StateManager allows you to *gasp* manage game states.
    ////////////////////////////////////////////////////////
    class TGE_API StateManager : public StateListener
    {
        public:
            typedef struct
            {
                std::string name;
                State* state;
            } state_info;

            ////////////////////////////////////////////////
            /// \brief Default destructor
            ////////////////////////////////////////////////
            ~StateManager();

            ////////////////////////////////////////////////
            /// \brief Creates state_info for the new state
            /// and adds it to States for managing.
            ////////////////////////////////////////////////
            void manageState(std::string stateName, State* state);

            ////////////////////////////////////////////////
            /// \brief Polls through the state stack until
            /// it finds the state whose name matches the
            /// given argument, then returns that state.
            ////////////////////////////////////////////////
            State* findByName(std::string stateName);

            ////////////////////////////////////////////////
            /// \brief Returns a reference to the state at
            /// the top of the ActiveStateStack.
            ////////////////////////////////////////////////
            State* getActiveState();

            ////////////////////////////////////////////////
            /// \brief Begins execution at the given state.
            ////////////////////////////////////////////////
            void start(State* state);

            ////////////////////////////////////////////////
            /// \brief Exits the current state, pops it from
            /// the stack, then pushes the passed state to
            /// the front of the stack and starts it.
            ////////////////////////////////////////////////
            void changeState(State* state);

            ////////////////////////////////////////////////
            /// \brief Pushes the passed state to the front
            /// of the stack and starts it.
            ////////////////////////////////////////////////
            bool pushState(State* state);

            ////////////////////////////////////////////////
            /// \brief Pops the state at the top of the stack.
            ////////////////////////////////////////////////
            void popState();

            ////////////////////////////////////////////////
            /// \brief Pauses the state currently being
            /// executed.
            ////////////////////////////////////////////////
            void pauseState();

            ////////////////////////////////////////////////
            /// \brief Pops all states in the stack then
            /// pushes a given state to the top.
            ////////////////////////////////////////////////
            void popAllAndPushState(State* state);

        protected:
            ////////////////////////////////////////////////
            /// \brief Initialize new game state.
            ////////////////////////////////////////////////
            void init(State *state);

            std::vector<State*>		    ActiveStateStack; ///< States which are currently being executed
            std::vector<state_info>		States; ///< States which are managed by the StateManager
    };
}

#endif // STATE_MANAGER_HPP

////////////////////////////////////////////////////////////
/// \class TGE::StateManager
/// \ingroup framework
///
/// StateManager handles the loading and unloading of all
/// game states.
///
/// \see TGE::StateListener
///
////////////////////////////////////////////////////////////
