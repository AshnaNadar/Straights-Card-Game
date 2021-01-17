// easystrategy.h
#ifndef _EASYSTRATEGY_H
#define _EASYSTRATEGY_H

#include "strategy.h"

class EasyStrategy : public Strategy {
    public:
        virtual ~EasyStrategy();
        
    private:
        Card play(Computer *c) override;
};

#endif
