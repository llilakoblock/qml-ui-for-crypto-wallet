#include "scopeddatabase.h"

#include "database.h"

#include <gtest/gtest.h>

namespace freedom
{
namespace testutils
{

ScopedDatabase::~ScopedDatabase() { EXPECT_TRUE(RemoveDatabase(test_info_)); }

ScopedDatabase::ScopedDatabase(const testing::TestInfo& test_info) : test_info_(test_info)
{
	EXPECT_TRUE(CreateDatabase(test_info_));
}

} // namespace testutils
} // namespace freedom
