#ifndef DUR_DURVIEW_H
#define DUR_DURVIEW_H

#include "durModel.h"

static const char *const ranks = "6789XJQKA";
static const char *const suits = "scdh"; //Spades, Clubs, Diamonds, Hearts
static const char *const places = "LLRRaaddPPDD";

void durView(sBoard *b);
void durView_msg(char *s);

#ifdef DUR_DEBUG   // [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [
void durView_dbg_board(const sBoard *b);
#endif //DUR_DEBUG // ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ]

#endif //DUR_DURVIEW_H
