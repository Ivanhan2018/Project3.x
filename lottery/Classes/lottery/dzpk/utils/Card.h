#ifndef _CARD_H
#define _CARD_H

class Card
{
public:
	typedef enum {
		Two=2,
		Three,
		Four,
		Five,
		Six,
		Seven,
		Eight,
		Nine,
		Ten,
		Jack,
		Queen,
		King,
		Ace,
		
		FirstFace=Two,
		LastFace=Ace
	} Face;
	
	typedef enum {
		Clubs=1,
		Diamonds,
		Hearts,
		Spades,
		
		FirstSuit=Clubs,
		LastSuit=Spades
	} Suit;
	
	Card();
	Card(Face f, Suit s);
	Card(const char *str);
	
	void setCard(const char *str);

	void getValue(Face *f, Suit *s) const;
	Face getFace() const { return face; };
	Suit getSuit() const { return suit; };
	
	char getFaceSymbol() const;
	char getSuitSymbol() const;
	const char* getName() const;
	
	bool operator <  (const Card &c) const { return (getFace() < c.getFace()); };
	bool operator >  (const Card &c) const { return (getFace() > c.getFace()); };
	bool operator == (const Card &c) const { return (getFace() == c.getFace()); };
	
	static Face convertFaceSymbol(char fsym);
	static Suit convertSuitSymbol(char ssym);

private:
	Face face;
	Suit suit;
};


#endif /* _CARD_H */
