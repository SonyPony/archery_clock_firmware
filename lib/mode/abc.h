#ifndef ABC_H
#define ABC_H

#include "base.h"
#include <data_model.h>
#include "ab.h"

typedef AbModeData AbcModeData;

AbcModeData *abc_mode_init(InitializationCommand *init_data);
void abc_mode_print(AbcModeData *mode_data);
void abc_mode_display(DisplayState *display, AbcModeData *mode_data);

#endif // ABC_H
