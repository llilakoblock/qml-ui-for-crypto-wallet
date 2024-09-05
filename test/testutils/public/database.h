#pragma once

namespace testing
{
class TestInfo;
} // namespace testing

namespace freedom
{
namespace testutils
{

bool CreateDatabase(const testing::TestInfo& test_info);
bool RemoveDatabase(const testing::TestInfo& test_info);
bool RemoveTestFolders();

} // namespace testutils
} // namespace freedom
