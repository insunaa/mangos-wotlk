/*
 * This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "QueryResultSqlite.h"
#include <cstdint>
#include <cstring>
#include <sqlite3.h>
#ifdef DO_SQLITE

#include "Util/Util.h"
#include "Policies/Singleton.h"
#include "Platform/Define.h"
#include "Multithreading/Threading.h"
#include "DatabaseEnv.h"
#include "Util/Timer.h"
#include "DatabaseSqlite.h"

size_t DatabaseSqlite::db_count = 0;

void DatabaseSqlite::ThreadStart()
{ };

void DatabaseSqlite::ThreadEnd()
{ };

DatabaseSqlite::DatabaseSqlite()
{
    // before first connection
    if (db_count++ == 0);
}

DatabaseSqlite::~DatabaseSqlite()
{
    StopServer();
    if (--db_count == 0);
}

SqlConnection* DatabaseSqlite::CreateConnection()
{
    return new SQLiteConnection(*this);
}

SQLiteConnection::~SQLiteConnection()
{
    FreePreparedStatements();
    //mysql_close(mSqlite);
    sqlite3_close(mSqlite);
}

bool SQLiteConnection::Initialize(const char* infoString)
{
    if (sqlite3_open(infoString, &mSqlite) != SQLITE_OK)
    {
        sLog.outError("Could not open SQLite database");
        return false;
    }
    sqlite3_exec(mSqlite, "PRAGMA journal_mode=WAL;", 0, 0, 0);
    sqlite3_busy_timeout(mSqlite, 2);

    Tokens tokens = StrSplit(infoString, ";");

    DETAIL_LOG("Connected to SQLite database at %s", infoString);


    // set connection properties to UTF8 to properly handle locales for different
    // server configs - core sends data in UTF8, so MySQL must expect UTF8 too
//    Execute("SET NAMES `utf8`");
//    Execute("SET CHARACTER SET `utf8`");

    return true;
}

bool SQLiteConnection::_Query(const char* sql, sqlite3_stmt** pStmt)
{
    if (!mSqlite)
        return false;

    uint32 _s = WorldTimer::getMSTime();
    int result;
    result = sqlite3_prepare_v2(mSqlite, sql, -1, pStmt, NULL);
    if (result != SQLITE_OK) {
        sLog.outErrorDb("SQL: %s", sql);
        sLog.outErrorDb("query ERROR: %s", sqlite3_errmsg(mSqlite));
        return false;
    }

    return true;
}

QueryResult* SQLiteConnection::Query(const char* sql)
{
    sqlite3_stmt** pStmt = new(sqlite3_stmt*);
    if (!_Query(sql, pStmt))
        return nullptr;

    QueryResultSqlite* queryResult = new QueryResultSqlite(pStmt);

    if (queryResult->NextRow())
        return queryResult;
    return nullptr;
}

QueryNamedResult* SQLiteConnection::QueryNamed(const char* sql)
{
    MYSQL_RES* result = nullptr;
    MYSQL_FIELD* fields = nullptr;
    uint64 rowCount = 0;
    uint32 fieldCount = 0;

    sqlite3_stmt** pStmt = new(sqlite3_stmt*);

    if (!_Query(sql, pStmt))
        return nullptr;

    fieldCount = sqlite3_column_count(*pStmt);

    QueryFieldNames names(fieldCount);
    for (uint32 i = 0; i < fieldCount; ++i)
        names[i] = fields[i].name;

    QueryResultSqlite* queryResult = new QueryResultSqlite(pStmt);

    queryResult->NextRow();
    return new QueryNamedResult(queryResult, names);
}

bool SQLiteConnection::Execute(const char* sql)
{
    if (!mSqlite)
        return false;

    {
        uint32 _s = WorldTimer::getMSTime();
        sqlite3_stmt** pStmt = new(sqlite3_stmt*);

        int result;
        result = sqlite3_prepare_v2(mSqlite, sql, -1, pStmt, NULL);
        if (result != SQLITE_OK)
        {
            sLog.outErrorDb("SQL: %s", sql);
            sLog.outErrorDb("SQL ERROR: %s", sqlite3_errmsg(mSqlite));
            return false;
        }
        result = sqlite3_step(*pStmt);
        if (result != SQLITE_DONE && result != SQLITE_OK)
        {
            sLog.outErrorDb("SQL: %s", sql);
            sLog.outErrorDb("SQL ERROR: %s", sqlite3_errmsg(mSqlite));
            return false;
        }
        sqlite3_finalize(*pStmt);
        DEBUG_FILTER_LOG(LOG_FILTER_SQL_TEXT, "[%u ms] SQL: %s", WorldTimer::getMSTimeDiff(_s, WorldTimer::getMSTime()), sql);
        // end guarded block
    }

    return true;
}

bool SQLiteConnection::_TransactionCmd(const char* sql)
{
    sqlite3_stmt** pStmt = new(sqlite3_stmt*);

    int result;
    result = sqlite3_prepare_v2(mSqlite, sql, -1, pStmt, NULL);
    if (result != SQLITE_OK)
    {
        sLog.outErrorDb("SQL: %s", sql);
        sLog.outErrorDb("SQL ERROR: %s", sqlite3_errmsg(mSqlite));
        return false;
    }
    result = sqlite3_step(*pStmt);
    if (result != SQLITE_DONE && result != SQLITE_OK)
    {
        sLog.outErrorDb("SQL: %s", sql);
        sLog.outErrorDb("SQL ERROR: %s", sqlite3_errmsg(mSqlite));
        return false;
    }
    DEBUG_FILTER_LOG(LOG_FILTER_SQL_TEXT, "SQL: %s", sql);
    return true;
}

bool SQLiteConnection::BeginTransaction()
{
    return _TransactionCmd("BEGIN");
}

bool SQLiteConnection::CommitTransaction()
{
    return _TransactionCmd("COMMIT");
}

bool SQLiteConnection::RollbackTransaction()
{
    return _TransactionCmd("ROLLBACK");
}

unsigned long SQLiteConnection::escape_string(char* to, const char* from, unsigned long length)
{
    if (!mSqlite || !to || !from || !length)
        return 0;
    std::string newFrom(from);
    std::string newTo;

    for (char& c : newFrom)
    {
        if (c == '\'') {
            newTo += "''";  // Replace single quotes with two single quotes
        } else {
            newTo += c;
        }
    }
    strcpy(to, newTo.c_str());

    return newTo.length();
}

//////////////////////////////////////////////////////////////////////////
SqlPreparedStatement* SQLiteConnection::CreateStatement(const std::string& fmt)
{
    return new SqlitePreparedStatement(fmt, *this, mSqlite);
}

//////////////////////////////////////////////////////////////////////////
SqlitePreparedStatement::SqlitePreparedStatement(const std::string& fmt, SqlConnection& conn, sqlite3* mysql) : SqlPreparedStatement(fmt, conn),
    m_pSqliteConn(mysql), m_stmt(nullptr), m_pResult(nullptr)
{
}

SqlitePreparedStatement::~SqlitePreparedStatement()
{
    RemoveBinds();
}

bool SqlitePreparedStatement::prepare()
{
    if (isPrepared())
        return true;

    // remove old binds
    RemoveBinds();

    if (!m_stmt)
        m_stmt = new(sqlite3_stmt*);

    // Create statement object
    int result = sqlite3_prepare_v2(m_pSqliteConn, m_szFmt.c_str(), m_szFmt.length(), m_stmt, nullptr);
    if (result != SQLITE_OK) {
        sLog.outError("SQL: sqlite3_prepare_v2() failed: %s", sqlite3_errmsg(m_pSqliteConn));
        return false;
    }
    // Get the parameter count from the statement
    m_nParams = sqlite3_bind_parameter_count(*m_stmt);

    // Check if we have a statement which returns result sets
    if (sqlite3_stmt_readonly(*m_stmt) == 0) {
        // Our statement is a query
        m_bIsQuery = true;
        m_nColumns = sqlite3_column_count(*m_stmt);  // Get the number of columns in the result set
    }

    m_bPrepared = true;
    return true;
}

void SqlitePreparedStatement::bind(const SqlStmtParameters& holder)
{
    if (!isPrepared())
    {
        MANGOS_ASSERT(!isPrepared());
        return;
    }

    // verify if we bound all needed input parameters
    if (m_nParams != holder.boundParams())
    {
        MANGOS_ASSERT(m_nParams != holder.boundParams());
        return;
    }

    unsigned int nIndex = 0; // SQLite uses 1-based index for parameter binding
    SqlStmtParameters::ParameterContainer const& _args = holder.params();

    for (const auto& param : _args)
    {
        // Bind parameter
        addParam(nIndex, param);
        nIndex++;
    }
}

void SqlitePreparedStatement::addParam(unsigned int nIndex, const SqlStmtFieldData& data)
{
    MANGOS_ASSERT(nIndex < m_nParams);

    // SQLite uses different types and structures for binding parameters
    int result = SQLITE_OK;

    switch (data.type()) {
        case FIELD_BOOL:
        case FIELD_UI8:
            result = sqlite3_bind_int(*m_stmt, nIndex + 1, *static_cast<const uint8*>(data.buff()));
            break;
        case FIELD_UI16:
            result = sqlite3_bind_int(*m_stmt, nIndex + 1, *static_cast<const uint16*>(data.buff()));
            break;
        case FIELD_UI32:
            result = sqlite3_bind_int(*m_stmt, nIndex + 1, *static_cast<const uint32*>(data.buff()));
            break;
        case FIELD_UI64:
            result = sqlite3_bind_int64(*m_stmt, nIndex + 1, *static_cast<const uint64*>(data.buff()));
            break;
        case FIELD_I8:
            result = sqlite3_bind_int(*m_stmt, nIndex + 1, *static_cast<const int8*>(data.buff()));
            break;
        case FIELD_I16:
            result = sqlite3_bind_int(*m_stmt, nIndex + 1, *static_cast<const int16*>(data.buff()));
            break;
        case FIELD_I32:
            result = sqlite3_bind_int(*m_stmt, nIndex + 1, *static_cast<const int32*>(data.buff()));
            break;
        case FIELD_I64:
            result = sqlite3_bind_int64(*m_stmt, nIndex + 1, *static_cast<const int64*>(data.buff()));
            break;
        case FIELD_FLOAT:
            result = sqlite3_bind_double(*m_stmt, nIndex + 1, *static_cast<const float*>(data.buff()));
            break;
        case FIELD_DOUBLE:
            result = sqlite3_bind_double(*m_stmt, nIndex + 1, *static_cast<const double*>(data.buff()));
            break;
        case FIELD_STRING:
            result = sqlite3_bind_text(*m_stmt, nIndex + 1, static_cast<const char*>(data.buff()), -1, SQLITE_STATIC);
            break;
        case FIELD_NONE:
            result = sqlite3_bind_null(*m_stmt, nIndex + 1);
            break;
        // Handle other data types as needed

        default:
            MANGOS_ASSERT(false && "Unsupported parameter type");
    }

    if (result != SQLITE_OK) {
        // Handle the error, e.g., print an error message
        fprintf(stderr, "Error binding parameter at index %d: %s\n", nIndex, sqlite3_errmsg(m_pSqliteConn));
    }
}

void SqlitePreparedStatement::RemoveBinds()
{
    if (!m_stmt)
        return;

    // Finalize the prepared statement
    sqlite3_finalize(*m_stmt);
    *m_stmt = nullptr;

    m_bPrepared = false;
}

bool SqlitePreparedStatement::execute()
{
    if (!isPrepared())
        return false;

    int result = sqlite3_step(*m_stmt);

    if (result != SQLITE_DONE)
    {
        sLog.outErrorDb("SQL: cannot execute '%s'", m_szFmt.c_str());
        sLog.outErrorDb("SQL ERROR: %s", sqlite3_errmsg(m_pSqliteConn));
        return false;
    }

    // Reset the prepared statement to be executed again if needed
    sqlite3_reset(*m_stmt);

    return true;
}

enum_field_types SqlitePreparedStatement::ToMySQLType(const SqlStmtFieldData& data, bool& bUnsigned)
{
    bUnsigned = 0;
    enum_field_types dataType = MYSQL_TYPE_NULL;

    switch (data.type())
    {
        case FIELD_NONE:    dataType = MYSQL_TYPE_NULL;                     break;
        // MySQL does not support MYSQL_TYPE_BIT as input type
        case FIELD_BOOL:    // dataType = MYSQL_TYPE_BIT;      bUnsigned = 1;  break;
        case FIELD_UI8:     dataType = MYSQL_TYPE_TINY;     bUnsigned = 1;  break;
        case FIELD_I8:      dataType = MYSQL_TYPE_TINY;                     break;
        case FIELD_I16:     dataType = MYSQL_TYPE_SHORT;                    break;
        case FIELD_UI16:    dataType = MYSQL_TYPE_SHORT;    bUnsigned = 1;  break;
        case FIELD_I32:     dataType = MYSQL_TYPE_LONG;                     break;
        case FIELD_UI32:    dataType = MYSQL_TYPE_LONG;     bUnsigned = 1;  break;
        case FIELD_I64:     dataType = MYSQL_TYPE_LONGLONG;                 break;
        case FIELD_UI64:    dataType = MYSQL_TYPE_LONGLONG; bUnsigned = 1;  break;
        case FIELD_FLOAT:   dataType = MYSQL_TYPE_FLOAT;                    break;
        case FIELD_DOUBLE:  dataType = MYSQL_TYPE_DOUBLE;                   break;
        case FIELD_STRING:  dataType = MYSQL_TYPE_STRING;                   break;
    }

    return dataType;
}
#endif
