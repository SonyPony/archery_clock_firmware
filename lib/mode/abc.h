#ifndef ABC_H
#define ABC_H

#include <lib/mode/base.h>
#include <lib/data/data_model.h>
#include "ab.h"


class AbcModeData: public AbModeData 
{
    public:
        AbcModeData();

        void log() const override;
        bool display(DisplayState* displayState) const override;
};

#endif // ABC_H
