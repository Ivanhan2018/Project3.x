#include "FvReferencePtr.h"

FvReferencePtrList::FvReferencePtrList()
{

}
FvReferencePtrList::~FvReferencePtrList()
{
	_ClearReference();
}
void FvReferencePtrList::_ClearReference()const
{
	FvDoubleLink<void*>::Iterator iter = m_ReferencePtrList.GetHead();
	while (!m_ReferencePtrList.IsEnd(iter))
	{
		(*iter).m_Content = 0;
		FvDoubleLink<void*>::Next(iter);
	}
	m_ReferencePtrList.Clear();
}
void FvReferencePtrList::_AddReference(FvDoubleLinkNode2<void*>& kNode)const
{
	m_ReferencePtrList.PushBack(kNode);
}
