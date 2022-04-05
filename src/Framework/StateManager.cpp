/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Framework/StateManager.hpp>
#include <iostream>

namespace TGE
{
    StateManager::~StateManager()
    {
        state_info si;

        while(!ActiveStateStack.empty())
        {
            ActiveStateStack.back()->close();
            ActiveStateStack.pop_back();
        }

        while(!States.empty())
        {
            si = States.back();
            si.state->destroy();
            States.pop_back();
        }
    }

    void StateManager::manageState(std::string stateName, State* state)
    {
        state_info new_state_info;
        new_state_info.name = stateName;
        new_state_info.state = state;
        States.push_back(new_state_info);
    }

    State* StateManager::findByName(std::string stateName)
    {
        std::vector<state_info>::iterator itr;

        for(itr=States.begin(); itr!=States.end(); itr++)
        {
            if(itr->name==stateName)
                return itr->state;
        }

        return 0;
    }

    State* StateManager::getActiveState()
    {
        if(!ActiveStateStack.empty())
        {
            return ActiveStateStack.back();
        }

        return NULL;
    }

    void StateManager::changeState(State* state)
    {
        if(!ActiveStateStack.empty())
        {
            ActiveStateStack.back()->close();
            ActiveStateStack.pop_back();
        }
        ActiveStateStack.push_back(state);
        init(state);
        ActiveStateStack.back()->enter();
    }

    bool StateManager::pushState(State* state)
    {
        if(!ActiveStateStack.empty())
        {
            if(!ActiveStateStack.back()->pause())
                return false;
        }

        ActiveStateStack.push_back(state);
        init(state);
        ActiveStateStack.back()->enter();

        return true;
    }

    void StateManager::popState()
    {
        if(!ActiveStateStack.empty())
        {
            ActiveStateStack.back()->close();
            ActiveStateStack.pop_back();
        }

        if(!ActiveStateStack.empty())
        {
            init(ActiveStateStack.back());
            ActiveStateStack.back()->resume();
        }
    }

    void StateManager::popAllAndPushState(State* state)
    {
        while(!ActiveStateStack.empty())
        {
            ActiveStateStack.back()->close();
            ActiveStateStack.pop_back();
        }

        pushState(state);
    }

    void StateManager::pauseState()
    {
        if(!ActiveStateStack.empty())
        {
            ActiveStateStack.back()->pause();
        }

        if(ActiveStateStack.size() > 2)
        {
            init(ActiveStateStack.at(ActiveStateStack.size() - 2));
            ActiveStateStack.at(ActiveStateStack.size() - 2)->resume();
        }
    }

    void StateManager::init(State* state)
    {
        // TODO Initialize new state
    }
}
