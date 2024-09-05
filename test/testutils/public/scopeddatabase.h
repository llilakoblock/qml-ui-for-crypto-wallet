#pragma once

namespace testing
{
class TestInfo;
} // namespace testing

namespace freedom
{
namespace testutils
{

class ScopedDatabase
{
public:
	~ScopedDatabase();
	ScopedDatabase(const testing::TestInfo& test_info);

private:
	const testing::TestInfo& test_info_;
};

} // namespace testutils
} // namespace freedom
