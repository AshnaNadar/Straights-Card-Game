// view.cc

/*
 * used MVC example of GTKmm program
 *
 *  Created by Jo Atlee on 06/07/09.
 *  Updated to GTKmm 3.0 (version 3.10.1) from 2.4 on February 13, 2017.
 *  Copyright 2009, 2017 UW. All rights reserved.
 *
 */

#include "observer.h"
#include "view.h"
#include "controller.h"
#include "table.h"
#include "subject.h"
#include "DeckGUI.h"

#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>

using std::string;
using std::istringstream;
using std::vector;

// Creates buttons with labels. Sets formatBox elements to have the same size, with 0 pixels between widgets
View::View(Controller *c, Table *t) : table_{t}, controller_{c}, panels{},
    formatBox{ Gtk::ORIENTATION_VERTICAL, 0 }, // box orientation and space between children in pixels
    start_button{ "Start Game" }, quit_button{ "End Game" }
{
    // Sets some properties of the window.
    set_title( "CS246 Straights Final" );   
	set_default_size( 250, 100 ); // width, height in pixels
	
    // Add panel to the window
    add( panels );

    // Add format box to the panel box
    panels.pack_start( formatBox );

    formatBox.pack_start( header );
    formatBox.pack_start( clubBox );
    formatBox.pack_start( diamondBox );
    formatBox.pack_start( heartBox );
    formatBox.pack_start( spadeBox );
    formatBox.pack_start( infoBox );
    formatBox.pack_start( handBox );
    
    // Add buttons and entry widget to the header box
    header.pack_start( start_button, Gtk::PACK_EXPAND_WIDGET);
    header.pack_start( seed_entry, Gtk::PACK_EXPAND_WIDGET);
    header.pack_start( quit_button, Gtk::PACK_EXPAND_WIDGET);

    seed_entry.set_placeholder_text("enter the game seed here");
    seed_entry.set_alignment(0.5);

    // initialize a player frame, button, and labels in player info box for each player
    for (int i = 0; i < table_->getPlayerCount(); i++) {
        playerInfo.push_back(Gtk::manage(new Gtk::Box( Gtk::ORIENTATION_VERTICAL, 5 )));
        discardInfo.push_back(Gtk::manage(new Gtk::Label("0 discards")));
        scoreInfo.push_back(Gtk::manage(new Gtk::Label("0 points")));
        playerButtons.push_back(Gtk::manage(new Gtk::Button("Human")));

        player.push_back(Gtk::manage(new Gtk::Frame("Player" + std::to_string(i+1))));
        player.at(i)->add(*playerInfo.at(i));

        playerInfo.at(i)->pack_start(*playerButtons.at(i), Gtk::PACK_EXPAND_WIDGET);
        playerInfo.at(i)->pack_start(*scoreInfo.at(i), Gtk::PACK_EXPAND_WIDGET);
        playerInfo.at(i)->pack_start(*discardInfo.at(i), Gtk::PACK_EXPAND_WIDGET);

        infoBox.pack_start(*player.at(i), Gtk::PACK_EXPAND_WIDGET);
        infoBox.set_homogeneous(true);
    }

    // initialize club suit pile
    for (int i = 0; i < 13; i++) {
        clubPile.push_back(Gtk::manage(new Gtk::Image()));
        clubBox.pack_start(*clubPile.at(i), Gtk::PACK_EXPAND_WIDGET);
        clubPile.at(i)->set( deck.null() );
    }

    // initialize diamond suit pile
    for (int i = 0; i < 13; i++) {
        diamondPile.push_back(Gtk::manage(new Gtk::Image()));
        diamondBox.pack_start(*diamondPile.at(i), Gtk::PACK_EXPAND_WIDGET);
        diamondPile.at(i)->set( deck.null() );
    }

    // initialize heart suit pile
    for (int i = 0; i < 13; i++) {
        heartPile.push_back(Gtk::manage(new Gtk::Image()));
        heartBox.pack_start(*heartPile.at(i), Gtk::PACK_EXPAND_WIDGET);
        heartPile.at(i)->set( deck.null() );
    }

    // initialize spade suit pile
    for (int i = 0; i < 13; i++) {
        spadePile.push_back(Gtk::manage(new Gtk::Image()));
        spadeBox.pack_start(*spadePile.at(i), Gtk::PACK_EXPAND_WIDGET);
        spadePile.at(i)->set( deck.null() );
    }

    suitPiles.push_back(clubPile);
    suitPiles.push_back(diamondPile);
    suitPiles.push_back(heartPile);
    suitPiles.push_back(spadePile);


    // initialize images for cards in player's hand
    for (int i = 0; i < 13; i++) {
        handCards.push_back(Gtk::manage(new Gtk::Image()));
        handCards.at(i)->set( deck.null() );
    }

    // initialize buttons for each card in player's hand
    for (int i = 0; i < 13; i++) {
        handButtons.push_back(Gtk::manage(new Gtk::Button()));
        handBox.pack_start(*handButtons.at(i), Gtk::PACK_EXPAND_WIDGET);
        handButtons.at(i)->set_image( *handCards.at(i) );
        handButtons.at(i)->set_sensitive(false);
    }

    // Associate button "clicked" events with local onButtonClicked() method defined below.
    start_button.signal_clicked().connect( sigc::mem_fun( *this, &View::startButtonClicked));
    quit_button.signal_clicked().connect( sigc::mem_fun( *this, &View::quitButtonClicked));

    for (int i = 0; i < table_->getPlayerCount(); i++) { 
        playerButtons.at(i)->signal_clicked().connect([this, i]() { playerButtonClicked(i); });
    }

    for (int i = 0; i < 13; i++) {
        handButtons.at(i)->signal_clicked().connect([this, i]() { cardButtonClicked(i); });
    }
	
    // The final step is to display the buttons (they display themselves).
    show_all();

    // Register view as observer of model.
    table_->subscribe(this);
} // View::View

View::~View() {}

const char face_names[] = { 'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K' };
const char suit_names[] = { 'C', 'D', 'H', 'S' };
const std::string suit_pile_names[] = { "Clubs", "Diamonds", "Hearts", "Spades" };

void View::showDialog(string message) {
    Gtk::MessageDialog dialogWindow(*this, "", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
    dialogWindow.set_message(message);
    dialogWindow.set_default_response(Gtk::RESPONSE_OK);
    dialogWindow.run();
}

void View::update() {
    if (table_->getDeck().getCards().size() == table_->getTurnCount()) { // GAME/ROUND ENDS

        // disable making a play
        for (int i = 0; i < 13; i++) {
        handButtons.at(i)->set_sensitive(false);
        }

        // DISPLAY PLAYERS DISCARDS & SCORES
        string message;

        for (auto player : table_->getPlayers()) {
            // disable rage quit buttons
            playerButtons.at(player->getId() - '0' - 1)->set_sensitive(false);

            scoreInfo.at(player->getId() - '0' - 1)->set_label(std::to_string(player->getRoundScore() + player->getGameScore()) + " points");

            message += "Player" + std::to_string(player->getId() - '0') + "'s discards: ";
            for (auto card : player->getDiscards()) {
                message.push_back(face_names[card.face-1]);
                message.push_back(suit_names[card.suit]);
                message += " ";
            } 
            message += "\n";
            message += "Player" + std::to_string(player->getId() - '0') + "'s score: " + std::to_string(player->getGameScore()) + 
                " + " + std::to_string(player->getRoundScore()) + " = " + std::to_string(player->getRoundScore() + player->getGameScore()) + "\n";
        }
        showDialog(message);
 
        if (!table_->getWinners().empty()) { // GAME ENDS
            // DISPLAY WINNER(S)
            message.clear();
            for (auto player : table_->getWinners()) {
                message += "Player" + std::to_string(player->getId() - '0') + " wins!\n";
            }
            showDialog(message);
        }
    } else { // GAME CONTINUES

        if (table_->getTurnCount() == 0) { // NEW ROUND
            string message = "A new round begins. It's Player" + std::to_string(table_->getCurPlayer()->getId() - '0') + "'s turn to play.";

            showDialog(message);

            // update player scores
            for (auto player : table_->getPlayers() ) { scoreInfo.at(player->getId() - '0' - 1)->set_label(std::to_string(player->getGameScore()) + " points"); }
        }

        // set curPlayer's Rage Quit button to be active and update player discard counts
        for ( auto player : table_->getPlayers() ) {
             if ((player->getId() == table_->getCurPlayer()->getId()) && !(table_->getCurPlayer()->getIsComp())) {
                playerButtons.at(player->getId() - '0' - 1)->set_sensitive(true);
            } else { playerButtons.at(player->getId() - '0' - 1)->set_sensitive(false); }

            discardInfo.at(player->getId() - '0' - 1)->set_label(std::to_string(player->getDiscards().size()) + " discards");
        }

        // display suitpiles
        for (int s = 0; s < 4; s++) {
            Suits suit = static_cast<Suits>(s);

            for (int f = 0; f < 13; f++) {
                Faces face = static_cast<Faces>(f + 1);
                Card suitCard = Card{face, suit};

                if (table_->getSuitPiles().at(s)->hasCard(suitCard)) {
                    suitPiles.at(s).at(f)->set( deck.image(face, suit) );
                } else {
                    suitPiles.at(s).at(f)->set( deck.null() );
                }
            }
        }

        // display cards in hand
        for (int i = 0; i < 13; i++) {
            if (table_->getCurPlayer()->getHand().size() > (unsigned int)i) {
                handButtons.at(i)->set_sensitive(true);

                Card curCard = table_->getCurPlayer()->getHand().at(i);
                vector<Card> legalCards = table_->getCurPlayer()->getLegalPlays();

                if (std::find(legalCards.begin(), legalCards.end(), curCard) != legalCards.end()) {
                    handCards.at(i)->set( deck.legalImage(curCard.face, curCard.suit) );

                } else { handCards.at(i)->set( deck.image(curCard.face, curCard.suit) ); }
                
            } else { handCards.at(i)->set( deck.null() ); }
        }

        if (table_->getCurPlayer()->getIsComp()) { // COMPUTER TURN
            controller_->compPlayerMove();
            table_->nextPlayer();
        }
    }
}

void View::playerButtonClicked(int i) {
    if (playerButtons.at(i)->get_label() == "Human") {
        playerButtons.at(i)->set_label("Computer");
    } else if (playerButtons.at(i)->get_label() == "Computer") {
        playerButtons.at(i)->set_label("Human");
    } else {
        playerButtons.at(table_->getCurPlayer()->getId() - '0' - 1)->set_sensitive(false);
        controller_->rageQuitButtonClicked();
        update();
    }
}

void View::startButtonClicked() {
    start_button.set_sensitive(false);
    seed_entry.set_sensitive(false);

    istringstream iseed{ seed_entry.get_text() };
    unsigned int seed;

    if (iseed >> seed) {
        table_->setSeed(seed);
    }

    int startingPlayer = table_->getCurPlayer()->getId() - '0' - 1;
    for (int i = 0; i < table_->getPlayerCount(); i++) {
        table_->setCurPlayer(table_->getPlayers().at(i));
        if (playerButtons.at(i)->get_label() == "Computer") { controller_->compPlayerButtonClicked(); }
        playerButtons.at(i)->set_label("Rage Quit");
    }
    table_->setCurPlayer(table_->getPlayers().at(startingPlayer));

    // enable making a play
    for (int i = 0; i < 13; i++) {
    handButtons.at(i)->set_sensitive(true);
    }

    update();
} // View::startButtonClicked

void View::cardButtonClicked(int i) {
    Card clickedCard = table_->getCurPlayer()->getHand().at(i);
    try {
        controller_->playButtonClicked(clickedCard);
        table_->nextPlayer();
    } catch (...) {
        try {
            controller_->discardButtonClicked(clickedCard);
            table_->nextPlayer();
        } catch (...) {}
    }
} // View::cardButtonClicked

void View::quitButtonClicked() {
    destroy_();
} // View::quitButtonClicked
