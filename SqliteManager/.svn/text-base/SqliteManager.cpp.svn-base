#include "SqliteManager.h"
#include "Role.h"
#include "cocos2d.h"

using namespace cocos2d;

std::string SqliteManager::m_dbPath = "";

SqliteManager::~SqliteManager()
{

}

bool SqliteManager::openDB()
{
	if (!m_sqlite.sqliteWrapperOpen(m_dbPath))
	{
		CCLOG("Open DB failed!");
		return false;
	}

	CCLOG("Open DB success!");
	return true;
}

bool SqliteManager::createDBTable(std::string const& stmt)
{
	if (m_sqlite.sqlWrapperDirectStatement(stmt))
	{
		CCLOG("Create Table success!");
		return true;
	}
	CCLOG("Create Table failed!");
	return false;
}

void SqliteManager::checkChatData(int roleId)
{
	CCString* statementStr = CCString::createWithFormat("select count(roleId) from chatTable_%d where roleId = %d", Role::self()->getRoleId(), roleId);
	SqliteStatement* stmt1 = m_sqlite.Statement(statementStr->getCString());
	if (!stmt1)
	{
		CCLOG("get chatTable data error!");
		return;
	}

	int count = 0;
	//统计下当前roleId下的聊天数据总共多少条
	while (stmt1->sqlNextRow())
	{
		count = stmt1->valueInt(0);
	}
	//如果达到50条了，就删除掉第一条
	if (count >= MAX_CHAT_SAVE_COUNT)
	{
		CCString* selectStr = CCString::createWithFormat("select * from chatTable_%d where roleId = %d", Role::self()->getRoleId(), roleId);
		SqliteStatement* stmt2 = m_sqlite.Statement(selectStr->getCString());
		if (!stmt2)
		{
			CCLOG("get chatTable data error!");
			return;
		}
		while (stmt2->sqlNextRow())
		{
			CCString* delteStr = CCString::createWithFormat("delete from chatTable_%d where roleId = %d and timestamp = %d", Role::self()->getRoleId(), roleId, stmt2->valueInt(6));
			if (m_sqlite.sqlWrapperDirectStatement(delteStr->getCString()))
			{
				CCLOG("delete data to chatTable success!");
				return;
			}
		}
	}
}

bool SqliteManager::insertChatDataToTable(STRUCT_NS_ROLE_CHAT_BROADCAST item)
{
	if (m_sqlite.sqliteWrapperOpen(m_dbPath))
	{
		CCLOG("Open DB success!");
		std::string dbStr = this->groupInsertChatData(item);
		if (dbStr == "")
		{
			CCLOG("insert sql statement is error!");
			return false;
		}

		//先检查下有没有超过条数上限
		this->checkChatData(item.roleId);
		if (m_sqlite.sqlWrapperDirectStatement(dbStr))
		{
			CCLOG("Insert data to chatTable success!");
			return true;
		}
		CCLOG("Insert data to chatTable failed!");
		return false;
	}
	CCLOG("Open DB failed!");
	return false;
}

bool SqliteManager::insertDataToTable(std::string& statements)
{
	if (m_sqlite.sqliteWrapperOpen(m_dbPath))
	{
		CCLOG("Open DB success!");
		if (m_sqlite.sqlWrapperDirectStatement(statements))
		{
			CCLOG("Insert data to DB success!");
			return true;
		}
		CCLOG("Insert data to DB failed!");
		return false;
	}
	CCLOG("Open DB failed!");
	return false;
}

bool SqliteManager::getDBChatData(std::map<int, std::vector<STRUCT_NS_ROLE_CHAT_BROADCAST>>& secretChatMap)
{
	if (m_sqlite.sqliteWrapperOpen(m_dbPath))
	{
		CCLOG("Open DB success!");
		CCString* statementStr = CCString::createWithFormat("select* from chatTable_%d", Role::self()->getRoleId());
		SqliteStatement* stmt = m_sqlite.Statement(statementStr->getCString());
		if (!stmt)
		{
			CCLOG("get chatTable data error!");
			return false;
		}

		STRUCT_NS_ROLE_CHAT_BROADCAST item;
		while (stmt->sqlNextRow())
		{
			int roleId = stmt->valueInt(0);
			int chatChannel = stmt->valueInt(1);
			std::string chatContent = stmt->valueString(2);
			std::string roleName = stmt->valueString(3);
			int headId = stmt->valueInt(4);
			int level = stmt->valueInt(5);
			int time = stmt->valueInt(6);
			std::string familyName = stmt->valueString(7);
			int familyJob = stmt->valueInt(8);
			int targetRoleId = stmt->valueInt(9);
			std::string targetRoleName = stmt->valueString(10);
			int targetRoleHeadId = stmt->valueInt(11);
			int targetRoleLevel = stmt->valueInt(12);
			std::string targetRoleFamilyName = stmt->valueString(13);

			item.roleId = roleId;
			item.chatChannel = chatChannel;
			item.chatContent = chatContent;
			item.familyJob = familyJob;
			item.familyName = familyName;
			item.headId = headId;
			item.level = level;
			item.roleName = roleName;
			item.targetRoleFamilyName = targetRoleFamilyName;
			item.targetRoleHeadId = targetRoleHeadId;
			item.targetRoleId = targetRoleId;
			item.targetRoleLevel = targetRoleLevel;
			item.targetRoleName = targetRoleName;
			item.timestamp = time;
			secretChatMap[item.roleId].push_back(item);
		}
		return true;
	}
	CCLOG("Open DB failed!");
	return false;
}

bool SqliteManager::getDBData()
{
	return false;
}

void SqliteManager::getCanUseWriteablePath()
{
	m_dbPath = FileUtils::getInstance()->getWritablePath() +"qmzg2.db";
}

std::string SqliteManager::groupInsertChatData(STRUCT_NS_ROLE_CHAT_BROADCAST item)
{
	CCString* str = CCString::createWithFormat("insert into chatTable_%d values(", Role::self()->getRoleId());
	std::string firstStatementStr = str->getCString();
	std::string endStr = ")";
	CCString *chanelIdStr = CCString::createWithFormat("%d,", item.chatChannel);

	std::string chatContentStr = "'";
	chatContentStr = chatContentStr + item.chatContent;
	chatContentStr = chatContentStr + "',";

	CCString *roleIdStr = CCString::createWithFormat("%d,", item.roleId);

	std::string roleNameStr = "'";
	roleNameStr = roleNameStr + item.roleName;
	roleNameStr = roleNameStr + "',";

	CCString *headIdStr = CCString::createWithFormat("%d,", item.headId);
	CCString *levelStr = CCString::createWithFormat("%d,", item.level);
	CCString *timeStr = CCString::createWithFormat("%d,", item.timestamp);

	std::string familyNameStr = "'";
	familyNameStr = familyNameStr + item.familyName;
	familyNameStr = familyNameStr + "',";

	CCString *familyJobStr = CCString::createWithFormat("%d,", item.familyJob < 0 ? 0 : item.familyJob);
	CCString *targetRoleIdStr = CCString::createWithFormat("%d,", item.targetRoleId);

	std::string targetRoleNameStr = "'";
	targetRoleNameStr = targetRoleNameStr + item.targetRoleName;
	targetRoleNameStr = targetRoleNameStr + "',";

	CCString *targetRoleHeadIdStr = CCString::createWithFormat("%d,", item.targetRoleHeadId);
	CCString *targetRoleLevelStr = CCString::createWithFormat("%d,", item.targetRoleLevel);

	std::string targetRoleFamilyNameStr = "'";
	targetRoleFamilyNameStr = targetRoleFamilyNameStr + item.targetRoleFamilyName;
	targetRoleFamilyNameStr = targetRoleFamilyNameStr + "'";

	firstStatementStr = firstStatementStr + roleIdStr->getCString() + chanelIdStr->getCString() + chatContentStr + roleNameStr +
		headIdStr->getCString() + levelStr->getCString() + timeStr->getCString() + familyNameStr + familyJobStr->getCString() +
		targetRoleIdStr->getCString() + targetRoleNameStr + targetRoleHeadIdStr->getCString() + targetRoleLevelStr->getCString() +
		targetRoleFamilyNameStr + endStr;
	return firstStatementStr;
}

bool SqliteManager::deleteChatDataToDB(int id, std::map<int, std::vector<STRUCT_NS_ROLE_CHAT_BROADCAST>>& tmpMap)
{
	if (id <= 0)
	{
		return false;
	}

	//收集下与当前ROLEID聊天对象的所有roleId
	std::set<int>colllectId;
	std::map<int, std::vector<STRUCT_NS_ROLE_CHAT_BROADCAST>>::iterator it = tmpMap.begin();
	for (; it != tmpMap.end(); ++it)
	{
		for (unsigned int i = 0; i < it->second.size(); i++)
		{
			if (it->second[i].targetRoleId == id)
			{
				colllectId.insert(it->first);
			}
		}
	}

	if (m_sqlite.sqliteWrapperOpen(m_dbPath))
	{
		CCLOG("Open DB success!");
		//先删除目标聊天记录
		CCString* dbStr = CCString::createWithFormat("delete from chatTable_%d where roleId = %d", Role::self()->getRoleId(), id);
		if (m_sqlite.sqlWrapperDirectStatement(dbStr->getCString()))
		{
			CCLOG("Delete data to chatTable success1!");
		}
		
		//再删除与其相关的聊天对象的说话记录
		std::set<int>::iterator it2 = colllectId.begin();
		for (; it2 != colllectId.end(); ++it2)
		{
			dbStr = CCString::createWithFormat("delete from chatTable_%d where roleId = %d and targetRoleId = %d", Role::self()->getRoleId(), (*it2), id);
			if (m_sqlite.sqlWrapperDirectStatement(dbStr->getCString()))
			{
				CCLOG("Delete data to chatTable success2!");
			}
		}
		return true;
	}
	CCLOG("Open DB failed!");
	return false;
}

void SqliteManager::closedDB()
{
	m_sqlite.closedSqlite();
}
