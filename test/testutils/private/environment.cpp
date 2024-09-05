#include "environment.h"

#include "database.h"

namespace freedom
{
namespace testutils
{

void Environment::SetUp() {}

void Environment::TearDown() { RemoveTestFolders(); }

} // namespace testutils
} // namespace freedom
