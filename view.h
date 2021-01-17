// view.h

/*
 * used MVC example of GTKmm program
 *
 *  Created by Jo Atlee on 06/07/09.
 *  Updated to GTKmm 3.0 (version 3.10.1) from 2.4 on February 13, 2017.
 *  Copyright 2009, 2017 UW. All rights reserved.
 *
 */

#ifndef MVC_VIEW_H
#define MVC_VIEW_H
 
#include <gtkmm.h>
#include "DeckGUI.h"
#include "observer.h"
#include <vector>
#include <string>

class Controller;
class Table;

class View : public Gtk::Window, public Observer {
  public:
    View( Controller*, Table* );
    virtual ~View();
    virtual void update();	// Observer Pattern: concrete update() method

  private:
    // Observer Pattern: to access Model accessors without having to downcast subject
    Table *table_;
	
    // Strategy Pattern member (plus signal handlers)
    Controller *controller_;

    // Card Images
    DeckGUI deck;

    // Legal Card Images
    DeckGUI legal;

    // Member widgets:
    std::vector<Gtk::Image*> clubPile;
    std::vector<Gtk::Image*> diamondPile;
    std::vector<Gtk::Image*> heartPile;
    std::vector<Gtk::Image*> spadePile;
    std::vector<std::vector<Gtk::Image*>> suitPiles;

    std::vector<Gtk::Image*> handCards;
    std::vector<Gtk::Button*> handButtons;

    std::vector<Gtk::Box*> playerInfo;
    std::vector<Gtk::Label*> discardInfo;
    std::vector<Gtk::Label*> scoreInfo;
    std::vector<Gtk::Button*> playerButtons;
    std::vector<Gtk::Frame*> player;  //Frame for player type/rage button, discard info, and score info

    Gtk::Box panels;
    Gtk::Box formatBox;   // Box for stacking buttons vertically (default layout is horizontal)
    Gtk::Box header;      // Box for Start Game, End Game, and Seed entry
    Gtk::Box handBox;     // Box for player hand cards

    Gtk::Box clubBox;     // Box for club suit pile
    Gtk::Box diamondBox;  // Box for diamond suit pile
    Gtk::Box heartBox;    // Box for heart suit pile
    Gtk::Box spadeBox;    // Box for spade suit pile

    Gtk::Box infoBox;     // Box for all player's info

    Gtk::Entry seed_entry;
    Gtk::Button start_button;
    Gtk::Button quit_button;

    int numOfCardsInHand;

    // Signal handlers:
    void startButtonClicked();
    void playerButtonClicked(int i);
    void cardButtonClicked(int i);
    void showDialog(std::string message);
    void quitButtonClicked();
}; // View

#endif
