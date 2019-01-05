#include "GameDebug.h"
#include "CommunityCards.h"

using namespace std;

CommunityCards::CommunityCards()
{

}

bool CommunityCards::setFlop(Card c1, Card c2, Card c3)
{
	cards.clear();
	
	cards.push_back(c1);
	cards.push_back(c2);
	cards.push_back(c3);
	
	return true;
}

bool CommunityCards::setTurn(Card c)
{
	if (cards.size() != 3)
		return false;
	
	cards.push_back(c);
	
	return true;
}

bool CommunityCards::setRiver(Card c)
{
	if (cards.size() != 4)
		return false;
	
	cards.push_back(c);
	
	return true;
}

void CommunityCards::debug()
{
	print_cards("Community", &cards);
}
