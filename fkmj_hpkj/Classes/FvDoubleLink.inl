

//+--------------------------------------------------------------------------------------------------------------------------------------------
template <class TNode>
void FvDoubleLinkRoot<TNode>::Detach()
{
	if(IsAttach())
	{
		m_Pre->m_Next = m_Next;
		m_Next->m_Pre = m_Pre;
		m_Pre = NULL;
		m_Next = NULL;
	}
}
//+--------------------------------------------------------------------------------------------------------------------------------------------
//+--------------------------------------------------------------------------------------------------------------------------------------------
template <class T, template<class> class TNode>
FvDoubleLink<T, TNode>::FvDoubleLink()
{
	_Init();
}
//+------------------------------------------------------------------------------------------------------------
template <class T, template<class> class TNode>
FvDoubleLink<T, TNode>::~FvDoubleLink()
{
	Clear();
}
//+------------------------------------------------------------------------------------------------------------
template <class T, template<class> class TNode>
void FvDoubleLink<T, TNode>::PushBack(Node& kNode)
{
	kNode.Detach();
	_PushBefore(RootNode(), kNode);
}
template <class T, template<class> class TNode>
void FvDoubleLink<T, TNode>::PushFront(Node & kNode)
{
	kNode.Detach();
	_PushAfter(RootNode(), kNode);
}
template <class T, template<class> class TNode>
void FvDoubleLink<T, TNode>::PushBack(FvDoubleLink& kList)
{
	_PushBefore(RootNode(), kList);
}
template <class T, template<class> class TNode>
void FvDoubleLink<T, TNode>::PushFront(FvDoubleLink& kList)
{
	_PushAfter(RootNode(), kList);
}
//+------------------------------------------------------------------------------------------------------------
template <class T, template<class> class TNode>
void FvDoubleLink<T, TNode>::Clear()
{
	Node* pIter = RootNode().m_Next;
	while (pIter != &RootNode())
	{
		Node* pNext = pIter->m_Next;
		pIter->m_Pre = NULL;
		pIter->m_Next = NULL;
		pIter = pNext;
	}
	_Init();
}
//+------------------------------------------------------------------------------------------------------------
template <class T, template<class> class TNode>
int FvDoubleLink<T, TNode>::GetSize(const FvDoubleLink& kList)
{
	int uiSize = 0;
	Node* pIter = kList.RootNode().m_Next;
	while (pIter != &kList.RootNode())
	{
		Node* pNext = pIter->m_Next;
		++uiSize;
		pIter = pNext;
	}
	return uiSize;
}
//+------------------------------------------------------------------------------------------------------------
template <class T, template<class> class TNode>
void FvDoubleLink<T, TNode>::PushAfter(Node& qPreNode, Node& qNewNode)
{
	qNewNode.Detach();
	_PushAfter(qPreNode, qNewNode);
}
template <class T, template<class> class TNode>
void FvDoubleLink<T, TNode>::PushBefore(Node& qNextNode, Node& qNewNode)
{
	qNewNode.Detach();
	_PushBefore(qNextNode, qNewNode);
}
template <class T, template<class> class TNode>
void FvDoubleLink<T, TNode>::PushAfter(Node& qPreNode, FvDoubleLink& kList)
{
	if(qPreNode.IsAttach() == false)
	{
		return;
	}
	if(kList.IsEmpty())
	{
		return;
	}
	_PushAfter(qPreNode, kList);
}
template <class T, template<class> class TNode>
void FvDoubleLink<T, TNode>::PushBefore(Node& qNextNode, FvDoubleLink& kList)
{
	if(qNextNode.IsAttach() == false)
	{
		return;
	}
	_PushBefore(qNextNode, kList);
}
//+------------------------------------------------------------------------------------------------------------
template <class T, template<class> class TNode>
void FvDoubleLink<T, TNode>::_PushAfter(Node& qPreNode, Node& qNewNode)
{
	Node* pNext = qPreNode.m_Next;
	qPreNode.m_Next = &qNewNode;
	qNewNode.m_Next = pNext;
	qNewNode.m_Pre = &qPreNode;
	FV_ASSERT(pNext);
	pNext->m_Pre = &qNewNode;
}
template <class T, template<class> class TNode>
void FvDoubleLink<T, TNode>::_PushBefore(Node& qNextNode, Node& qNewNode)
{
	Node* pPre = qNextNode.m_Pre;
	qNextNode.m_Pre = &qNewNode;
	qNewNode.m_Pre = pPre;
	qNewNode.m_Next = &qNextNode;
	FV_ASSERT(pPre);
	pPre->m_Next = &qNewNode;
}
template <class T, template<class> class TNode>
void FvDoubleLink<T, TNode>::_PushAfter(Node& qPreNode, FvDoubleLink& kList)
{
	if(kList.IsEmpty())
	{
		return;
	}
	Node* pkFirst = kList.RootNode().m_Next;
	Node* pkBack = kList.RootNode().m_Pre;
	Node* pkNext = qPreNode.m_Next;
	_Link(qPreNode, *pkFirst);
	_Link(*pkBack, *pkNext);
	kList._Init();
}
template <class T, template<class> class TNode>
void FvDoubleLink<T, TNode>::_PushBefore(Node& qNextNode, FvDoubleLink& kList)
{
	if(kList.IsEmpty())
	{
		return;
	}
	Node* pkFirst = kList.RootNode().m_Next;
	Node* pkBack = kList.RootNode().m_Pre;
	Node* pkPre = qNextNode.m_Pre;
	_Link(*pkPre, *pkFirst);
	_Link(*pkBack, qNextNode);
	kList._Init();
}
//+------------------------------------------------------------------------------------------------------------
template <class T, template<class> class TNode>
void FvDoubleLink<T, TNode>::_Init()
{
	RootNode().m_Pre = &RootNode();
	RootNode().m_Next = &RootNode();
}
//+------------------------------------------------------------------------------------------------------------
template <class T, template<class> class TNode>
void FvDoubleLink<T, TNode>::_Link(Node& kPre, Node& kNext)
{
	kPre.m_Next = &kNext;
	kNext.m_Pre = &kPre;
}
//+------------------------------------------------------------------------------------------------------------
template <class T, template<class> class TNode>
template<class TVisitor, void (TVisitor::*func)(T&)>
void FvDoubleLink<T, TNode>::Each(TVisitor& qVisitor)const
{
	Iterator pCurrent = RootNode().m_Next;
	while (!IsEnd(pCurrent))
	{
		T& kContent = pCurrent->Content();
		pCurrent = pCurrent->m_Next;
		(qVisitor.*func)(kContent);
	}
}
template <class T, template<class> class TNode>
template<void (*func)(T&)>
void FvDoubleLink<T, TNode>::Each()const
{
	Iterator pCurrent = RootNode().m_Next;
	while (!IsEnd(pCurrent))
	{
		T& kContent = pCurrent->Content();
		pCurrent = pCurrent->m_Next;
		(*func)(kContent);
	}
}

//+--------------------------------------------------------------------------------------------------------------------------------------------
//+--------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
void FvDoubleLinkNode3<T>::Detach()
{
	if(IsAttach())
	{
		m_Pre->m_Next = m_Next;
		m_Next->m_Pre = m_Pre;
		m_Pre = this;
		m_Next = this;
	}
}template <class T>
void FvDoubleLinkNode3<T>::DetachAll()
{
	FvDoubleLinkNode3* iter = m_Next;
	while (iter != this)
	{
		FvDoubleLinkNode3* pNext = iter->m_Next;
		iter->m_Pre = iter;
		iter->m_Next = iter;
		iter = pNext;
	}
	m_Pre = this;
	m_Next = this;
}
//+------------------------------------------------------------------------------------------------------------
template <class T>
void FvDoubleLinkNode3<T>::PushAfter(FvDoubleLinkNode3& qPreNode, FvDoubleLinkNode3& qNewNode)
{
	qNewNode.Detach();
	_PushAfter(qPreNode, qNewNode);
}
template <class T>
void FvDoubleLinkNode3<T>::PushBefore(FvDoubleLinkNode3& qNextNode, FvDoubleLinkNode3& qNewNode)
{
	qNewNode.Detach();
	_PushBefore(qNextNode, qNewNode);
}
//+------------------------------------------------------------------------------------------------------------
template <class T>
void FvDoubleLinkNode3<T>::_PushAfter(FvDoubleLinkNode3& qPreNode, FvDoubleLinkNode3& qNewNode)
{
	FvDoubleLinkNode3* pNext = qPreNode.m_Next;
	qPreNode.m_Next = &qNewNode;
	qNewNode.m_Next = pNext;
	qNewNode.m_Pre = &qPreNode;
	FV_ASSERT(pNext);
	pNext->m_Pre = &qNewNode;
}
template <class T>
void FvDoubleLinkNode3<T>::_PushBefore(FvDoubleLinkNode3& qNextNode, FvDoubleLinkNode3& qNewNode)
{
	FvDoubleLinkNode3* pPre = qNextNode.m_Pre;
	qNextNode.m_Pre = &qNewNode;
	qNewNode.m_Pre = pPre;
	qNewNode.m_Next = &qNextNode;
	FV_ASSERT(pPre);
	pPre->m_Next = &qNewNode;
}
//+--------------------------------------------------------------------------------------------------------------------------------------------
//+--------------------------------------------------------------------------------------------------------------------------------------------