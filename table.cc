// table.cc
#include "table.h"
#include "human.h"
#include "computer.h"
#include "easystrategy.h"
#include "basicstrategy.h"
#include "hardstrategy.h"

#include <algorithm>

using std::dynamic_pointer_cast;
using std::make_shared;
using std::shared_ptr;
using std::vector;
using std::move;


Table::Table(unsigned int seed, int gameEndScore, int numOfPlayers, Faces startFace, Suits startSuit, Difficulty defaultLevel, bool dynamicStrategy)
    : deck{seed}, numOfTurns{0}, startingCard{startFace, startSuit}, gameEndScore{gameEndScore}, numOfPlayers{numOfPlayers}, dynamicStrategy{dynamicStrategy} {
    
    // initialize players
    for (int id = 1; id <= numOfPlayers; id++) {
        players.emplace_back(make_shared<Human>(id + '0'));
    }

    // initialize suitpiles
    for (int s = Suits::CLUB; s <= Suits::SPADE; s++) {
        Suits suit = static_cast<Suits>(s);
        suitPiles.emplace_back(make_shared<SuitPile>(suit));
    }

    // initialize play strategies
    playStrategy.emplace_back(make_shared<EasyStrategy>());
    playStrategy.emplace_back(make_shared<BasicStrategy>());
    playStrategy.emplace_back(make_shared<HardStrategy>());

    // initialize default strategy
    defaultStrategy = playStrategy.at(defaultLevel);

    // start game
    newRound();
}


Table::~Table() {}


void Table::newCompPlayer() {
    auto comp = make_shared<Computer>(move(curPlayer), defaultStrategy);
    curPlayer = comp;
    players.at(curPlayer->getId() - '0' - 1) = comp;
    setDynamicCompDifficulty(curPlayer);
}


void Table::newRound() {
    // shuffle deck
    deck.shuffle();
        
    int cardsPerPlayer = deck.getCards().size()/players.size();
    int startPos = 0;
    int endPos = cardsPerPlayer;
    for (auto player : players) { 
        // empty hand, legal plays, and discard pile
        player->clearHand();
        player->clearLegalPlays();
        player->clearDiscardPile();
        // deal cards in deck to players
        vector<Card> newHand;
        vector<Card> theDeck = deck.getCards();
        newHand.assign(theDeck.begin() + startPos, theDeck.begin() + endPos);
        player->setHand(newHand);
        startPos += cardsPerPlayer;
        endPos += cardsPerPlayer;
        // set starting player
        if (player->hasCard(startingCard)) {
            curPlayer = player;
        }
        // set gameScore and reset roundScore
        player->updateGameScore();
        player->setRoundScore(0);
        // reset dynamic computer player difficulty
        setDynamicCompDifficulty(player);
    }
    // reset turn count
    numOfTurns = 0;
    // generate legal plays
    generateLegalPlays();
    // clear suit piles
    for (auto pile : suitPiles) {
        pile->clear();
    }
}


void Table::nextPlayer() {
    if (deck.getCards().size() == ++numOfTurns) { // ROUND ENDS
        findWinners();
        notify(); // notify view of round summary/winner(s) (if any)

        if (winners.empty()) { // GAME CONTINUES
            newRound();
            notify(); // notify view of new round
        }
    } else { // ROUND CONTINUES
        unsigned int nextPlayer = curPlayer->getId() - '0';
        if (nextPlayer == players.size()) { nextPlayer = 0; }
        curPlayer = players.at(nextPlayer); // go to next player
        setDynamicCompDifficulty(curPlayer);

        if (curPlayer->getHand().empty()) {
            this->nextPlayer(); // if no cards in hand, go to next player
        } else {
            generateLegalPlays();
            notify();
        }
    }
}


void Table::generateLegalPlays() {
    curPlayer->clearLegalPlays();
    if (numOfTurns == 0) { // curPlayer is the starting player; invariant: starting player has startingCard
        curPlayer->addLegalPlay(startingCard); // only legal play first player has is starting card
    } else {
        for (auto card : curPlayer->getHand()) {
            shared_ptr<SuitPile> pile = suitPiles.at(card.suit);
            if (card.face == startingCard.face) {
                curPlayer->addLegalPlay(card);
            } else if (!pile->isEmpty() && ((card.face == pile->getTop() + 1) || (card.face == pile->getBottom() - 1))) {
                curPlayer->addLegalPlay(card);
            }
        }
    }
}


// invariant: card is a legal play (can be added to suit pile)
void Table::addToPile(Card c) {
    shared_ptr<SuitPile> pile = suitPiles.at(c.suit);
    if (c.face > pile->getTop()) { // note: startingCard always greater than NOFACE
        pile->addToTop(c);
    } else {
        pile->addToBottom(c);
    }
}


void Table::findWinners() {
    int minScore = (players.at(0)->getGameScore() + players.at(0)->getRoundScore());
    bool gameOver = false;
    // find player(s) with lowest score
    for (auto player : players) {
        int playerScore = player->getRoundScore() + player->getGameScore();
        if (playerScore >= gameEndScore) { gameOver = true; }
        if (playerScore < minScore) {
            minScore = playerScore;
            winners.clear();
            winners.emplace_back(player);
        } else if (playerScore == minScore) {
            winners.emplace_back(player);
        }
    }
    // if no players score >= game ending score, no winners
    if (!gameOver) { winners.clear(); }
}


void Table::setSeed(unsigned int seed) {
    deck = Deck{seed};
    newRound();
}


Deck Table::getDeck() { return deck; }


unsigned int Table::getTurnCount() { return numOfTurns; }


shared_ptr<Player> Table::getCurPlayer() { return curPlayer; }


void Table::setCurPlayer(shared_ptr<Player> p) { curPlayer = p; }


vector<shared_ptr<Player>> Table::getPlayers() { return players; }


vector<shared_ptr<Player>> Table::getWinners() { return winners; }


vector<shared_ptr<SuitPile>> Table::getSuitPiles() { return suitPiles; }


void Table::setTurnCount(int count) { numOfTurns = count; }


void Table::setGameEndScore(int score) { gameEndScore = score; }


int Table::getPlayerCount() { return numOfPlayers; }


// if the curPlayer is a dynamic computer player, sets strategy difficulty according to their hand
void Table::setDynamicCompDifficulty(shared_ptr<Player> player) {
if ((player->getIsComp()) && dynamicStrategy) {
            if (player->getHand().size() > 9) {
                dynamic_pointer_cast<Computer>(player)->setStrategy(playStrategy.at(Difficulty::EASY));
            } else if (player->getHand().size() < 5) {
                dynamic_pointer_cast<Computer>(player)->setStrategy(playStrategy.at(Difficulty::HARD));
            } else { dynamic_pointer_cast<Computer>(player)->setStrategy(playStrategy.at(Difficulty::MEDIUM)); }
        }
}
