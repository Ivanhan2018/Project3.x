//{future header message}
#pragma once

#include "FvKernel.h"
#include "FvDebug.h"
//+------------------------------------------------------------------------------------------------------------

template <class TNode>
class FvDoubleLinkRoot
{
	FV_NOT_COPY_COMPARE(FvDoubleLinkRoot);
public:
	bool IsAttach()const{return (m_Pre != NULL);}
	void Detach();

	TNode* Pre()const{return m_Pre;}
	TNode* Next()const{return m_Next;}

protected:
	FvDoubleLinkRoot():m_Pre(NULL), m_Next(NULL){}
	~FvDoubleLinkRoot(){Detach();}

private:

	TNode* m_Pre;
	TNode* m_Next;

	template <class, template<class >class> friend class FvDoubleLink;
};
//+------------------------------------------------------------------------------------------------------------
template <class T>
class FvDoubleLinkNode1: public FvDoubleLinkRoot<FvDoubleLinkNode1<T> >
{
public:
	FvDoubleLinkNode1(){}
	~FvDoubleLinkNode1(){}

	T Content(){ return static_cast<T>(this); }
	template <class, template<class >class> friend class FvDoubleLink;
};
template <class T>
class FvDoubleLinkNode2: public FvDoubleLinkRoot<FvDoubleLinkNode2<T> >
{
public:
	FvDoubleLinkNode2(const T& kT = T()):m_Content(kT){}
	template<class U>
	FvDoubleLinkNode2(U& kU):m_Content(kU){}
	~FvDoubleLinkNode2(){}

	T& Content(){ return m_Content;}
	T m_Content;
	template <class, template<class >class> friend class FvDoubleLink;
};
//+------------------------------------------------------------------------------------------------------------
template <class T, template<class> class TNode = FvDoubleLinkNode2>
class FvDoubleLink
{
	FV_NOT_COPY_COMPARE(FvDoubleLink);
public:
	typedef TNode<T> Node;
	typedef Node* Iterator;

	FvDoubleLink();
	~FvDoubleLink();

	void PushBack(Node& kNode);
	void PushFront(Node & kNode);
	void PushBack(FvDoubleLink& kList);
	void PushFront(FvDoubleLink & kList);
	void Clear();

	static void PushAfter(Node& qPreNode, Node& qNewNode);
	static void PushBefore(Node& qNextNode, Node& qNewNode);
	static void PushAfter(Node& qPreNode, FvDoubleLink& kList);
	static void PushBefore(Node& qNextNode, FvDoubleLink& kList);

	bool IsEmpty()const{ return (RootNode().m_Next == &RootNode());}
	bool IsNotEmpty()const{return !IsEmpty();}
	static int GetSize(const FvDoubleLink& kList);

	bool IsEnd(const Iterator iter)const{return (iter == &RootNode());}

	Iterator GetHead()const{return RootNode().m_Next;}
	static Iterator Next(Iterator& iter)
	{
		FV_ASSERT(iter && iter->m_Next);
		return iter = iter->m_Next;
	}

	Iterator GetTail()const{return RootNode().m_Pre;}
	static Iterator Pre(Iterator& iter)
	{
		FV_ASSERT(iter && iter->m_Pre);
		return iter = iter->m_Pre;
	}

	template<class TVisitor, void (TVisitor::*func)(T&)>
	void Each(TVisitor& qVisitor)const;
	template<void (*func)(T&)>
	void Each()const;

private:

	static void _PushAfter(Node& qPreNode, Node& qNewNode);
	static void _PushBefore(Node& qNextNode, Node& qNewNode);
	static void _PushAfter(Node& qPreNode, FvDoubleLink& kList);
	static void _PushBefore(Node& qNextNode, FvDoubleLink& kList);
	static void _Link(Node& kPre, Node& kNext);

	void _Init();

	Node& RootNode(){return static_cast<Node&>(m_kRoot);}	
	const Node& RootNode()const{return static_cast<const Node&>(m_kRoot);}	
	FvDoubleLinkRoot<Node> m_kRoot;
};

//+------------------------------------------------------------------------------------------------------------
//+------------------------------------------------------------------------------------------------------------
template <class T>
class FvDoubleLinkNode3
{
	FV_NOT_COPY_COMPARE(FvDoubleLinkNode3);
public:

	FvDoubleLinkNode3()
	{
		m_Pre = this;
		m_Next = this;
	}
	~FvDoubleLinkNode3(){Detach();}

	bool IsAttach()const{return (m_Pre != this);}
	void Detach();
	void DetachAll();

	static void PushAfter(FvDoubleLinkNode3& qPreNode, FvDoubleLinkNode3& qNewNode);
	static void PushBefore(FvDoubleLinkNode3& qNextNode, FvDoubleLinkNode3& qNewNode);

	FvDoubleLinkNode3* GetPre()const{return m_Pre;}
	FvDoubleLinkNode3* GetNext()const{return m_Next;}

	T m_Content;

private:

	static void _PushAfter(FvDoubleLinkNode3& qPreNode, FvDoubleLinkNode3& qNewNode);
	static void _PushBefore(FvDoubleLinkNode3& qNextNode, FvDoubleLinkNode3& qNewNode);

	FvDoubleLinkNode3* m_Pre;
	FvDoubleLinkNode3* m_Next;
};

//+------------------------------------------------------------------------------------------------------------
template<class T>
class FvDoubleLinkNameSpace
{
public:
	typedef FvDoubleLink<T*, FvDoubleLinkNode1> List1;
	typedef FvDoubleLink<T*, FvDoubleLinkNode2> List2;
};

#include "FvDoubleLink.inl"