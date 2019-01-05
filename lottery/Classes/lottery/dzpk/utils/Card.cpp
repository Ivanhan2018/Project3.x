#include "Card.h"


static const char face_symbols[] = {
	'2', '3', '4', '5', '6', '7', '8', '9',
	'T', 'J', 'Q', 'K', 'A'
};

static const char suit_symbols[] = {
	'c', 'd', 'h', 's'
};


Card::Card()
{
	face = Card::FirstFace;
	suit = Card::FirstSuit;
}

Card::Card(Face f, Suit s)
{
	face = f;
	suit = s;
}

Card::Card(const char *str)
{
	face = convertFaceSymbol(str[0]);
	suit = convertSuitSymbol(str[1]);
}

void Card::setCard(const char *str)
{
	face = convertFaceSymbol(str[0]);
	suit = convertSuitSymbol(str[1]);
}

void Card::getValue(Face *f, Suit *s) const
{
	if (f)
		*f = face;
	
	if (s)
		*s = suit;
}

char Card::getFaceSymbol() const
{	
	return face_symbols[face - Card::FirstFace];
}

char Card::getSuitSymbol() const
{
	return suit_symbols[suit - Card::FirstSuit];
}

const char* Card::getName() const
{
	static char card_name[3];
	
	card_name[0] = getFaceSymbol();
	card_name[1] = getSuitSymbol();
	card_name[2] = '\0';
	
	return card_name;
}

Card::Face Card::convertFaceSymbol(char fsym)
{
	for (unsigned int i=Card::FirstFace; i <= Card::LastFace; i++)
		if (fsym == face_symbols[i - Card::FirstFace])
			return (Card::Face)i;
		
	return Card::FirstFace;
}

Card::Suit Card::convertSuitSymbol(char ssym)
{
	for (unsigned int i=Card::FirstSuit; i <= Card::LastSuit; i++)
		if (ssym == suit_symbols[i - Card::FirstSuit])
			return (Card::Suit)i;
		
	return Card::FirstSuit;
}
