#include "GameDebug.h"

using namespace std;

#if DEBUG

void print_cards(const char *name, vector<Card> *cards)
{
	fprintf(stderr, "[cards]: %s: [[ ", name);
	for (vector<Card>::iterator e = cards->begin(); e != cards->end(); e++)
		fprintf(stderr, "%s ", e->getName());
	
	fprintf(stderr, "]]\n");
}

#endif
