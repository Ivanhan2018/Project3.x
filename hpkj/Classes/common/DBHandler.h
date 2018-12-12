#pragma once
#include "sqlite3.h"
#include "cocos2d.h"
#include <string>

USING_NS_CC;

#define SQL_BUSY_SLEEP_MIC 500 //500 毫秒.
class DBHandler:Node
{
public:
	int result;		//sqlite3_exec返回值
	sqlite3 *pDB;  //数据库指针
	DBHandler(void);
	~DBHandler(void);
	static DBHandler *sharedDBHandler();
	bool isTableExisted(char *name);
	bool isRecordExisted(char *name, int ID);
	bool isRecordExisted(char *tableName, char *fieldName, char *fieldValue, bool isText);
	int getInt(char const *table, int row, int col);
	sqlite3_int64 getInt64(char const *table, int row, int col);
	double getDouble(char const *table, int row, int col);
	const unsigned char * getText(char const *table, int row, int col);
	int open();
	int execute(char const *sql);
	void close();
	sqlite3 *getDB();
	int updateValueInfo(int levelID, int userID, float time, int star, int moeny, int score);
	void updateUserInfo(int userID);
};
