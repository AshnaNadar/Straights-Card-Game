// textview.h

/*
 * used MVC example - Text-based view
 *
 *  Created by Gustavo Tondello on 29/06/20 based on GTKmm version.
 *  Copyright 2009, 2017, 2020 UW. All rights reserved.
 *
 */

#ifndef MVC_TEXTVIEW_H
#define MVC_TEXTVIEW_H

#include "observer.h"
#include <istream>
 
class Controller;
class Table;

class TextView : public Observer {
  public:
    TextView( Controller*, Table*, std::istream*);
    virtual ~TextView();
    virtual void update();	// Observer Pattern: concrete update() method

  private:
    // Observer Pattern: to access Model accessors without having to downcast subject
    Table *table_;
	
    // Strategy Pattern member (plus signal handlers)
    Controller *controller_;

    // User input:
    void nextCommand();

    std::istream *in;
}; // View

#endif
