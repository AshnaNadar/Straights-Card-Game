//hardstrategy.h
#ifndef _HARDSTRATEGY_H
#define _HARDSTRATEGY_H

#include "strategy.h"

class HardStrategy : public Strategy {
    public:
        virtual ~HardStrategy();
        
    private:
        Card play(Computer *c) override;
};

#endif

