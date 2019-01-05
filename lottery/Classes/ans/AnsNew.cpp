/*
** this module implement debug log printer
** author: zhouwei
** date: 2012.09.20
*/

#include <vector>
#include <list>
#include <algorithm>
#include "AnsMacros.h"
#include "AnsNew.h"

#if defined(_WIN32) && defined(__ANS_DEBUG)

namespace ans{
namespace debug{

struct NewTrack
{
	void* ptr;
	const char* file;
	unsigned line;
	bool operator==(const NewTrack& other) const
	{
		return (file == other.file || \
			strcmp(file, other.file) == 0) &&
			line == other.line;
	}
};

typedef std::list<NewTrack*> NewTrackList;
typedef NewTrackList::iterator NewTrackListIter;
typedef std::vector<const NewTrack*> NewTrackVec;
typedef NewTrackVec::const_iterator NewTrackVecIter;

static NewTrackList* _newTracks = NULL;

// -----------------------------------------------------------------------
void addNewTrack(void* p, const char* file, unsigned line)
{
	if(_newTracks == NULL)
		_newTracks = new std::list<NewTrack*>;
	NewTrack *pTrack = (NewTrack*)malloc(sizeof(NewTrack));
	pTrack->ptr = p;
	pTrack->file = file;
	pTrack->line = line;
	_newTracks->insert(_newTracks->begin(), pTrack);
}

void removeNewTrack(void* p)
{
	if(!_newTracks) return;
	NewTrackListIter iter = _newTracks->begin();
	while(iter != _newTracks->end())
	{
		if((*iter)->ptr == p)
		{
			free(*iter);
			_newTracks->remove(*iter);
			break;
		}
		++iter;
	}
}

// -------------------------------------------------------------
static bool hasCollected(NewTrackVec& tracks, const NewTrack& track)
{
	NewTrackVecIter iter = tracks.begin();
	while(iter != tracks.end())
	{
		if(*(*iter) == track)
			return true;
		++iter;
	}
	tracks.push_back(&track);
	return false;
}

void dumpUnfree(std::ofstream& s, bool undbl)
{
	if(!_newTracks) return;
	NewTrackVec passeds;
	char buff[512];
	NewTrackListIter iter = _newTracks->begin();
	while(iter != _newTracks->end())
	{
		if(!undbl || !hasCollected(passeds, *(*iter)))
		{
			ANS_SNPRINTF(buff, sizeof(buff), "unfree pointer: %s(%d)\n", 
				(*iter)->file, (*iter)->line);
			s << buff;
		}
		++iter;
	}
}

std::string getUnfreeInfo(bool undbl)
{
	if(!_newTracks) return "";
	NewTrackVec passeds;
	std::string result;
	char buff[512];
	NewTrackListIter iter = _newTracks->begin();
	while(iter != _newTracks->end())
	{
		if(undbl && hasCollected(passeds, *(*iter)))
		{
			ANS_SNPRINTF(buff, sizeof(buff), "unfree pointer: %s(%d)\n", 
				(*iter)->file, (*iter)->line);
			result.append(buff);
		}
		++iter;
	}
	return result;
}

}}

#endif // __ANS_DEBUG