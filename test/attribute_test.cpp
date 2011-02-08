#include "test_helper.h"

extern string database_file;

class AttributeTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    delete_database();
    connection.connect( options
                        ( "adapter", "sqlite" )
                        ( "database", database_file ) );
    Person::setup( &connection );
  }
  virtual void TearDown() {
    delete_database();
  }
 protected:
  Connection connection;
};

TEST_F( AttributeTest, SetSingly ) {
  Person joe;
  joe[ "name" ]    = "Joe";
  joe[ "surname" ] = "Yates";
  joe[ "age" ]     = 45;
  joe[ "height" ]  = 1.80;
  EXPECT_TRUE( joe.save() );
}