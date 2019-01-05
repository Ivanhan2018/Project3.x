#include "Strength.h"

using namespace std;

Strength::Strength()
{
	hold_cards = new HoleCards();
	community_cards = new CommunityCards();
	hand_strength = new HandStrength();
}

Strength::~Strength()
{
	delete hold_cards;
	delete community_cards;
	delete hand_strength;
}

int Strength::init()
{
	hold_cards->clear();
	community_cards->clear();
	return 0;
}

int Strength::set_hold_card(const char *c1, const char *c2)
{
	hold_cards->setCards(c1, c2);
	return 0;
}

int Strength::set_card_flop(const char *c1, const char *c2, const char *c3)
{
	community_cards->setFlop(c1, c2, c3);
	return 0;
}

int Strength::set_card_rurn(const char *c)
{
	community_cards->setTurn(c);
	return 0;
}

int Strength::set_card_river(const char *c)
{
	community_cards->setRiver(c);
	return 0;
}

int Strength::get_cards_rank()
{
	if (hold_cards->cards.size() != 2)
		return -1;
	if (community_cards->cards.size() < 3)
		return -2;

	GameLogic::getStrength(hold_cards, community_cards, hand_strength);
	ranking = hand_strength->getRanking();

	return ranking;
}
