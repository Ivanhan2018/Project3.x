#ifndef _HOLECARDS_H
#define _HOLECARDS_H

#include <vector>

#include "Card.h"

class HoleCards
{
public:
	HoleCards();
	
	bool setCards(Card c1, Card c2);
	
	void clear() { cards.clear(); };
	
	void copyCards(std::vector<Card> *v) const { v->insert(v->end(), cards.begin(), cards.end()); };
	
	void debug();

	std::vector<Card> cards;
};

#endif /* _HOLECARDS_H */
