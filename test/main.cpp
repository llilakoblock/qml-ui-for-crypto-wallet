#include "environment.h"
#include "freedom_logger.h"

#include <gtest/gtest.h>

#include <QApplication>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	freedom::logger::FreedomLogger logger(6);
	::testing::InitGoogleTest(&argc, argv);
	::testing::AddGlobalTestEnvironment(new freedom::testutils::Environment());
	return RUN_ALL_TESTS();
}
