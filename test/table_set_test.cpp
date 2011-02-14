#include "test_helper.h"
#include <active_record/table.h>

extern string database_file;

class TableSetTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    connection.connect( options
                        ( "adapter", "sqlite" )
                        ( "database", database_file ) );
  }
  virtual void TearDown() {
    delete_database();
  }
 protected:
  Connection connection;
};

TEST_F( TableSetTest, TableCreation ) {
  Table td( &connection, "people" );

  TableSet::create_table( td );

  assert_file_non_zero_length( database_file );
}

TEST_F( TableSetTest, UpdateDatabase ) {
  Table td( &connection, "people" );
  td.fields().push_back( Field( "name",    ActiveRecord::text ) );
  td.fields().push_back( Field( "surname", ActiveRecord::text ) );

  TableSet tables;
  tables[ "Person" ] = td;

  tables.update_database();

  string sql = table_definition( connection, "people" );
  assert_string( "CREATE TABLE people (id INTEGER PRIMARY KEY, name TEXT, surname TEXT)", sql );
}

class ReadSchemaTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    delete_database();
    string create = "echo 'CREATE TABLE foo (bar INTEGER, baz TEXT, qux FLOAT);' | sqlite3 " + database_file + ";";
    system( create.c_str() );
    connection.connect( options
                        ( "adapter", "sqlite" )
                        ( "database", database_file ) );
  }
  virtual void TearDown() {
    delete_database();
  }
 protected:
  Connection connection;
};

TEST_F( ReadSchemaTest, LoadsSchema ) {
  TableSet schema = TableSet::schema( &connection );
  Table foo_table = schema[ "foo" ];

  ASSERT_EQ( foo_table.fields().size(), 3 );
  assert_field( foo_table, 0, "bar", ActiveRecord::integer );
  assert_field( foo_table, 1, "baz", ActiveRecord::text );
  assert_field( foo_table, 2, "qux", ActiveRecord::floating_point );
}

class TableSetCreateTableTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    delete_database();
    connect_database( connection, database_file );
  }
  virtual void TearDown() {
    delete_database();
  }
 protected:
  Connection connection;
};

TEST_F( TableSetCreateTableTest, PrimaryKeyField ) {
  Table td( &connection, "people" );
  td.primary_key( "hi" );
  td.fields().push_back( Field( "height",  ActiveRecord::floating_point ) );

  TableSet tables;
  tables[ "Person" ] = td;
  tables.update_database();

  string sql = table_definition( connection, "people" );
  assert_string( "CREATE TABLE people (hi INTEGER PRIMARY KEY, height FLOAT)", sql );
}

TEST_F( TableSetCreateTableTest, Timestamps ) {
  Table td( &connection, "people" );
  td.timestamps( true );
  td.fields().push_back( Field( "name",    ActiveRecord::text ) );
  td.fields().push_back( Field( "surname", ActiveRecord::text ) );

  TableSet tables;
  tables[ "Person" ] = td;
  tables.update_database();

  string sql = table_definition( connection, "people" );
  assert_string( "CREATE TABLE people (id INTEGER PRIMARY KEY, name TEXT, surname TEXT, created_at TEXT, updated_at TEXT)", sql );
}

class TableSetUpdateDatabaseTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    delete_database();
    pipe_to_sqlite( database_file, "CREATE TABLE foo (bar INTEGER);" );
    connect_database( connection, database_file );
  }
  virtual void TearDown() {
    delete_database();
  }
 protected:
  Connection connection;
};

TEST_F( TableSetUpdateDatabaseTest, AddsFields ) {
  Table td( &connection, "foo" );
  td.fields().push_back( Field( "bar", ActiveRecord::integer ) );
  td.fields().push_back( Field( "baz", ActiveRecord::text ) );
  td.fields().push_back( Field( "qux", ActiveRecord::floating_point ) );

  TableSet tables;
  tables[ "Foo" ] = td;
  tables.update_database();

  string sql = table_definition( connection, "foo" );
  assert_string( "CREATE TABLE foo (bar INTEGER, baz TEXT, qux FLOAT)", sql );
}
