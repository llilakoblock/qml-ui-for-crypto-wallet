#include "nlohmann/json.hpp"
#include "swaps/swaplog.h"

#include <gtest/gtest.h>

TEST(SwapLog, ToJsonFromJson)
{
	using namespace freedom::peertopeer;

	const SwapLog log1;
	nlohmann::json json1;
	to_json(json1, log1);

	SwapLog log2;
	from_json(json1, log2);
	EXPECT_EQ(log1, log2);

	const SwapLog log3(SwapLog::None, "None");
	const SwapLog log4(SwapLog::InProgress, "InProgress");
	const SwapLog log5(SwapLog::Done, "Done");
	const SwapLog log6(SwapLog::Failed, "Failed");
	nlohmann::json json3, json4, json5, json6;
	to_json(json3, log3);
	to_json(json4, log4);
	to_json(json5, log5);
	to_json(json6, log6);

	SwapLog log7, log8, log9, log10;
	from_json(json3, log7);
	from_json(json4, log8);
	from_json(json5, log9);
	from_json(json6, log10);

	EXPECT_EQ(log3, log7);
	EXPECT_EQ(log4, log8);
	EXPECT_EQ(log5, log9);
	EXPECT_EQ(log6, log10);
}
