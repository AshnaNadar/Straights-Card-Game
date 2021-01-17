// textview.cc

/*
 * used MVC example - Text-based view
 *
 *  Created by Gustavo Tondello on 29/06/20 based on GTKmm version.
 *  Copyright 2009, 2017, 2020 UW. All rights reserved.
 *
 */

#include "observer.h"
#include "textview.h"
#include "controller.h"
#include "table.h"
#include "subject.h"

#include <iostream>
#include <string>
#include <sstream>
#include <map>

using std::istringstream;
using std::getline;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::map;

const char face_names[] = { 'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K' };
const char suit_names[] = { 'C', 'D', 'H', 'S' };
const string suit_pile_names[] = { "Clubs", "Diamonds", "Hearts", "Spades" };
map<char, int> faces = { {'A', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9}, {'T', 10}, {'J', 11}, {'Q', 12}, {'K', 13} };
map<char, int> suits = { {'C', 0}, {'D', 1}, {'H', 2}, {'S', 3} };

TextView::TextView(Controller *c, Table *t, std::istream *in) : table_{t}, controller_{c}, in{in} {
    // Register view as observer of model.
    table_->subscribe(this);

    // prompt user to input player types for each player
    int startingPlayer = table_->getCurPlayer()->getId() - '0' - 1;
    for (auto player : table_->getPlayers()) {
        table_->setCurPlayer(player);
        cout << "Is Player" << player->getId() << " a human (h) or a computer (c)?" << endl << ">";
        char playerType;
        *in >> playerType;
        if (playerType == 'c') {
            controller_->compPlayerButtonClicked();
        }
    }
    table_->setCurPlayer(table_->getPlayers().at(startingPlayer));
    in->clear();
    in->ignore();

    // Display initial view
    update();
}


TextView::~TextView() {}


void TextView::update() {
    if (table_->getDeck().getCards().size() == table_->getTurnCount()) { // GAME/ROUND ENDS
        // PRINT PLAYERS DISCARDS & SCORES
        for (auto player : table_->getPlayers()) {
            cout << "Player" << player->getId() << "'s discards:";
            for (auto card : player->getDiscards()) {
                cout << " " << face_names[card.face-1] << suit_names[card.suit];
            } 
            cout << endl;
            cout << "Player" << player->getId() << "'s score: " << player->getGameScore() << " + " << player->getRoundScore() << " = " << player->getRoundScore() + player->getGameScore() << endl;
        }
        if (!table_->getWinners().empty()) { // GAME ENDS
            // PRINT WINNER(S)
            for (auto player : table_->getWinners()) {
                cout << "Player" << player->getId() << " wins!" << endl;
            }
        }

    } else { // GAME CONTINUES
        
        if (table_->getTurnCount() == 0) { // NEW ROUND
            cout << "A new round begins. It's Player" << table_->getCurPlayer()->getId() << "'s turn to play." << endl;
        }
        if (!table_->getCurPlayer()->getIsComp()) { // HUMAN PLAYER TURN
            // PRINT SUIT PILES
            cout << "Cards on the table:" << endl;
            for (auto pile : table_->getSuitPiles()) {
                cout << suit_pile_names[pile->getSuit()] << ":";
                for (auto card: pile->getCards()) {
                    cout << " " << face_names[card.face-1];
                }
                cout << endl;
            }
            // PRINT HAND
            cout << "Your hand:";
            for (auto card : table_->getCurPlayer()->getHand()) {
                cout << " " << face_names[card.face-1] << suit_names[card.suit];
            } 
            cout << endl;
            // PRINT LEGAL PLAYS
            cout << "Legal plays:";
            for (auto card : table_->getCurPlayer()->getLegalPlays()) {
                cout << " " << face_names[card.face-1] << suit_names[card.suit];
            } 
            cout << endl;
        }
        nextCommand();
    }
}


void TextView::nextCommand() {
    if (table_->getCurPlayer()->getIsComp()) {
        // invariant: computer player makes a makes a legal move
        Card c = controller_->compPlayerMove(); // card played/discarded by computer player
        if (table_->getCurPlayer()->getLegalPlays().empty()) {
            cout << "Player" << table_->getCurPlayer()->getId() << " discards " << face_names[c.face-1] << suit_names[c.suit] << "." << endl;
        } else {
            cout << "Player" << table_->getCurPlayer()->getId() << " plays " << face_names[c.face-1] << suit_names[c.suit] << "." << endl;
        }
        table_->nextPlayer(); 

    } else { // HUMAN PLAYER TURN
        cout << ">";
        string command;
        if (in->fail()) { in = &cin; }
        getline(*in, command);
        istringstream iss(command);
        iss >> command;
        
        if (command == "play") {
            string card;
            iss >> card;
            Faces cardFace = static_cast<Faces>(faces.at(card.at(0)));
            Suits cardSuit = static_cast<Suits>(suits.at(card.at(1)));
            try {
                controller_->playButtonClicked(Card{cardFace, cardSuit});
                cout << "Player" << table_->getCurPlayer()->getId() << " plays " << card << "." << endl;
                table_->nextPlayer(); 
            } catch (...) {
                cout << "This is not a legal play." << endl;
                nextCommand();
            }

        } else if (command == "discard") {
            string card;
            iss >> card;
            Faces cardFace = static_cast<Faces>(faces.at(card.at(0)));
            Suits cardSuit = static_cast<Suits>(suits.at(card.at(1)));
            try {
                controller_->discardButtonClicked(Card{cardFace, cardSuit});
                cout << "Player" << table_->getCurPlayer()->getId() << " discards " << card << "." << endl;
                table_->nextPlayer(); 
            } catch (...) {
                cout << "You have a legal play. You may not discard." << endl;
                nextCommand();
            }

        } else if (command == "deck") {
            // PRINT DECK
            int cardsInRow = 0;
            for (auto card : table_->getDeck().getCards()) {
                if (cardsInRow == 13) { 
                    cardsInRow = 0;
                    cout << endl; 
                }
                if (cardsInRow != 0) { cout << " "; }
                cout << face_names[card.face-1] << suit_names[card.suit];
                cardsInRow++;
            }
            cout << endl;
            nextCommand();

        } else if (command == "ragequit") {
            cout << "Player" << table_->getCurPlayer()->getId() << " ragequits. A computer will now take over." << endl;
            controller_->rageQuitButtonClicked();
            nextCommand();

        } else if (command == "quit") {
            controller_->quitButtonClicked();
            update();

        } else if (command == "") {
            if (in != &cin) { cout << endl; }
            nextCommand(); 

        } else {
            cout << "Invalid command. Please try again." << endl;
            nextCommand(); 
        }
    }
}
