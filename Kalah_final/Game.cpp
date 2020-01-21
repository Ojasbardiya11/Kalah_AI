//
//  Game.cpp
//  Kalah
//
//  Created by Ojas Bardiya on 5/18/19.
//  Copyright © 2019 Ojas bardiya. All rights reserved.
//
#include <iostream>

#include "Game.h"
#include "Board.h"
#include "Side.h"
#include "Player.h"

Game::Game(const Board& b, Player* south, Player* north) : mBoard(b), Southplayer(south), Northplayer(north),mCurrentSide(1)
{
    
}

void Game::display() const
{
    std::cout << "      " << Northplayer->name() << "       " << std::endl;
    std::cout << "   ";
    for (int i = 1; i <= mBoard.holes(); i++)
    {
        std::cout << mBoard.beans(NORTH, i) << "  ";
    }
    std::cout << std::endl;
    std::cout << mBoard.beans(NORTH, POT) << "            " << mBoard.beans(SOUTH, POT);
    std::cout << std::endl;
    std::cout << "   ";
    for (int j = 1; j <= mBoard.holes(); j++)
    {
        std::cout << mBoard.beans(SOUTH, j) << "  ";
    }
    std::cout << std::endl;
    std::cout << "      " << Southplayer->name() << "       " << std::endl;
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
    if (mBoard.beansInPlay(SOUTH) > 0 && mBoard.beansInPlay(NORTH) > 0)
    {
        over = false;
    }
    else
    {
        over = true;
        if (mBoard.beans(NORTH, POT) > mBoard.beans(SOUTH, POT))
        {
            hasWinner = true;
            winner = NORTH;
        }
        else if (mBoard.beans(NORTH, POT) < mBoard.beans(SOUTH, POT))
        {
            hasWinner = true;
            winner = SOUTH;
        }
        else
        {
            hasWinner = false;
        }
    }
}

bool Game::move()
{
    bool isGameOver;
    bool isWinner;
    Side winnerSide;
    Side endSide;
    Side turn;
    int endHole;
    int holeChosen;
    status(isGameOver, isWinner, winnerSide);
    if (isGameOver)
    {
        return false;
    }
    else
    {
        if (mCurrentSide == 1)  //determine which side moves
        {
            turn = SOUTH;
        }
        else
        {
            turn = NORTH;
        }
        if (turn == 1)  //Player of the south side
        {
            holeChosen = Southplayer->chooseMove(mBoard, SOUTH);
            std::cout << Southplayer->name() << " chooses hole " << holeChosen << std::endl;
            mBoard.sow(SOUTH, holeChosen, endSide, endHole);
            if (mBoard.beansInPlay(SOUTH) == 0)
            {
                if(mBoard.beansInPlay(NORTH) > 0)     //check if there any beans left on the North side
                {
                    for (int i = 1; i <= mBoard.holes(); i++)
                    {
                        mBoard.moveToPot(NORTH, i, NORTH);
                    }
                    std::cout << "Sweeping remaining beans into " << Northplayer->name() << "'s pot." << std::endl;
                }
                display();
                return true;
            }
            while (endHole == 0 && mBoard.beansInPlay(SOUTH) > 0)  //if the last bean is sown in a pot, the turn continues
            {
                display();
                std::cout << Southplayer->name() << " gets another turn." << std::endl;
                holeChosen = Southplayer->chooseMove(mBoard, SOUTH);
                std::cout << Southplayer->name() << " chooses hole " << holeChosen << std::endl;
                mBoard.sow(SOUTH, holeChosen, endSide, endHole);
            }
           if (endSide == SOUTH && mBoard.beans(SOUTH, endHole) == 1 && mBoard.beans(opponent(turn),endHole) > 0) //if the last hole in which the bean is sown was previously empty, move the beans of the opposite hole and that hole into the Side of the current player
            {
                mBoard.moveToPot(SOUTH, endHole, SOUTH);
                mBoard.moveToPot(opponent(turn), endHole, SOUTH);
                display();
            }
            else
            {
                display();
            }
        }
        else    //same algorithm in case the turn starts with the player on the North side
        {
            holeChosen = Northplayer->chooseMove(mBoard, NORTH);
            std::cout << Northplayer->name() << " chooses hole " << holeChosen << std::endl;
            mBoard.sow(NORTH, holeChosen, endSide, endHole);
            if (mBoard.beansInPlay(NORTH) == 0)
            {
                if (mBoard.beansInPlay(SOUTH) > 0)  //check if there are any beans left on the south side
                {
                    for (int j = 1; j <= mBoard.holes(); j++)
                    {
                        mBoard.moveToPot(SOUTH, j, SOUTH);
                    }
                    std::cout << "Sweeping remaining beans into " << Southplayer->name() << "'s pot." << std::endl;
                }
                display();
                return true;
            }
            while (endHole == 0 && mBoard.beansInPlay(NORTH) > 0)
            {
                display();
                std::cout << Northplayer->name() << " gets another turn." << std::endl;
                holeChosen = Northplayer->chooseMove(mBoard, NORTH);
                std::cout << Northplayer->name() << " chooses hole " << holeChosen << std::endl;
                mBoard.sow(NORTH, holeChosen, endSide, endHole);
            }
            if (endSide == NORTH && mBoard.beans(NORTH, endHole) == 1 && mBoard.beans(opponent(turn),endHole) > 0)
            {
                mBoard.moveToPot(NORTH, endHole, NORTH);
                mBoard.moveToPot(opponent(turn), endHole, NORTH);
                display();
            }
            else
            {
                display();
            }
        }
    }
    mCurrentSide = opponent(turn); // switch the move to the other player
    return true;
}

void Game::play()
{
    bool isGameOver;
    bool isWinner;
    Side winnerSide;
    std::string nameofWinner;
    status(isGameOver, isWinner, winnerSide);
    display();
    if ( Southplayer->isInteractive() == false && Northplayer->isInteractive() == false) //case when both players are not interactive
    {
        while(!isGameOver)
        {
            move();
            std::cout << "Press enter to continue. ";
            std::cin.ignore(10000, '\n');
            status(isGameOver, isWinner, winnerSide);
        }
    }
    else
    {
        while (!isGameOver)   //when atleast one is interactive
        {
            move();
            status(isGameOver, isWinner, winnerSide);
        }
    }
    
    if(mBoard.beansInPlay(NORTH) > 0)     //check if there any beans left on the North side
    {
        for (int i = 1; i <= mBoard.holes(); i++)
        {
            mBoard.moveToPot(NORTH, i, NORTH);
        }
        std::cout << "Sweeping remaining beans into " << Northplayer->name() << "'s pot." << std::endl;
    }
    else if (mBoard.beansInPlay(SOUTH) > 0)  //check if there are any beans left on the south side
    {
        for (int j = 1; j <= mBoard.holes(); j++)
        {
            mBoard.moveToPot(SOUTH, j, SOUTH);
        }
         std::cout << "Sweeping remaining beans into " << Southplayer->name() << "'s pot." << std::endl;
    }
    
    display();  //displays the end position
    status(isGameOver, isWinner, winnerSide);
    if (isWinner == false)
    {
        std::cout << "The match ended in a tie." << std::endl;
    }
    else
    {
        if (winnerSide == SOUTH)
        {
            nameofWinner = Southplayer->name();
            std::cout << "The winner is " << nameofWinner <<"." << std::endl;
        }
        else
        {
            nameofWinner = Northplayer->name();
            std::cout << "The winner is " << nameofWinner <<"." << std::endl;
        }
    }
}

int Game::beans(Side s, int hole) const
{
    if (hole < 0 || hole > mBoard.holes())
    {
        return -1;
    }
    int numofBeans = (mBoard.beans(s, hole));
    return (numofBeans);
}


