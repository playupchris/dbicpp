#ifndef _DBICXX_SQLITE3_STATEMENT_H
#define _DBICXX_SQLITE3_STATEMENT_H

namespace dbi {

    class Sqlite3Result;
    class Sqlite3Statement : public AbstractStatement {
        private:
        string _sql;
        sqlite3 *_conn;
        sqlite3_stmt *_stmt;
        Sqlite3Result *_result;

        protected:
        uint64_t last_insert_id;

        public:
        Sqlite3Statement();
        ~Sqlite3Statement();
        Sqlite3Statement(string query, sqlite3 *conn);
        void cleanup();
        void finish();
        string command();

        uint32_t execute();
        uint32_t execute(param_list_t&);
        uint64_t lastInsertID();

        Sqlite3Result* result();
    };
}

#endif
