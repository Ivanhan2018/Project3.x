#ifndef _STRENGTH_H
#define _STRENGTH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>

#include "Deck.h"
#include "HoleCards.h"
#include "GameLogic.h"
#include "CommunityCards.h"

class Strength {
public:
	int ranking;

private:
	HoleCards *hold_cards;
	CommunityCards *community_cards;
	HandStrength *hand_strength;

public:
	Strength();
	~Strength();

	int init();
	int set_hold_card(const char *c1, const char *c2);
	int set_card_flop(const char *c1, const char *c2, const char *c3);
	int set_card_rurn(const char *c);
	int set_card_river(const char *c);
	int get_cards_rank();

};

#endif
