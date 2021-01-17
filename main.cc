/*
 * Used MVC example of GTKmm program
 *
 *  Created by Jo Atlee on 06/07/09.
 *  Updated to GTKmm 3.0 (version 3.10.1) from 2.4 on February 13, 2017.
 *  Copyright 2009, 2017 UW. All rights reserved.
 *
 */

#include "table.h"
#include "controller.h"
#include "textview.h"
#include "view.h"

#include <gtkmm/application.h> // Bring in Gtk::Application
#include <iostream>
#include <fstream>
#include <istream>
#include <chrono>
#include <string>

int main( int argc, char * argv[] ) {    
	// use a time-based seed for the default seed value
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();

    std::string command;
    std::ifstream inputFile;
    std::istream *in = &std::cin;

	if ( argc > 1 ) {
        command = argv[1];

        try {
            // invariant: a maximum of 2 arguments can be used.
            if (argc > 2) { command = argv[2]; } // seed will be last argument
			seed = std::stoi( std::string{ command } );
            command = argv[1];
		} catch(...) {}

        std::string argFile = command + ".args";
        std::string inFile = command + ".in";

        // invariant: state .args file must contain a seed.
        inputFile.open(argFile);
        if (inputFile) { 
            inputFile >> command; // set command from state file
            if (!(inputFile >> seed)) { seed = std::stoi( std::string{ command } ); } // if only one argument in state file, set seed
        } 
        inputFile.close();

        inputFile.open(inFile);
        if (inputFile.is_open()) { in = &inputFile; } // set input stream to state .in file, if any
	} 

    // GUI ENHANCEMENT
    if (( argc > 1 ) && ( command == "-gui")) { 
        auto app = Gtk::Application::create();
        Table      table(seed);		            // Create model
        Controller controller( &table );        // Create controller
        View       view( &controller, &table ); // Create the view -- is passed handle to controller and model.
        
        return app->run( view );                // Show the window and return when it is closed.

    // HOUSE RULES ENHANCEMENT
    } else if (( argc > 1 ) && ( command == "-houserules" )) {
        Table      table(seed, 45, 2, Faces::KING, Suits::HEART);   // Create model
        Controller controller( &table );            // Create controller
        TextView   view( &controller, &table, in ); // Create the view -- is passed handle to controller, model, and input stream.
                                                    // The view's constructor will start the UI
    // EASY COMPUTER PLAYERS ENHANCEMENT
    } else if (( argc > 1 ) && ( command == "-easy") ) {
        Table      table(seed, 80, 4, Faces::SEVEN, Suits::SPADE, Difficulty::EASY);    // Create model
        Controller controller( &table );            // Create controller
        TextView   view( &controller, &table, in ); // Create the view -- is passed handle to controller, model, and input stream.
                                                    // The view's constructor will start the UI
    // HARD COMPUTER PLAYERS ENHANCEMENT
    } else if (( argc > 1 ) && ( command == "-hard") ) { 
        Table      table(seed, 80, 4, Faces::SEVEN, Suits::SPADE, Difficulty::HARD);    // Create model
        Controller controller( &table );            // Create controller
        TextView   view( &controller, &table, in ); // Create the view -- is passed handle to controller, model, and input stream.
                                                    // The view's constructor will start the UI
    // DYNAMIC COMPUTER PLAYERS ENHANCEMENT
    } else if (( argc > 1 ) && ( command == "-dynamic")) { 
        Table      table(seed, 80, 4, Faces::SEVEN, Suits::SPADE, Difficulty::MEDIUM, true);    // Create model
        Controller controller( &table );            // Create controller
        TextView   view( &controller, &table, in ); // Create the view -- is passed handle to controller, model, and input stream.
                                                    // The view's constructor will start the UI
    // NO ENHANCEMENTS
    } else {
        Table      table(seed);		                // Create model
        Controller controller( &table );            // Create controller
        TextView   view( &controller, &table, in ); // Create the view -- is passed handle to controller, model, and input stream.
                                                    // The view's constructor will start the UI
    }
    inputFile.close();
} // main
