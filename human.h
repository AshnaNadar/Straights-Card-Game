// human.h

#ifndef _HUMAN_H
#define _HUMAN_H

#include "player.h"
#include "card.h"

class Human : public Player {
    public:
    Human(char id);
    virtual ~Human();

    // invariant: Human::makePlay() will never be called
    Card makePlay() override;
};

#endif
