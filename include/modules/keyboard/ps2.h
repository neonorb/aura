#ifndef PS2_H_
#define PS2_H_

#include <int.h>

#include <modules/keyboard/keyboard.h>

static uint8 getScancode();

static void readKeyboard();

void ps2_interrupt();

#endif /* PS2_H_ */
