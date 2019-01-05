#ifndef _DECK_H
#define _DECK_H

#include <vector>

#include "Card.h"

class Deck
{
public:
	void fill();
	void empty();
	int count() const;
	
	bool push(Card card);
	bool pop(Card &card);
	bool shuffle(int seed);
	
	void debugRemoveCard(Card card);
	void debugPushCards(const std::vector<Card> *cardsvec);
	void debug();
	
private:
	std::vector<Card> cards;
};

#endif /* _DECK_H */
