#include "etherscan/etherscandatatypes/types/ethinternalhistory.h"

#include <gtest/gtest.h>

TEST(EtherscanObserverEthInternalHistoryElement, ToJsonFromJson)
{
	using namespace freedom::etherscan::eth_internal_history;

	const EthInternalHistoryElement elem_1{};
	nlohmann::json elem_json_1;
	to_json(elem_json_1, elem_1);

	EthInternalHistoryElement elem_2{};
	from_json(elem_json_1, elem_2);
	nlohmann::json elem_json_2;
	to_json(elem_json_2, elem_2);

	EXPECT_EQ(elem_1, elem_2);
	EXPECT_EQ(elem_json_1, elem_json_2);

	EthInternalHistoryElement elem_3;
	elem_3.block_number = "block_number";
	elem_3.time_stamp = "time_stamp";
	elem_3.hash = "hash";
	elem_3.from = "from";
	elem_3.to = "to";
	elem_3.value = "value";
	elem_3.contract_address = "contract_address";
	elem_3.input = "input";
	elem_3.type = "type";
	elem_3.gas = "gas";
	elem_3.gas_used = "gas_used";
	elem_3.trace_id = "trace_id";
	elem_3.is_error = "is_error";
	elem_3.err_code = "err_code";

	nlohmann::json elem_json_3;
	to_json(elem_json_3, elem_3);

	EthInternalHistoryElement elem_4{};
	from_json(elem_json_3, elem_4);
	nlohmann::json elem_json_4;
	to_json(elem_json_4, elem_4);

	EXPECT_EQ(elem_3, elem_4);
	EXPECT_EQ(elem_json_3, elem_json_4);
}
