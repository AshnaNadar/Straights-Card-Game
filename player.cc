//player.cc 
#include "player.h"

#include <algorithm>

using std::find;
using std::vector;

// note: all players are initially assumed to be humans
Player::Player(char id, bool isComp, int gameScore, int roundScore) 
    : id{id}, isComp{isComp}, gameScore{gameScore}, roundScore{roundScore} {}

Player::~Player() {}

void Player::discard(Card card) {
    if (legalPlays.empty()) {
        discardPile.emplace_back(card);
        hand.erase(find(hand.begin(), hand.end(), card)); // note: assuming card is in hand
        roundScore += card.face;
    } else { throw std::exception{}; } // must make legal play
}

void Player::play(Card card) {
    if (find(legalPlays.begin(), legalPlays.end(), card) != legalPlays.end()) {
        hand.erase(find(hand.begin(), hand.end(), card)); // note: assuming card is in hand
        
    } else { throw std::exception{}; } // not legal play, must discard
}

char Player::getId() { return id; }

bool Player::getIsComp() { return isComp; }

int Player::getRoundScore() { return roundScore; }

void Player::setRoundScore(int score) { roundScore = score; }

int Player::getGameScore() { return gameScore; }

void Player::updateGameScore() { gameScore += roundScore; }

vector<Card> Player::getHand() { return hand; }

void Player::setHand(std::vector<Card> newHand) { hand = newHand; }

vector<Card> Player::getDiscards() { return discardPile; }

void Player::clearHand() { hand.clear(); }

void Player::clearDiscardPile() { discardPile.clear(); }

vector<Card> Player::getLegalPlays() { return legalPlays; }

void Player::clearLegalPlays() { legalPlays.clear(); }

void Player::addLegalPlay(Card card) { legalPlays.emplace_back(card); }

bool Player::hasCard(Card card) { return std::find(hand.begin(), hand.end(), card) != hand.end(); };
