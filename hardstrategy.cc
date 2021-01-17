// hardstrategy.cc
#include "hardstrategy.h"
#include "computer.h"

// play high, discard low strategy
Card HardStrategy::play(Computer *c) { 
    Card playCard;
    
    if (!c->getLegalPlays().empty()) {
        Faces max = Faces::NOFACE;
        for (auto card : c->getLegalPlays()) {
            if (card.face >= max) { 
                max = card.face;
                playCard = card; // max card in legal plays
            }
        }
        c->Player::play(playCard);
    } else {
        Faces min = Faces::KING;
        for (auto card : c->getHand()) {
            if (card.face <= min) { 
                min = card.face;
                playCard = card; // min card in hand
            }
        }
        c->Player::discard(playCard);
    }
    return playCard; 
}

HardStrategy::~HardStrategy() {}
