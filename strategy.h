// strategy.h
#ifndef _STRATEGY_H
#define _STRATEGY_H

class Card;
class Computer;

class Strategy {
    public:
    virtual Card play(Computer *c) = 0;
};

#endif
