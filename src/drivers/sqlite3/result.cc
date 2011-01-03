#include "common.h"

namespace dbi {
    void Sqlite3Result::init() {
        _rowno         = 0;
        _rows          = 0;
        _cols          = 0;

        affected_rows  = 0;
        last_insert_id = 0;
    }

    void Sqlite3Result::clear() {
        _rowno         = 0;
        _rows          = 0;
        affected_rows  = 0;
        last_insert_id = 0;

        _rowdata.clear();
    }

    Sqlite3Result::~Sqlite3Result() {
        cleanup();
    }

    Sqlite3Result::Sqlite3Result(sqlite3_stmt *stmt, string sql) {
        init();

        _sql = sql;
        fetchMeta(stmt);
    }

    void Sqlite3Result::fetchMeta(sqlite3_stmt *stmt) {
        _cols = sqlite3_column_count(stmt);

        for (int n = 0; n < _cols; n++) {
            _rsfields.push_back(sqlite3_column_name(stmt, n));
            switch(sqlite3_column_type(stmt, n)) {
                case SQLITE_INTEGER: _rstypes.push_back(DBI_TYPE_INT);     break;
                case SQLITE_FLOAT:   _rstypes.push_back(DBI_TYPE_NUMERIC); break;
                case SQLITE_TEXT:    _rstypes.push_back(DBI_TYPE_TEXT);    break;
                case SQLITE_BLOB:    _rstypes.push_back(DBI_TYPE_BLOB);    break;
                default:             _rstypes.push_back(DBI_TYPE_TEXT);
            }
        }
    }

    void Sqlite3Result::write(unsigned char *data, uint64_t length) {
        if (_rowdata.size() <= _rowno) {
            ResultRow r;
            _rowdata.push_back(r);
        }
        _rowdata[_rowno].push_back(
            data ?
                length ? PARAM(data,length) : PARAM((char*)data)
                : PARAM(null())
        );
    }

    void Sqlite3Result::flush() {
        _rowno++;
        _rows++;
    }

    uint32_t Sqlite3Result::rows() {
        return affected_rows > 0 ? affected_rows : _rows;
    }

    bool Sqlite3Result::consumeResult() {
        return false;
    }

    void Sqlite3Result::prepareResult() {
        // NOP
    }

    uint64_t Sqlite3Result::lastInsertID() {
        return last_insert_id;
    }

    bool Sqlite3Result::read(ResultRow &row) {
        if (_rowno < _rows) {
            row = _rowdata[_rowno++];
            return true;
        }
        return false;
    }

    bool Sqlite3Result::read(ResultRowHash &rowhash) {
        if (_rowno < _rows) {
            rowhash.clear();
            for (int n = 0; n < _cols; n++) rowhash[_rsfields[n]] = _rowdata[_rowno][n];
            _rowno++;
            return true;
        }
        return false;
    }

    unsigned char* Sqlite3Result::read(uint32_t r, uint32_t c, uint64_t *l) {
        if (r >= _rows || c >= _cols || r < 0 || c < 0) return 0;

        if (_rowdata[r][c].isnull) {
            return 0;
        }
        else {
            if (l) *l = _rowdata[r][c].value.length();
            return (unsigned char*)_rowdata[r][c].value.c_str();
        }
    }

    vector<string>& Sqlite3Result::fields() {
        return _rsfields;
    }

    uint32_t Sqlite3Result::columns() {
        return _cols;
    }

    void Sqlite3Result::cleanup() {
        _rsfields.clear();
        _rstypes.clear();
        _rowdata.clear();

        _rowno   = 0;
        _rows    = 0;
        _cols    = 0;
    }

    uint32_t Sqlite3Result::tell() {
        return _rowno;
    }

    void Sqlite3Result::rewind() {
        _rowno = 0;
    }

    void Sqlite3Result::seek(uint32_t r) {
        _rowno = r;
    }

    vector<int>& Sqlite3Result::types() {
        return _rstypes;
    }
}