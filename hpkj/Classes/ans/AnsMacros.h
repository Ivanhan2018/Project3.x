/*
** implement macros for generating code in class
** author: zhouwei
** date: 2012.06.05
*/

#ifndef __BRICK_MACRO_H__
#define __BRICK_MACRO_H__

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

// -------------------------------------------------------------
#define ANS_EXIT_IF(cnd) do{ if ((cnd)) goto exit0; } while(0)

#define ANS_SCOPE_BEGIN() do{
#define ANS_SCOPE_BREAK_IF(cnd) if(cnd) break
#define ANS_SCOPE_END() }while(0);

#define ANS_SAFE_DELETE(p) do{delete (p); (p) = NULL;}while(0)
#define ANS_SAFE_DELETES(p) do{delete [](p); (p) = NULL;}while(0)


#if defined(_MSC_VER) && (_MSC_VER >= 1200)
	// Microsoft visual studio, version 6 and higher.
	#define ANS_SNPRINTF _snprintf
#else
	#define ANS_SNPRINTF snprintf
#endif	

// -----------------------------------------------------------------------
// dispatch handle
// -----------------------------------------------------------------------
#define ANS_DISPATCH_BEGIN(caseRes) \
	switch(caseRes) {

#define ANS_DISPATCH_HANDLE(caseID, handler, ...) \
	case caseID: \
		handler(##__VA_ARGS__); \
		break;

#define ANS_DISPATCH_END() \
	default: \
		break; \
	}

#define ANS_RET_DISPATCH_BEGIN(caseRes) \
	switch(caseRes) {

#define ANS_RET_DISPATCH_HANDLE(caseID, handler, ...) \
	case caseID: \
		ret = handler(##__VA_ARGS__); \
		break;

#define ANS_RET_DISPATCH_END(code) \
	default: \
		ret = code; \
	}

// ---------------------------------------------------------------------------------
// inner typedef
// ---------------------------------------------------------------------------------
// -----------------------------------------------------------------------
// widget array
// -----------------------------------------------------------------------
#define ANS_TYPEDEF_VECTOR_INSTS(eType, name) \
	typedef std::vector<eType> name; \
	typedef std::vector<eType>& R##name; \
	typedef const vector<eType> C##name; \
	typedef const vector<eType>& CR##name; \
	typedef name::iterator name##Iter; \
	typedef name::const_iterator name##CIter; \
	typedef name::reverse_iterator name##RIter;

#define ANS_TYPEDEF_LIST_INSTS(eType, name) \
	typedef std::list<eType> name; \
	typedef std::list<eType>& R##name; \
	typedef const std::list<eType> C##name; \
	typedef const std::list<eType>& CR##name; \
	typedef name::iterator name##Iter; \
	typedef name::const_iterator name##CIter; \
	typedef name::reverse_iterator name##RIter;

#define ANS_TYPEDEF_MAP_INSTS(kType, vType, name) \
	typedef std::map<kType, vType> name; \
	typedef std::map<kType, vType>& R##name; \
	typedef const std::map<kType, vType> C##name; \
	typedef const std::map<kType, vType>& CR##name; \
	typedef name::iterator name##Iter; \
	typedef name::const_iterator name##CIter; 


// ---------------------------------------------------------------------------------
// properties
// ---------------------------------------------------------------------------------
// -----------------------------------------------------------------------
// read/write properties
// -----------------------------------------------------------------------
// readable/writable property declaration
#define ANS_RW_PROPERTY_DEC(acc, propType, memName, funName, varType)\
	acc: propType memName;\
	public: propType funName() const;\
	public: void funName(varType);
#define ANS_RW_PROPERTY_PREFIX_DEC(acc, propType, memName, funName, varType)\
	acc: propType memName;\
	public: propType get##funName() const;\
	public: void set##funName(varType);

// readable/writable virtual property declaration
#define ANS_RW_VIR_PROPERTY_DEC(acc, propType, memName, funName, varType)\
	acc: propType memName;\
	public: virtual propType funName() const;\
	public: virtual void funName(varType);
#define ANS_RW_VIR_PROPERTY_PREFIX_DEC(acc, propType, memName, funName, varType)\
	acc: propType memName;\
	public: virtual propType get##funName() const;\
	public: virtual void set##funName(varType);

// readable/writable reference property declaration
#define ANS_RW_REF_PROPERTY_DEC(acc, propType, memName, funName, varType)\
	acc: propType memName;\
	public: propType& funName() const;\
	public: void funName(varType);
#define ANS_RW_REF_PROPERTY_PREFIX_DEC(acc, propType, memName, funName, varType)\
	acc: propType memName;\
	public: propType& get#funName() const;\
	public: void set#funName(varType);

// readable/writable virtual reference property declaration
#define ANS_RW_VIR_REF_PROPERTY_DEC(acc, propType, memName, funName, varType)\
	acc: propType memName;\
	public: virtual propType& funName() const;\
	public: virtual void funName(varType);
#define ANS_RW_VIR_REF_PROPERTY_PREFIX_DEC(acc, propType, memName, funName, varType)\
	acc: propType memName;\
	public: virtual propType& get##funName() const;\
	public: virtual void set##funName(varType);

// -------------------------------------------------------------
// readable/writable property definition
#define ANS_RW_PROPERTY_DEF(acc, propType, memName, funName, varType)\
	acc: propType memName;\
	public: propType funName() const {return memName;};\
	public: void funName(varType value) {memName = value;}
#define ANS_RW_PROPERTY_PREFIX_DEF(acc, propType, memName, funName, varType)\
	acc: propType memName;\
	public: propType get##funName() const {return memName;};\
	public: void set##funName(varType value) {memName = value;}

// readable/writable virtual property definition
#define ANS_RW_VIR_PROPERTY_DEF(acc, propType, memName, funName, varType)\
	acc: propType memName;\
	public: virtual propType funName() const {return memName;};\
	public: virtual void funName(varType value) {memName = value;}
#define ANS_RW_VIR_PROPERTY_PREFIX_DEF(acc, propType, memName, funName, varType)\
	acc: propType memName;\
	public: virtual propType get##funName() const {return memName;};\
	public: virtual void set##funName(varType value) {memName = value;}

// readable/writable reference property definition
#define ANS_RW_REF_PROPERTY_DEF(acc, propType, memName, funName, varType)\
	acc: propType memName;\
	public: propType& funName() const {return memName;};\
	public: void funName(varType value) {memName = value;}
#define ANS_RW_REF_PROPERTY_PREFIX_DEF(acc, propType, memName, funName, varType)\
	acc: propType memName;\
	public: propType& get##funName() const {return memName;};\
	public: void set##funName(varType value) {memName = value;}

// readable/writable virtual reference property definition
#define ANS_RW_VIR_REF_PROPERTY_DEF(acc, propType, memName, funName, varType)\
	acc: propType memName;\
	public: virtual propType& funName() const {return memName;};\
	public: virtual void funName(varType value) {memName = value;}
#define ANS_RW_VIR_REF_PROPERTY_PREFIX_DEF(acc, propType, memName, funName, varType)\
	acc: propType memName;\
	public: virtual propType& get##funName() const {return memName;};\
	public: virtual void set##funName(varType value) {memName = value;}

// -------------------------------------------------------------
// readonly properties
// -------------------------------------------------------------
// readonly property declaration
#define ANS_R_PROPERTY_DEC(acc, propType, memName, funName)\
	acc: propType memName;\
	public: propType funName() const;

// readonly virtual property declaration
#define ANS_R_VIR_PROPERTY_DEC(acc, propType, memName, funName)\
	acc: propType memName;\
	public: virtual propType funName() const;


// readonly property definition
#define ANS_R_PROPERTY_DEF(acc, propType, memName, funName)\
	acc: propType memName;\
	public: propType funName() const {return memName;};

// readonly virtual property definition
#define ANS_R_VIR_PROPERTY_DEF(acc, propType, memName, funName)\
	acc: propType memName;\
	public: virtual propType funName() const {return memName;}


// -------------------------------------------------------------
// readonly reference property declaration
#define ANS_R_REF_PROPERTY_DEC(acc, propType, memName, funName)\
	acc: propType memName;\
	public: propType& funName() const;

// readonly virtual reference property declaration
#define ANS_R_VIR_REF_PROPERTY_DEC(acc, propType, memName, funName)\
	acc: propType memName;\
	public: virtual propType& funName() const;


// readonly reference property definition
#define ANS_R_REF_PROPERTY_DEF(acc, propType, memName, funName)\
	acc: propType memName;\
	public: propType& funName() const {return memName;}

// readonly virtual reference property definition
#define ANS_R_VIR_REF_PROPERTY_DEF(acc, propType, memName, funName)\
	acc: propType memName;\
	public: virtual propType& funName() const {return memName;}

// -------------------------------------------------------------
// readonly reference property declaration
#define ANS_R_CREF_PROPERTY_DEC(acc, propType, memName, funName)\
	acc: propType memName;\
	public: const propType& funName() const;

// readonly virtual reference property declaration
#define ANS_R_VIR_CREF_PROPERTY_DEC(acc, propType, memName, funName)\
	acc: propType memName;\
	public: virtual const propType& funName() const;


// readonly reference property definition
#define ANS_R_CREF_PROPERTY_DEF(acc, propType, memName, funName)\
	acc: propType memName;\
	public: const propType& funName() const {return memName;}

// readonly virtual reference property definition
#define ANS_R_VIR_CREF_PROPERTY_DEF(acc, propType, memName, funName)\
	acc: propType memName;\
	public: virtual const propType& funName() const {return memName;}

#endif


// -----------------------------------------------------------------------
// common simple structs
// -----------------------------------------------------------------------
#define ANS_SMP_STRUCT_1(cls, base, type1, name1) \
	struct cls : public base \
	{ \
		cls(type1 _v) : name1(_v){} \
		mutable type1 name1; \
	};

#define ANS_SMP_STRUCT_2(cls, base, type1, name1, type2, name2) \
	struct cls : public base \
	{ \
		cls(type1 _v1, type2 _v2) : name1(_v1), name2(_v2){} \
		mutable type1 name1; \
		mutable type2 name2; \
	};

#define ANS_SMP_STRUCT_3(cls, base, type1, name1, type2, name2, type3, name3) \
	struct cls : public base \
	{ \
		cls(type1 _v1, type2 _v2, type3 _v3) : name1(_v1), name2(_v2), name3(_v3){} \
		mutable type1 name1; \
		mutable type2 name2; \
		mutable type3 name3; \
	};

#define ANS_SMP_STRUCT_4(cls, base, type1, name1, type2, name2, type3, name3, type4, name4) \
	struct cls : public base \
	{ \
		cls(type1 _v1, type2 _v2, type3 _v3, type4 _v4) : name1(_v1), name2(_v2), name3(_v3), name4(_v4){} \
		mutable type1 name1; \
		mutable type2 name2; \
		mutable type3 name3; \
		mutable type4 name4; \
	};

#define ANS_SMP_STRUCT_5(cls, base, type1, name1, type2, name2, type3, name3, type4, name4, type5, name5) \
	struct cls : public base \
	{ \
		cls(type1 _v1, type2 _v2, type3 _v3, type4 _v4, type5 _v5) : name1(_v1), name2(_v2), name3(_v3) name4(_v4), name5(_v5){} \
		mutable type1 name1; \
		mutable type2 name2; \
		mutable type3 name3; \
		mutable type4 name4; \
		mutable type5 name5; \
	};
