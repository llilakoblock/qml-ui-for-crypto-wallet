#include "etherscan/etherscandatatypes/types/erc20history.h"

#include <gtest/gtest.h>

TEST(EtherscanObserverErc20HistoryElement, ToJsonFromJson)
{
	using namespace freedom::etherscan::erc20_history;

	const Erc20HistoryElement elem_1{};
	nlohmann::json elem_json_1;
	to_json(elem_json_1, elem_1);

	Erc20HistoryElement elem_2{};
	from_json(elem_json_1, elem_2);
	nlohmann::json elem_json_2;
	to_json(elem_json_2, elem_2);

	EXPECT_EQ(elem_1, elem_2);
	EXPECT_EQ(elem_json_1, elem_json_2);

	Erc20HistoryElement elem_3;
	elem_3.block_number = "12345789";
	elem_3.time_stamp = "123456789";
	elem_3.hash = "hash";
	elem_3.nonce = "nonce";
	elem_3.block_hash = "block_hash";
	elem_3.from = "from";
	elem_3.contract_address = "contract_address";
	elem_3.to = "to";
	elem_3.value = "value";
	elem_3.token_name = "token_name";
	elem_3.token_symbol = "token_symbol";
	elem_3.token_decimal = "token_decimal";
	elem_3.transaction_index = "transaction_index";
	elem_3.gas = "gas";
	elem_3.gas_price = "gas_price";
	elem_3.gas_used = "gas_used";
	elem_3.cumulative_gas_used = "cumulative_gas_used";
	elem_3.input = "input";
	elem_3.confirmations = "confirmations";

	nlohmann::json elem_json_3;
	to_json(elem_json_3, elem_3);

	Erc20HistoryElement elem_4{};
	from_json(elem_json_3, elem_4);
	nlohmann::json elem_json_4;
	to_json(elem_json_4, elem_4);

	EXPECT_EQ(elem_3, elem_4);
	EXPECT_EQ(elem_json_3, elem_json_4);
}
