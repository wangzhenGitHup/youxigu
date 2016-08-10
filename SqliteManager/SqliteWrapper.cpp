#include "SqliteWrapper.h"
#include "cocos2d.h"

using namespace cocos2d;

SqliteWrapper::SqliteWrapper() :m_db(nullptr)
{

}

bool SqliteWrapper::sqliteWrapperOpen(std::string const& db_file)
{
	if (sqlite3_open(db_file.c_str(), &m_db) != SQLITE_OK)
	{
		return false;
	}
	return true;
}

bool SqliteWrapper::sqlWrapperSelectStmt(std::string const& stmt, ResultTable& res)
{
	char* errorMsg;
	int ret;
	res.reset();
	ret = sqlite3_exec(m_db, stmt.c_str(), SelectCallback, static_cast<void*>(&res), &errorMsg);
	if (ret != SQLITE_OK)
	{
		return false;
	}
	return true;
}

int SqliteWrapper::SelectCallback(void* p_data, int num_fields, char** p_fields, char** p_col_names)
{
	ResultTable* res = reinterpret_cast<ResultTable*>(p_data);
	ResultRecord record;

/*#ifdef SQLITE_WRAPPER_REPORT_COLUMN_NAMES
	if (res->m_records.size() == 0)
	{
		ResultRecord col_names;
		for (int i = 0; i < num_fields; i++)
		{
			if (p_fields[i])
			{
				col_names.m_fields.push_back(p_col_names[i]);
			}
			else
			{
				col_names.m_fields.push_back("(null)");
			}
			res->m_records.push_back(col_names);
		}
	}
#endif*/

	for (int i = 0; i < num_fields; i++)
	{
		if (p_fields[i])
		{
			record.m_fields.push_back(p_fields[i]);
		}
		else
		{
			record.m_fields.push_back("<null>");
		}
	}
	res->m_records.push_back(record);
	return 0;
}

SqliteStatement* SqliteWrapper::Statement(std::string const& statement)
{
	SqliteStatement* stmt = nullptr;
	try
	{
		stmt = new SqliteStatement(statement, m_db);
	}
	catch (const char* e)
	{
		return 0;
	}
}

SqliteStatement::SqliteStatement(std::string const& statement, sqlite3* db)
{
	if (sqlite3_prepare(db, statement.c_str(),
		-1, /*读取的字节长度， 如果小于0，就终止，如果大于0，则为能读取的最大字节数*/
		&m_stmt, /*用来指向输入参数中下一个需要编译的sql语句存放的sqlite statement对象的指针*/
		0/*指针：指向m_stmt未编译的部分*/
		) != SQLITE_OK)
	{
		throw sqlite3_errmsg(db);
	}
	
	if (!m_stmt)
	{
		throw "stmt_is null";
	}
}

SqliteStatement::~SqliteStatement()
{
	if (m_stmt)
	{
		sqlite3_finalize(m_stmt);
	}
}

SqliteStatement::SqliteStatement() : m_stmt(nullptr)
{
}

bool SqliteStatement::sqlBind(int pos_zero_indexed, std::string const& value)
{
	if (sqlite3_bind_text(m_stmt, 
		pos_zero_indexed + 1, /*通配符索引*/
		value.c_str(), 
		value.length(), /*文本长度*/
		SQLITE_TRANSIENT/*sqlite自我复制*/
		) != SQLITE_OK)
	{
		return false;
	}
	return true;
}

bool SqliteStatement::sqlBind(int pos_zero_indexed, double value)
{
	if (sqlite3_bind_double(m_stmt, pos_zero_indexed + 1, value) != SQLITE_OK)
	{
		return false;
	}
	return true;
}

bool SqliteStatement::sqlBind(int pos_zero_indexed, int value)
{
	if (sqlite3_bind_int(m_stmt, pos_zero_indexed + 1, value) != SQLITE_OK)
	{
		return false;
	}
	return true;
}

bool SqliteStatement::sqlBindNil(int pos_zero_indexed)
{
	if (sqlite3_bind_null(m_stmt, pos_zero_indexed + 1) != SQLITE_OK)
	{
		return false;
	}
	return true;
}

bool SqliteStatement::sqlExcute()
{
	int rc = sqlite3_step(m_stmt);
	if (rc == SQLITE_BUSY)
	{
		CCLOG("sqlite busy");
		return false;
	}
	if (rc == SQLITE_ERROR)
	{
		CCLOG("sqlite error");
		return false;
	}
	if (rc == SQLITE_MISUSE)
	{
		CCLOG("sqlite error");
		return false;
	}

	if (rc != SQLITE_DONE)
	{
		return false;
	}
	sqlite3_reset(m_stmt);
	return true;
}

SqliteStatement::dataType SqliteStatement::DataType(int pos_zero_indexed)
{
	return dataType(sqlite3_column_type(m_stmt, pos_zero_indexed));
}

int SqliteStatement::valueInt(int pos_zero_indexed)
{
	return sqlite3_column_int(m_stmt, pos_zero_indexed);
}

std::string SqliteStatement::valueString(int pos_zero_indexed)
{
	return std::string(reinterpret_cast<const char*>(sqlite3_column_text(m_stmt, pos_zero_indexed)));
}

bool SqliteStatement::sqlRestartSelect()
{
	sqlite3_reset(m_stmt);
	return true;
}

bool SqliteStatement::sqlReset()
{
	int rc = sqlite3_step(m_stmt);
	sqlite3_reset(m_stmt);
	if (rc == SQLITE_ROW)
	{
		return true;
	}
	return false;
}

bool SqliteStatement::sqlNextRow()
{
	int rc = sqlite3_step(m_stmt);
	if (rc == SQLITE_ROW)
	{
		return true;
	}
	if (rc == SQLITE_DONE)
	{
		sqlite3_reset(m_stmt);
		return false;
	}
	else if (rc == SQLITE_MISUSE)
	{
		CCLOG("SqliteStatement::sqlNextRow SQLITE_MISUSE");
	}
	else if (rc == SQLITE_BUSY)
	{
		CCLOG("SqliteStatement::sqlNextRow SQLITE_BUSY");
	}
	return false;
}

bool SqliteWrapper::sqlWrapperDirectStatement(std::string const& stmt)
{
	char* errorMsg;
	int ret;
	
	ret = sqlite3_exec(m_db, stmt.c_str(), 0, 0, &errorMsg);
	if (ret != SQLITE_OK)
	{
		return false;
	}
	return true;
}


std::string SqliteWrapper::lastError()
{
	return sqlite3_errmsg(m_db);
}

bool SqliteWrapper::Begin()
{
	return sqlWrapperDirectStatement("begin");
}

bool SqliteWrapper::Commit()
{
	return sqlWrapperDirectStatement("commit");
}

bool SqliteWrapper::Rollback()
{
	return sqlWrapperDirectStatement("rollback");
}

int SqliteWrapper::closedSqlite()
{
	return sqlite3_close(m_db);
}
