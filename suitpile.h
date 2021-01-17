// suitpile.h
#ifndef _SUITPILE_H
#define _SUITPILE_H

#include <vector>
#include <memory>

#include "card.h"

class SuitPile {
    public:
        // ctor and dtor:
        SuitPile(Suits suit);
        virtual ~SuitPile();

        void addToTop(Card card); 
        void addToBottom(Card card);
        void clear();
        bool isEmpty();
        bool hasCard (Card card);

        // accessor and mutator methods:
        Faces getTop();
        Faces getBottom();
        Suits getSuit();
        std::vector<Card> getCards();

    private:
        Suits suit;
        Faces top;
        Faces bottom;
        std::vector<Card> pile;
};

#endif
