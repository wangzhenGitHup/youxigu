#ifndef SQLITE_WRAPPER_H__
#define  SQLITE_WRAPPER_H__

#include <string>
#include <vector>
#include "sqlite3.h"

class SqliteStatement
{
public:
	SqliteStatement();
	~SqliteStatement();

	int valueInt(int pos_zero_indexed);
	std::string valueString(int pos_zero_indexed);
	bool sqlBind(int pos_zero_indexed, std::string const& value);
	bool sqlBind(int pos_zero_indexed, double value);
	bool sqlBind(int pos_zero_indexed, int value);
	bool sqlBindNil(int pos_zero_indexed);
	bool sqlExcute();
	bool sqlReset();
	bool sqlRestartSelect();
	bool sqlNextRow();

public:
	enum dataType
	{
		INT = SQLITE_INTEGER,
		FLOAT = SQLITE_FLOAT,
		TEXT = SQLITE_TEXT,
		BLOB = SQLITE_BLOB,
		NIL = SQLITE_NULL,
	};
	dataType DataType(int pos_zero_indexed);

private:
	SqliteStatement(std::string const& statement, sqlite3* db);

private:
	sqlite3_stmt* m_stmt = nullptr;
	friend class SqliteWrapper;
};

class SqliteWrapper
{
public:
	SqliteWrapper();
	bool sqliteWrapperOpen(std::string const& db_file);
	
	class ResultRecord
	{
	public:
		std::vector<std::string> m_fields;
	};

	class ResultTable
	{
	public:
		ResultTable() : m_ptr_cur_record(0){}
		std::vector<ResultRecord> m_records;

		ResultRecord* next()
		{
			if (m_ptr_cur_record < m_records.size())
			{
				return &(m_records[m_ptr_cur_record++]);
			}
			return 0;
		}

		void reset()
		{
			m_records.clear();
			m_ptr_cur_record = 0;
		}

	private:
		unsigned int m_ptr_cur_record = 0;
	};

	bool sqlWrapperSelectStmt(std::string const& stmt, ResultTable& res);
	bool sqlWrapperDirectStatement(std::string const& stmt);
	SqliteStatement* Statement(std::string const& stmt);
	std::string lastError();
	bool Begin();
	bool Commit();
	bool Rollback();
	int closedSqlite();

private:
	static int SelectCallback(void* p_data, int num_fields, char** p_fields, char** p_col_names);

private:
	sqlite3* m_db = nullptr;
};
#endif