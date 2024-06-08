#ifndef ABC_H
#define ABC_H

#include "base.h"
#include <data_model.h>
#include "ab.h"

typedef AbModeData AbcModeData;

void abc_mode_init(AbcModeData *mode_data, BaseModeData *base_mode_data, InitializationCommand *init_data);
void abc_mode_print(AbcModeData *mode_data);
void abc_mode_display(DisplayState *display, AbcModeData *mode_data);

#endif // ABC_H
