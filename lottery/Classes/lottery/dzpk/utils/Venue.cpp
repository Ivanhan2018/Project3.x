#include "Venue.h"

Venue::Venue():
room_id(0),
table_id(0),
ip("0.0.0.0"),
port(0),
sblind(0),
bblind(0),
min_money(0),
max_money(0),
seats(0),
max_member(0),
cur_member(0),
limit(0),
showVenueId(0),
quickRefuelItems("10,20,30,40,50")
{

}

Venue::~Venue()
{

}
