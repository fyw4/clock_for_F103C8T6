/*
 * Dino.h
 *
 *  Created on: Oct 4, 2025
 *      Author: WangQingChuan
 */

#ifndef INC_DINO_H_
#define INC_DINO_H_

#include "main.h"
#include "oled.h"

#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

extern const Image dino_crashedImg;
extern const Image dino_front_legImg;
extern const Image dino_back_legImg;
extern const Image dino_jumpsImg;
extern const Image tree_smallImg;
extern const Image tree_bigImg;

void dino_play();
void introMessage();
void show_line();

#endif /* INC_DINO_H_ */
