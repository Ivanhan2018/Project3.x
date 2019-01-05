#ifndef _COMMUNITYCARDS_H
#define _COMMUNITYCARDS_H

#include <vector>

#include "Card.h"

class CommunityCards
{
public:
	CommunityCards();
	
	typedef enum {
		None=-1,
		Flop,
		Turn=3,
		River
	} Round;
	
	bool setFlop(Card c1, Card c2, Card c3);
	bool setTurn(Card c);
	bool setRiver(Card c);
	
	void clear() { cards.clear(); };
	
	void copyCards(std::vector<Card> *v) const { v->insert(v->end(), cards.begin(), cards.end()); };
	
	void debug();

	std::vector<Card> cards;
};

#endif /* _COMMUNITYCARDS_H */
