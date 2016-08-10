#ifndef  __SQLITE_MANAGER__H__
#define __SQLITE_MANAGER__H__

#include "SqliteWrapper.h"
#include "RoleProtocols.h"

const std::string createDBStatement = "Create table chatDB(roleId integer, chatChannel interger, chatContent text, \
														 roleName text, headId integer, level integer, timestamp integer, \
														familyName text, familyJob integer, targetRoleId integer, targetRoleName text, \
														targetRoleHeadId integer, targetRoleLevel integer, targetRoleFamilyName text)";

#define MAX_CHAT_SAVE_COUNT 50

class SqliteManager
{
public:
	SqliteManager(){}

	~SqliteManager();
	static SqliteManager* getInstance()
	{
		static SqliteManager m_Instance;
		m_Instance.getCanUseWriteablePath();
		return &m_Instance;
	}
	//打开数据库
	bool openDB();
	//创建数据库表
	bool createDBTable(std::string const& stmt);
	//删除聊天表中的某个数据
	bool deleteChatDataToDB(int id, std::map<int, std::vector<STRUCT_NS_ROLE_CHAT_BROADCAST>>& tmpMap);
	//向数据库中插入聊天数据，只针对聊天，其他的自行处理
	bool insertChatDataToTable(STRUCT_NS_ROLE_CHAT_BROADCAST item);
	//插入数据
	bool insertDataToTable(std::string& statements);
	//获取数据库中的聊天数据
	bool getDBChatData(std::map<int, std::vector<STRUCT_NS_ROLE_CHAT_BROADCAST>>& secretChatMap);
	//获取数据库中的数据
	bool getDBData();
	//关闭数据库
	void closedDB();

private:
	//获取本地可用的存储路径
	static void getCanUseWriteablePath();
	//只针对聊天，其他的自行处理
	std::string groupInsertChatData(STRUCT_NS_ROLE_CHAT_BROADCAST item);
	//用于聊天，插入完新数据后，检测下有没有超过数量上限，超过了就删掉最早的那条
	void checkChatData(int roleId);

private:
	SqliteWrapper m_sqlite;
	static std::string m_dbPath;
};

#endif