//suitpile.cc
#include "suitpile.h"

#include <algorithm>

SuitPile::SuitPile(Suits suit) : suit{suit}, top{Faces::NOFACE}, bottom{Faces::NOFACE} {}

SuitPile::~SuitPile() {}

void SuitPile::addToTop(Card card) {
    pile.emplace_back(card);
    top = card.face;
    if (bottom == NOFACE) { bottom = card.face; } // first card added
}

void SuitPile::addToBottom(Card card) {
    pile.emplace(pile.begin(), card);
    bottom = card.face;
    if (top == NOFACE) { top = card.face; } // first card added
}

bool SuitPile::isEmpty() {
    return pile.empty();
}

void SuitPile::clear() {
    pile.clear();
    top = bottom = NOFACE;
} 

Faces SuitPile::getTop() { return top; }

Faces SuitPile::getBottom() { return bottom; }

Suits SuitPile::getSuit() { return suit; }

std::vector<Card> SuitPile::getCards() { return pile; }

bool SuitPile::hasCard (Card card) { return std::find(pile.begin(), pile.end(), card) != pile.end(); }
