//
//  Board.cpp
//  Kalah
//
//  Created by Ojas Bardiya on 5/18/19.
//  Copyright © 2019 Ojas bardiya. All rights reserved.
//

#include "Board.h"
#include "Side.h"

Board::Board(int nHoles, int nInitialBeansPerHole)
{
    if (nHoles <= 0)
    {
        nHoles = 1;
    }
    if (nInitialBeansPerHole < 0)
    {
        nInitialBeansPerHole = 0;
    }
    mHoles = nHoles;
    mBeansPerHole = nInitialBeansPerHole;
    arrSouth = new int[nHoles + 1];
    arrNorth = new int[nHoles + 1];
    arrSouth[POT] = 0;
    arrNorth[POT] = 0;
    for (int i = 1; i <= nHoles; i++)
    {
        arrSouth[i] = nInitialBeansPerHole;
        arrNorth[i] = nInitialBeansPerHole;
    }
}

int Board::holes() const
{
    return (mHoles);
}

int Board::beans(Side s, int hole) const
{
    
    if (hole > mHoles || hole < 0)
    {
        return -1;
    }
    if (s == NORTH)
    {
        return arrNorth[hole];
    }
    else
    {
        return  arrSouth[hole];
    }
}

int Board::beansInPlay(Side s) const
{
    int sumofBeans = 0;
    if (s == NORTH)
    {
        for (int i = 1; i <= mHoles; i++)
        {
            sumofBeans = sumofBeans + arrNorth[i];
        }
    }
    else
    {
        for (int j = 1; j <= mHoles; j++)
        {
            sumofBeans = sumofBeans + arrSouth[j];
        }
    }
    return (sumofBeans);
}

int Board::totalBeans() const
{
    int total = beansInPlay(NORTH) + beansInPlay(SOUTH) + beans(NORTH, 0) + beans(SOUTH, 0);
    return (total);
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
    if (hole == 0)
    {
        return false;
    }
    if (beans(s, hole) == 0 || beans(s, hole) == -1)
    {
        return false;
    }
    int numberofBeans= beans(s, hole);
    if (s == SOUTH)
    {
        arrSouth[hole] = 0;
        while (numberofBeans > 0)
        {
            for (int i = hole + 1; (i <= mHoles) && (numberofBeans > 0); i++)
            {
            arrSouth[i]++;
            numberofBeans--;
            endHole = i;
            }
            if (numberofBeans > 0)
            {
                arrSouth[POT]++;
                numberofBeans--;
                endHole = 0;
                if (numberofBeans > 0)
                {
                    s = NORTH;
                    for (int j = mHoles; (j >= 1) && (numberofBeans > 0); j--)
                    {
                        arrNorth[j]++;
                        numberofBeans--;
                        endHole = j;
                    }
                    if (numberofBeans > 0)
                    {
                        s = SOUTH;
                        for (int k = 1; (k <= hole) && (numberofBeans > 0); k++)
                        {
                            arrSouth[k]++;
                            numberofBeans--;
                            endHole = k;
                        }
}
                }
            }
        }
    }
    else if (s == NORTH)
    {
        arrNorth[hole] = 0;
        while (numberofBeans > 0)
        {
            for (int i = hole - 1; (i >= 1) && (numberofBeans > 0); i--)
            {
                arrNorth[i]++;
                numberofBeans--;
                endHole = i;
            }
            if (numberofBeans > 0)
            {
                arrNorth[POT]++;
                numberofBeans--;
                endHole = 0;
                if (numberofBeans > 0)
                {
                    s = SOUTH;
                    for (int j = 1; (j <= mHoles) && (numberofBeans > 0); j++)
                    {
                        arrSouth[j]++;
                        numberofBeans--;
                        endHole = j;
                    }
                    if (numberofBeans > 0)
                    {
                        s = NORTH;
                        for (int k = mHoles; (k >= 1) && (numberofBeans > 0); k++)
                        {
                            arrNorth[k]++;
                            numberofBeans--;
                            endHole = k;
                        }
                    }
                }
            }
    }
    }
    endSide = s;
    return true;
}

bool Board::moveToPot(Side s, int hole, Side potOwner)
{
    if (hole == 0)
    {
        return false;
    }
    if (beans(s, hole) == -1)
    {
        return false;
    }
    if (potOwner == SOUTH)
    {
        arrSouth[POT] = arrSouth[POT] + beans(s, hole);
        if (s == NORTH)
        {
            arrNorth[hole] = 0;
        }
        else
        {
            arrSouth[hole] = 0;
        }
    }
    else
    {
        arrNorth[POT] = arrNorth[POT] + beans(s, hole);
        if (s == NORTH)
        {
            arrNorth[hole] = 0;
        }
        else
        {
            arrSouth[hole] = 0;
        }
    }
    return true;
}

bool Board::setBeans(Side s, int hole, int beans)
{
    if (hole > mHoles || hole < 0)
    {
        return false;
    }
    if (beans < 0)
    {
        return false;
    }
    if (s == SOUTH)
    {
        arrSouth[hole] = beans;
    }
    else
    {
        arrNorth[hole] = beans;
    }
    return true;
}

Board::Board(const Board& other)
{
    mHoles = other.mHoles;
    mBeansPerHole = other.mBeansPerHole;
    arrNorth = new int[other.mHoles + 1];
    arrSouth = new int[other.mHoles + 1];
    arrNorth[POT] = other.beans(NORTH, POT);
    arrSouth[POT] = other.beans(SOUTH, POT);
    for (int i = 1; i <= other.mHoles; i++)
    {
        arrSouth[i] = other.beans(SOUTH, i);
        arrNorth[i] = other.beans(NORTH, i);
    }
}

Board& Board::operator=(const Board& other)
{
    if (this == &other)
    {
        return *this;
    }
    delete [] arrNorth;
    delete [] arrSouth;
    mHoles = other.mHoles;
    mBeansPerHole = other.mBeansPerHole;
    arrNorth = new int[other.mHoles + 1];
    arrSouth = new int[other.mHoles + 1];
    arrSouth[POT] = other.beans(SOUTH, POT);
    arrNorth[POT] = other.beans(NORTH, POT);
    for (int i = 1; i <= other.mHoles; i++)
    {
        arrSouth[i] = other.beans(SOUTH, i);
        arrNorth[i] = other.beans(NORTH, i);
    }
    return *this;
}

Board::~Board()
{
    delete [] arrSouth;
    delete [] arrNorth;
}
