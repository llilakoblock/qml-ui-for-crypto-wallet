#include "electrumobserver/electrumdatatypes/types/transactionverbose.hpp"

#include <gtest/gtest.h>

TEST(ElectrumObserverTransactionVerbose, ToJsonFromJson)
{
	using namespace freedom::electrum::transaction_verbose;

	const TransactionVerbose transaction_1{};
	nlohmann::json transaction_json_1;
	to_json(transaction_json_1, transaction_1);

	TransactionVerbose transaction_2{};
	from_json(transaction_json_1, transaction_2);
	nlohmann::json transaction_json_2;
	to_json(transaction_json_2, transaction_2);

	EXPECT_EQ(transaction_1, transaction_2);
	EXPECT_EQ(transaction_json_1, transaction_json_2);

	TransactionVerbose transaction_3;
	transaction_3.txid = "txid";
	transaction_3.hash = "qwerty123456789";
	transaction_3.version = 789;
	transaction_3.size = -4264654;
	transaction_3.locktime = -1;
	{
		Vin vin;
		vin.txid = "txid";
		vin.vout = 657897;
		vin.script_sig = ScriptSig{"qwe", "asd"};
		vin.sequence = -456789;
		transaction_3.vin = {vin, vin, vin};
	}
	{
		Vout vout;
		vout.value = 3.14;
		vout.n = 123456789;

		ScriptPubKey key;
		key.script_pub_key_asm = "script_pub_key_asm";
		key.hex = "qwerty";
		key.req_sigs = 456987123;
		key.type = "Type";
		key.addresses = std::vector<std::string>{"qwe", "asd", "zxc"};

		vout.script_pub_key = key;

		transaction_3.vout = {vout, vout, vout};
	}
	transaction_3.hex = "0x0ABCD1123456";
	transaction_3.blockhash = "213225344";
	transaction_3.confirmations = 10;
	transaction_3.time = 123456789;
	transaction_3.blocktime = 777777;
	transaction_3.vsize = 156268;
	transaction_3.weight = 531468465;

	nlohmann::json transaction_json_3;
	to_json(transaction_json_3, transaction_3);

	TransactionVerbose transaction_4{};
	from_json(transaction_json_3, transaction_4);
	nlohmann::json transaction_json_4;
	to_json(transaction_json_4, transaction_4);

	EXPECT_EQ(transaction_3, transaction_4);
	EXPECT_EQ(transaction_json_3, transaction_json_4);
}
