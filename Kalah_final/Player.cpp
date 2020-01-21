//
//  Player.cpp
//  Kalah
//
//  Created by Ojas Bardiya on 5/18/19.
//  Copyright © 2019 Ojas bardiya. All rights reserved.
//

#include <iostream>
#include <new>
#include <algorithm>
#include <cstdlib>
#include <climits>
#include "Player.h"
#include "Board.h"
#include "Side.h"

#include <chrono>
#include <future>
#include <atomic>
class AlarmClock
{
public:
    AlarmClock(int ms)
    {
        m_timedOut = false;
        m_isRunning = true;
        m_alarmClockFuture = std::async([=]() {
            for (int k = 0; k < ms && m_isRunning; k++)
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            if (m_isRunning)
                m_timedOut = true;
        });
    }
    
    ~AlarmClock()
    {
        m_isRunning = false;
        m_alarmClockFuture.get();
    }
    
    bool timedOut() const
    {
        return m_timedOut;
    }
    
    AlarmClock(const AlarmClock&) = delete;
    AlarmClock& operator=(const AlarmClock&) = delete;
private:
    std::atomic<bool> m_isRunning;
    std::atomic<bool> m_timedOut;
    std::future<void> m_alarmClockFuture;
};

//functions to determine choice of move for smartplayer
void minimax(const Board& b, Side s, const Side init_Side, int& Best_hole, int& value, AlarmClock& ac, int depth);
int evaluate(const Board& b, Side s, const Side init_Side, int hole);

Player::Player(std::string name) : m_name(name)
{
    
}

std::string Player::name() const
{
    return  m_name;
}

bool Player::isInteractive() const
{
    return false;
}

Player::~Player()
{
    
}

HumanPlayer::HumanPlayer(std::string humanName) : Player(humanName)
{
    
}

bool HumanPlayer::isInteractive() const
{
    return true;
}

int HumanPlayer::chooseMove(const Board& b, Side s) const
{
    if (b.beansInPlay(s) == 0)
    {
        return -1;
    }
    bool ans = true;
    int move;
    do
    {
        std::cout << "Select a hole, " << HumanPlayer::name() << ": ";
        std::cin >> move;
        if (b.beans(s, move) == 0 || move == 0 || move > b.holes())  //makes sure move entered is valid
        {
            std::cout << "Please enter a valid move..." << std::endl;
            ans = false;
        }
        else
        {
            ans = true;
        }
    }
    while (ans == false);
    return (move);
}

HumanPlayer::~HumanPlayer()
{
    
}

BadPlayer::BadPlayer(std::string badPlayerName) : Player(badPlayerName)
{
    
}

int BadPlayer::chooseMove(const Board& b, Side s) const
{

    int move;
    if (s == NORTH)            // returns the first hole which has any number of beans starting from the last hole on the North Side
    {
        for (int i = b.holes(); i >= 1; i--)
        {
            if (b.beans(s, i) > 0)
            {
                move = i;
                return (move);
            }
        }
    }
    else                        // returns the first hole which has any number of beans from the first hole on the South Side
    {
        for (int j = 1; j <= b.holes(); j++)
        {
            if (b.beans(s, j) > 0)
            {
                move = j;
                return (move);
            }
        }
    }
    return -1;  // If there are no holes that have any number of beans, the move cannot be played
}

BadPlayer::~BadPlayer()
{
    
}

SmartPlayer::SmartPlayer(std::string smartPlayerName) : Player(smartPlayerName)
{
    
}

int SmartPlayer::chooseMove(const Board& b, Side s) const
{
    AlarmClock ac(4900);
    int Best_hole = 2;
    int depth = 4;
    int value = 0;
    minimax(b, s, s, Best_hole, value, ac, depth);
    if (Best_hole == -1)
    {
        Best_hole = rand() % 1 + b.holes();
    }
    return Best_hole;
}

SmartPlayer::~SmartPlayer()
{
    
}

//implementation of minimax
void minimax(const Board& b, Side s, const Side init_Side, int& Best_hole, int& value, AlarmClock& ac, int depth)
{
    int count = 0;
    if (b.beansInPlay(s) == 0) //game is over
    {
        Best_hole = -1;
        if (s == init_Side)
        {
            if (b.beans(s, POT) > b.beans(opponent(s), POT))
            {
                value = 999;
            }
            else if (b.beans(s, POT) < b.beans(opponent(s), POT))
            {
                value = -999;
            }
            else
            {
                value = 0; //draw
            }
            return;
        }
        else
        {
            if (b.beans(s, POT) > b.beans(opponent(s), POT))
            {
                value = -999;
            }
            else if (b.beans(s, POT) < b.beans(opponent(s), POT))
            {
                value = 999;
            }
            else
            {
                value = 0; //draw
            }
            return;
        }
    }
    
    if (ac.timedOut() == true || depth == 0)
    {
        Best_hole = -1;
        if (s == init_Side)
        {
            if (b.beans(s, POT) > b.beans(opponent(s), POT))
            {
                value = 999;
            }
            else if (b.beans(s, POT) < b.beans(opponent(s), POT))
            {
                value = -999;
            }
            else
            {
                value = 0; //draw
            }
            return;
        }
        else
        {
            if (b.beans(s, POT) > b.beans(opponent(s), POT))
            {
                value = -999;
            }
            else if (b.beans(s, POT) < b.beans(opponent(s), POT))
            {
                value = 999;
            }
            else
            {
                value = 0; //draw
            }
            return;
        }
    }
    if (s == SOUTH)
    {
        if(b.beans(s, b.holes()) == 1) //last hole has only 1 bean
        {
            Best_hole = b.holes();
            value = evaluate(b, SOUTH, init_Side, 1);
            return;
        }
    }
    if (s == NORTH)
    {
        if(b.beans(s, 1) == 1)
        {
            Best_hole = 1;
            value = evaluate(b, NORTH, init_Side, 1);
            return;
        }
    }
    for (int j = 1; j <= b.holes(); j++)
    {
        if (ac.timedOut() == false && b.beans(s, j) > 0)
        {
            Side end_s;
            int end_h = 1;
            int val_2 = 0;
            count = count + 1;
            Board temp_board = b;
            int rand;
            temp_board.sow(s, j, end_s, end_h);
            if (end_s == s && end_h == 0)
            {
                minimax(temp_board, s, init_Side, rand, val_2, ac, depth);
            }
            if (b.beansInPlay(s) == 0)
            {
                if (b.beans(init_Side, POT) > b.beans(init_Side, POT))
                {
                    value = 999;
                    Best_hole = j;
                }
                else
                {
                    value = -999;
                    Best_hole = -1;
                }
            }
            else
            {
                minimax(temp_board, opponent(s), init_Side, rand, val_2, ac, depth - 1);
            }
            if (count == 1)
            {
                value = evaluate(b, s, init_Side, rand);
            }
            if (s == init_Side)
            {
                if (val_2 >= value)
                {
                    Best_hole = j;
                    value = val_2;
                }
            }
            else
            {
                if (val_2 >= value)
                {
                    Best_hole = j;
                    value = val_2;
                    
                }
            }
        }
    }
    return;
}

int evaluate(const Board& b, Side s, const Side init_Side, int hole)
{
    //set the heuristic used to evaluate what move the Smartplayer should play
    int P_heuristic = b.beans(s, POT) - b.beans(opponent(s), POT);
    if (s == init_Side)
    {
        if (P_heuristic >= 0)
        {
            if (P_heuristic == 0)
            {
                return 0;
            }
            else if (P_heuristic <= 2)
            {
                return 10;
            }
            else if (P_heuristic <= 4)
            {
                return 100;
            }
            else if (P_heuristic <= 8)
            {
                return 500;
            }
            else
            {
                return 1000;
            }
        }
        else
        {
            if (P_heuristic <= -8)
            {
                return -1000;
            }
            else if (P_heuristic <= -4)
            {
                return -500;
            }
            else if (P_heuristic <= -2)
            {
                return -100;
            }
            else
            {
                return -10;
            }
        }
    }
    else
    {
        if (P_heuristic >= 0)
        {
            if (P_heuristic == 0)
            {
                return 0;
            }
            else if (P_heuristic <= 2)
            {
                return -10;
            }
            else if (P_heuristic <= 4)
            {
                return -100;
            }
            else if (P_heuristic <= 8)
            {
                return -500;
            }
            else
            {
                return -1000;
            }
        }
        else
        {
            if (P_heuristic <= -8)
            {
                return 1000;
            }
            else if (P_heuristic <= -4)
            {
                return 500;
            }
            else if (P_heuristic <= -2)
            {
                return 100;
            }
            else
            {
                return 10;
            }
        }
    }
}
