#ifndef _ACTIVE_RECORD_CONNECTION_H_
#define _ACTIVE_RECORD_CONNECTION_H_

#include <active_record/options.h>
#include <active_record/type.h>
#include <active_record/table_set.h>
#include <active_record/row.h>
#include <active_record/query.h>
#include <cstdio>
#include <sqlite3.h>

namespace ActiveRecord {

class Connection {
 friend class TableSet;
 public:
  Connection();

  void      connect( OptionsHash options );
  // Database Structure
  bool      table_exists( const string &table_name );
  // Transactions
  void      begin_transaction();
  void      commit();
  // Queries
  bool          execute( const string &query,
                         const AttributeList &parameters = AttributeList() );
  Attribute     select_value( const string &query,
                              const AttributeList &parameters = AttributeList() );
  AttributeList select_values( const string &query,
                               const AttributeList &parameters = AttributeList() );
  Row           select_one( const string &query,
                            const AttributeList &parameters = AttributeList() );
  RowSet        select_all( const string &query,
                            const AttributeList &parameters = AttributeList() );
 private:
  Connection( const Connection& other );
  Connection operator=( const Connection& other );

  bool      sqlite_initialize( string database_path_name );
  sqlite3_stmt * prepare( const string &query, const AttributeList &parameters );
  void      bind_parameters( sqlite3_stmt *ppStmt, const AttributeList &parameters );

  sqlite3 *        db_;
};

} // namespace ActiveRecord

#endif // ndef _ACTIVE_RECORD_CONNECTION_H_
