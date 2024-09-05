#include "database.h"

#include "freedomconfig.h"
#include "freedomdatabase.h"

#include <gtest/gtest.h>

#include <QDir>

namespace
{

const QString kTestFolder("test-data");

QString GetTestFolder() { return (QDir::currentPath() + QDir::separator() + kTestFolder); }

} // namespace

namespace freedom
{
namespace testutils
{

bool CreateDatabase(const testing::TestInfo& test_info)
{
	using namespace freedom::config;
	using namespace freedom::core::database;

	DatabaseConfig config;
	EXPECT_TRUE(FreedomConfigurator().GetDatabaseConfig(config));

	SqlDatabaseConfig sqldbconfig(config);
	sqldbconfig.SetDatabaseName(QString(test_info.test_suite_name()) + "." + test_info.name() + ".db");
	sqldbconfig.SetDatabasePath(GetTestFolder());

	if (FreedomDatabase::CheckDatabaseExists(sqldbconfig))
		EXPECT_TRUE(RemoveDatabase(test_info));

	EXPECT_FALSE(FreedomDatabase::CheckDatabaseExists(sqldbconfig));
	const bool result = FreedomDatabase::CreateDatabase(sqldbconfig);
	EXPECT_TRUE(result);
	EXPECT_TRUE(FreedomDatabase::CheckDatabaseExists(sqldbconfig));
	return result;
}

bool RemoveDatabase(const testing::TestInfo& test_info)
{
	using namespace freedom::config;
	using namespace freedom::core::database;

	DatabaseConfig config;
	EXPECT_TRUE(FreedomConfigurator().GetDatabaseConfig(config));

	SqlDatabaseConfig sqldbconfig(config);
	sqldbconfig.SetDatabaseName(QString(test_info.test_suite_name()) + "." + test_info.name() + ".db");
	sqldbconfig.SetDatabasePath(GetTestFolder());

	const bool exists = FreedomDatabase::CheckDatabaseExists(sqldbconfig);
	EXPECT_TRUE(exists);
	if (!exists)
		return false;

	const bool result = FreedomDatabase::RemoveDatabase(sqldbconfig);
	EXPECT_TRUE(result);
	return result;
}

bool RemoveTestFolders()
{
	const bool result = QDir(QDir::fromNativeSeparators(GetTestFolder())).removeRecursively();
	EXPECT_TRUE(result);
	return result;
}

} // namespace testutils
} // namespace freedom
