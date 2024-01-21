#ifndef DUR_DURVIEW_H
#define DUR_DURVIEW_H

#include "durModel.h"

static const char *const ranks = "6789XJQKA";
static const char *const suits = "scdh"; //Spades, Clubs, Diamonds, Hearts
static const char *const places = "LLRRa1a2a3a4a5a6d1d2d3d4d5d6PPDD";

void durView(sBoard *b);

#ifdef DUR_DEBUG   // [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [
void durDbgView_board(sBoard *b);
#endif //DUR_DEBUG // ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ]

#endif //DUR_DURVIEW_H
