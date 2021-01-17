// player.h
#ifndef _PLAYER_H
#define _PLAYER_H

#include "card.h"

#include <vector>

class Player {
    public:
        Player(char id, bool isComp = false, int gameScore = 0, int roundScore = 0);
        virtual ~Player();
        void discard(Card card);
        void play(Card card);
        virtual Card makePlay() = 0;
        void updateGameScore();
        void clearHand();
        void clearDiscardPile();
        void clearLegalPlays();
        void addLegalPlay(Card card);
        bool hasCard(Card card);

        char getId();
        bool getIsComp();
        int getRoundScore();
        void setRoundScore(int score);
        int getGameScore();
        std::vector<Card> getHand();
        void setHand(std::vector<Card> newHand);
        std::vector<Card> getDiscards();
        std::vector<Card> getLegalPlays();
        

    protected:
        char id;
        bool isComp;
        int gameScore;
        int roundScore;
        std::vector<Card> hand;
        std::vector<Card> discardPile; 
        std::vector<Card> legalPlays; 
};

#endif
