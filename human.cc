// human.cc
#include "human.h"

Human::Human(char id) : Player{id} {}

Card Human::makePlay() { return getHand().at(0); }

Human::~Human() {}

