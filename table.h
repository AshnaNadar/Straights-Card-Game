// table.h
#ifndef _TABLE_H
#define _TABLE_H

#include "deck.h"
#include "suitpile.h"
#include "player.h"
#include "subject.h"
#include "card.h"

#include <vector>
#include <memory>

class Strategy;

enum Difficulty { EASY = 0, MEDIUM, HARD };

class Table : public Subject {
    public:
        // ctor and dtor:
        Table(unsigned int seed, int gameEndScore = 80, int numOfPlayers = 4, Faces startFace = SEVEN, Suits startSuit = SPADE, 
            Difficulty defaultLevel = MEDIUM, bool dynamicStrategy = false);
        virtual ~Table();

        void newCompPlayer();
        void nextPlayer();
        void addToPile(Card c);
        void findWinners();

        // accessor and mutator methods:
        void setSeed(unsigned int seed);
        int getPlayerCount();
        Deck getDeck();
        unsigned int getTurnCount();
        void setTurnCount(int count);
        void setGameEndScore(int score);
        std::shared_ptr<Player> getCurPlayer();
        void setCurPlayer(std::shared_ptr<Player> p);
        std::vector<std::shared_ptr<Player>> getPlayers();
        std::vector<std::shared_ptr<Player>> getWinners();
        std::vector<std::shared_ptr<SuitPile>> getSuitPiles();

    private:
        void newRound();
        void generateLegalPlays();
        void setDynamicCompDifficulty(std::shared_ptr<Player> player);
        

        Deck deck;
        unsigned int numOfTurns;
        std::shared_ptr<Player> curPlayer;
        std::vector<std::shared_ptr<Player>> players;
        std::vector<std::shared_ptr<Player>> winners;
        std::vector<std::shared_ptr<SuitPile>> suitPiles;    
        Card startingCard;
        int gameEndScore;
        int numOfPlayers;
        std::shared_ptr<Strategy> defaultStrategy;
        bool dynamicStrategy;
        std::vector<std::shared_ptr<Strategy>> playStrategy;
    };

#endif
