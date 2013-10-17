#include "sqlite_result.h"
#include "db_exception.h"
#include "value.h"

#include "sqlite3.h"

#include <sstream>

namespace cpp_db
{

void throw_db_exception(int error_code, sqlite3 *db);

sqlite_result::sqlite_result(const std::shared_ptr<void> &stmt_handle)
    : stmt(std::static_pointer_cast<sqlite3_stmt>(stmt_handle))
    , row_status(SQLITE_DONE)
{
    move_next();
    for (int column = 0; column < get_column_count(); ++column)
        column_names.emplace(sqlite3_column_name(stmt.get(), column), column);
}

void sqlite_result::move_next()
{
    row_status = sqlite3_step(stmt.get());
    if (row_status != SQLITE_DONE && row_status != SQLITE_ROW)
        throw_db_exception(row_status, sqlite3_db_handle(stmt.get()));
}

void sqlite_result::move_prev()
{
    throw db_exception("Not supported!");
}

void sqlite_result::move_first()
{
    if (int error_code = sqlite3_reset(stmt.get()))
        throw_db_exception(error_code, sqlite3_db_handle(stmt.get()));
    move_next();
}

bool sqlite_result::is_eof() const
{
    return row_status == SQLITE_DONE;
}

int sqlite_result::get_column_count() const
{
    return sqlite3_column_count(stmt.get());
}

std::string sqlite_result::get_column_name(int column) const
{
    if (const char *column_name = sqlite3_column_name(stmt.get(), column))
        return column_name;
    else
    {
        std::stringstream message;
        message << "Column " << column << " not found!";
        throw db_exception(message.str());
    }
}

value sqlite_result::get_column_value(int column) const
{
    sqlite3_stmt *pstmt = stmt.get();

    switch (sqlite3_column_type(pstmt, column))
    {
    case SQLITE_INTEGER:	// int64
        return sqlite3_column_int64(pstmt, column);
    case SQLITE_FLOAT:		// double
        return sqlite3_column_double(pstmt, column);
    case SQLITE_BLOB:		// void *
    {
        const uint8_t *data = static_cast<const uint8_t*>(sqlite3_column_blob(pstmt, column));
        return blob(data, data + sqlite3_column_bytes(pstmt, column));
    }
    case SQLITE_TEXT:		// const char *
        return std::string(reinterpret_cast<const char *>(sqlite3_column_text(pstmt, column)), sqlite3_column_bytes(pstmt, column));
    case SQLITE_NULL:		//
    default:
        break;
    }
    return null_type();
}

int sqlite_result::get_column_index(const std::string &column_name) const
{
    auto pos = column_names.find(column_name);
    if (pos != column_names.end())
        return pos->second;
    std::stringstream message;
    message << "Column '" << column_name << "' not found!";
    throw db_exception(message.str());
}

value sqlite_result::get_column_value(const std::string &column_name) const
{
    return get_column_value(get_column_index(column_name));
}

} // namespace cpp_db
