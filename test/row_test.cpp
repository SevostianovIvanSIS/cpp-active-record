#include "test_helper.h"

extern string database_file;

class RowTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    pipe_to_sqlite( database_file, "CREATE TABLE foo (bar INTEGER, baz TEXT, qux FLOAT);" );
    pipe_to_sqlite( database_file, "INSERT INTO foo (bar, baz, qux) VALUES (123, \"hello\", 1.5);" );
    sqlite3_open( database_file.c_str(), &db );
  }
  virtual void TearDown() {
    delete_database();
  }
  sqlite3      *db;
  sqlite3_stmt *ppStmt;
};

TEST_F( RowTest, GetType ) {
  char query[] = "SELECT * FROM foo";
  int prepare_result = sqlite3_prepare_v2( db, query, strlen(query), &ppStmt, 0 );
  sqlite3_step( ppStmt );

  Row row( ppStmt );

  ASSERT_EQ( ActiveRecord::integer,        row.get_type( "bar" ) );
  ASSERT_EQ( ActiveRecord::text,           row.get_type( "baz" ) );
  ASSERT_EQ( ActiveRecord::floating_point, row.get_type( "qux" ) );
}

TEST_F( RowTest, IsType ) {
  char query[] = "SELECT * FROM foo";
  int prepare_result = sqlite3_prepare_v2( db, query, strlen(query), &ppStmt, 0 );
  sqlite3_step( ppStmt );

  Row row( ppStmt );

  ASSERT_TRUE(  row.is_type( "bar", ActiveRecord::integer ) );
  ASSERT_FALSE( row.is_type( "bar", ActiveRecord::text ) );
}

TEST_F( RowTest, GetInteger ) {
  char query[] = "SELECT * FROM foo";
  int prepare_result = sqlite3_prepare_v2( db, query, strlen(query), &ppStmt, 0 );
  sqlite3_step( ppStmt );

  Row row( ppStmt );

  ASSERT_EQ( 123, row.get_integer( "bar" ) );
}

TEST_F( RowTest, GetText ) {
  char query[] = "SELECT * FROM foo";
  int prepare_result = sqlite3_prepare_v2( db, query, strlen(query), &ppStmt, 0 );
  sqlite3_step( ppStmt );

  Row row( ppStmt );

  ASSERT_EQ( "hello", row.get_text( "baz" ) );
}

TEST_F( RowTest, GetFloatingPoint ) {
  char query[] = "SELECT * FROM foo";
  sqlite3_prepare_v2( db, query, strlen(query), &ppStmt, 0 );
  sqlite3_step( ppStmt );

  Row row( ppStmt );

  ASSERT_EQ( 1.5, row.get_floating_point( "qux" ) );
}