// easystrategy.cc
#include "easystrategy.h"
#include "computer.h"

// play low, discard high strategy
Card EasyStrategy::play(Computer *c) { 
    Card playCard;
    
    if (!c->getLegalPlays().empty()) {
        Faces min = Faces::KING;
        for (auto card : c->getLegalPlays()) {
            if (card.face <= min) { 
                min = card.face;
                playCard = card; // min card in legal plays
            }
        }
        c->Player::play(playCard);
    } else {
        Faces max = Faces::NOFACE;
        for (auto card : c->getHand()) {
            if (card.face >= max) { 
                max = card.face;
                playCard = card; // max card in hand
            }
        }
        c->Player::discard(playCard);
    }
    return playCard; 
}

EasyStrategy::~EasyStrategy() {}
