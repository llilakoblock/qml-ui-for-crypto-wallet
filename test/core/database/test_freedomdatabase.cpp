#include "addresshistory.h"
#include "electrumobserver/electrumdatatypes/types/transactionverbose.hpp"
#include "etherscan/etherscandatatypes/types/erc20history.h"
#include "etherscan/etherscandatatypes/types/ethhistory.h"
#include "etherscan/etherscandatatypes/types/ethinternalhistory.h"
#include "generic_p2p_message.pb.h"
#include "scopeddatabase.h"
#include "sqlscopedlock.h"
#include "swaps/swap.h"
#include "freedomdatabase.h"

#include <gtest/gtest.h>

TEST(FreedomDatabase, CreateRemoveDatabase)
{
	using namespace freedom::testutils;
	[[maybe_unused]] ScopedDatabase db(*test_info_);
}

TEST(FreedomDatabase, AddUpdateTransactionHistory)
{
	using namespace freedom::testutils;
	using namespace freedom::core;
	using namespace freedom::core::database;

	[[maybe_unused]] ScopedDatabase db(*test_info_);
	FreedomDatabase database;

	std::optional<AddressHistoryEntry> history_entry_1;
	EXPECT_TRUE(database.GetTransactionHistory("tx_hash", history_entry_1));
	EXPECT_FALSE(history_entry_1.has_value());

	AddressHistoryEntry history_entry_2;
	history_entry_2.txhash = "4s56f4sdf98g4";
	history_entry_2.amount = "654654654.16545435545465";
	history_entry_2.currency_name = "BTC";
	history_entry_2.direction = AddressHistoryEntry::Direction::TXIN;
	history_entry_2.from = std::vector<std::string>{"qwe", "asd", "zxc"};
	history_entry_2.to = std::vector<std::string>{"qwe", "asd", "zxc"};
	history_entry_2.timestamp = 65431654654;
	history_entry_2.success = false;

	EXPECT_TRUE(database.AddUpdateTransactionHistory(history_entry_2));

	EXPECT_TRUE(database.GetTransactionHistory("tx_hash", history_entry_1));
	EXPECT_FALSE(history_entry_1.has_value());

	EXPECT_TRUE(database.GetTransactionHistory(history_entry_2.txhash, history_entry_1));
	ASSERT_TRUE(history_entry_1.has_value());
	EXPECT_EQ(*history_entry_1, history_entry_2);

	history_entry_2.direction = AddressHistoryEntry::Direction::TXOUT;
	history_entry_2.success = true;

	EXPECT_TRUE(database.AddUpdateTransactionHistory(history_entry_2));

	history_entry_1 = std::nullopt;
	EXPECT_TRUE(database.GetTransactionHistory(history_entry_2.txhash, history_entry_1));
	ASSERT_TRUE(history_entry_1.has_value());
	EXPECT_EQ(*history_entry_1, history_entry_2);
}

TEST(FreedomDatabase, GetAllTransactionHistory)
{
	using namespace freedom::testutils;
	using namespace freedom::core;
	using namespace freedom::core::database;

	std::srand(std::time(nullptr));

	[[maybe_unused]] ScopedDatabase db(*test_info_);
	FreedomDatabase database;

	constexpr int size = 150;

	AddressHistory address_history;
	address_history.history.reserve(size);

	for (int i = 0; i < size; ++i)
	{
		AddressHistoryEntry history_entry;
		history_entry.txhash = QString::number(i).toStdString();
		history_entry.amount = QString::number(std::rand()).toStdString();
		history_entry.currency_name = QString::number(std::rand()).toStdString();
		history_entry.direction =
			(std::rand() % 2) ? AddressHistoryEntry::Direction::TXIN : AddressHistoryEntry::Direction::TXOUT;
		history_entry.from = std::vector<std::string>{QString::number(std::rand()).toStdString(),
			QString::number(std::rand()).toStdString(), QString::number(std::rand()).toStdString()};
		history_entry.to = std::vector<std::string>{QString::number(std::rand()).toStdString(),
			QString::number(std::rand()).toStdString(), QString::number(std::rand()).toStdString()};
		history_entry.timestamp = i;
		history_entry.success = static_cast<bool>(std::rand() % 2);

		address_history.history.emplace_back(std::move(history_entry));
	}

	std::sort(std::begin(address_history.history), std::end(address_history.history),
		[](const auto& lhs, const auto& rhs)
		{
			return (lhs.timestamp < rhs.timestamp);
		});

	EXPECT_TRUE(database.AddUpdateTransactionHistory(address_history));

	std::optional<AddressHistory> result;
	EXPECT_TRUE(database.GetAllTransactionHistory(result));
	ASSERT_TRUE(result.has_value());

	std::sort(std::begin(result->history), std::end(result->history),
		[](const auto& lhs, const auto& rhs)
		{
			return (lhs.timestamp < rhs.timestamp);
		});

	EXPECT_EQ(address_history, *result);

	EXPECT_TRUE(database.AddUpdateTransactionHistory(address_history));

	result = std::nullopt;
	EXPECT_TRUE(database.GetAllTransactionHistory(result));
	ASSERT_TRUE(result.has_value());

	std::sort(std::begin(result->history), std::end(result->history),
		[](const auto& lhs, const auto& rhs)
		{
			return (lhs.timestamp < rhs.timestamp);
		});

	EXPECT_EQ(address_history, *result);
}

TEST(FreedomDatabase, AddUpdateBtcLikeTransactionVerbose)
{
	using namespace freedom::testutils;
	using namespace freedom::core;
	using namespace freedom::core::database;
	using namespace freedom::electrum::transaction_verbose;

	[[maybe_unused]] ScopedDatabase db(*test_info_);
	FreedomDatabase database;

	std::optional<TransactionVerbose> transaction_1;
	EXPECT_TRUE(database.GetBtcLikeTransactionVerbose("tx_hash", transaction_1));
	EXPECT_FALSE(transaction_1.has_value());

	TransactionVerbose transaction_2;
	transaction_2.txid = "txid";
	transaction_2.hash = "qwerty123456789";
	transaction_2.version = 789;
	transaction_2.size = -4264654;
	transaction_2.locktime = -1;
	{
		Vin vin;
		vin.txid = "txid";
		vin.vout = 657897;
		vin.script_sig = ScriptSig{"qwe", "asd"};
		vin.sequence = -456789;
		transaction_2.vin = {vin, vin, vin};
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

		transaction_2.vout = {vout, vout, vout};
	}
	transaction_2.hex = "0x0ABCD1123456";
	transaction_2.blockhash = "213225344";
	transaction_2.confirmations = 10;
	transaction_2.time = 123456789;
	transaction_2.blocktime = 777777;

	EXPECT_TRUE(database.AddUpdateBtcLikeTransactionVerbose(transaction_2));

	EXPECT_TRUE(database.GetBtcLikeTransactionVerbose("1234567897894653123", transaction_1));
	EXPECT_FALSE(transaction_1.has_value());

	EXPECT_TRUE(database.GetBtcLikeTransactionVerbose(transaction_2.txid, transaction_1));
	ASSERT_TRUE(transaction_1.has_value());
	EXPECT_EQ(*transaction_1, transaction_2);

	transaction_2.hash = "a6s5d46s5f465dasfg465adv65a4fg6a54dvz5cx6v4z65f4gzf1gzdf";
	transaction_2.version = 215644651;
	transaction_2.size = 0;

	EXPECT_TRUE(database.AddUpdateBtcLikeTransactionVerbose(transaction_2));

	transaction_1 = std::nullopt;
	EXPECT_TRUE(database.GetBtcLikeTransactionVerbose(transaction_2.txid, transaction_1));
	ASSERT_TRUE(transaction_1.has_value());
	EXPECT_EQ(*transaction_1, transaction_2);
}

TEST(FreedomDatabase, AddUpdateEthLikeTransactionVerbose)
{
	using namespace freedom::testutils;
	using namespace freedom::core;
	using namespace freedom::core::database;
	using namespace freedom::etherscan::eth_history;

	[[maybe_unused]] ScopedDatabase db(*test_info_);
	FreedomDatabase database;

	std::optional<EthHistoryElementVariant> elem_1;
	EXPECT_TRUE(database.GetEthLikeTransactionVerbose("tx_hash", elem_1));
	EXPECT_FALSE(elem_1.has_value());

	EthHistoryElement elem_2;
	elem_2.block_number = "123456789";
	elem_2.block_hash = "block_hash";
	elem_2.time_stamp = "123456789";
	elem_2.hash = "hash";
	elem_2.nonce = "nonce";
	elem_2.transaction_index = "transaction_index";
	elem_2.from = "from";
	elem_2.to = "to";
	elem_2.value = "value";
	elem_2.gas = "gas";
	elem_2.gas_price = "gas_price";
	elem_2.input = "input";
	elem_2.contract_address = "contract_address";
	elem_2.cumulative_gas_used = "cumulative_gas_used";
	elem_2.is_error = "is_error";
	elem_2.txreceipt_status = "txreceipt_status";
	elem_2.gas_used = "gas_used";
	elem_2.confirmations = "confirmations";

	EXPECT_TRUE(database.AddUpdateEthLikeTransactionVerbose(elem_2));

	EXPECT_TRUE(database.GetEthLikeTransactionVerbose("tx_hash", elem_1));
	EXPECT_FALSE(elem_1.has_value());

	EXPECT_TRUE(database.GetEthLikeTransactionVerbose(elem_2.hash, elem_1));
	ASSERT_TRUE(elem_1.has_value());
	EXPECT_EQ(std::get<EthHistoryElement>(*elem_1), elem_2);

	elem_2.block_hash = "fgsfgsdfga";
	elem_2.time_stamp = "123654789987";
	elem_2.hash = "34123452345";
	elem_2.nonce = "adsfasdrgfw5t";
	elem_2.transaction_index = "qwersdfgdrtys5t";
	elem_2.from = "q345ergwertyser";
	elem_2.to = "adfgdsrtydsgsdrg";
	elem_2.value = "2345wertgsr5ysr5g";

	EXPECT_TRUE(database.AddUpdateEthLikeTransactionVerbose(elem_2));

	elem_1 = std::nullopt;
	EXPECT_TRUE(database.GetEthLikeTransactionVerbose(elem_2.hash, elem_1));
	ASSERT_TRUE(elem_1.has_value());
	EXPECT_EQ(std::get<EthHistoryElement>(*elem_1), elem_2);
}

TEST(FreedomDatabase, AddUpdateEthInternalLikeTransactionVerbose)
{
	using namespace freedom::testutils;
	using namespace freedom::core;
	using namespace freedom::core::database;
	using namespace freedom::etherscan::eth_internal_history;

	[[maybe_unused]] ScopedDatabase db(*test_info_);
	FreedomDatabase database;

	std::optional<EthHistoryElementVariant> elem_1;
	EXPECT_TRUE(database.GetEthLikeTransactionVerbose("tx_hash", elem_1));
	EXPECT_FALSE(elem_1.has_value());

	EthInternalHistoryElement elem_2;
	elem_2.block_number = "123456789";
	elem_2.time_stamp = "123456789";
	elem_2.hash = "hash";
	elem_2.from = "from";
	elem_2.to = "to";
	elem_2.value = "value";
	elem_2.contract_address = "contract_address";
	elem_2.input = "input";
	elem_2.type = "type";
	elem_2.gas = "gas";
	elem_2.gas_used = "gas_used";
	elem_2.trace_id = "trace_id";
	elem_2.is_error = "is_error";
	elem_2.err_code = "err_code";

	EXPECT_TRUE(database.AddUpdateEthLikeTransactionVerbose(elem_2));

	EXPECT_TRUE(database.GetEthLikeTransactionVerbose("tx_hash", elem_1));
	EXPECT_FALSE(elem_1.has_value());

	EXPECT_TRUE(database.GetEthLikeTransactionVerbose(elem_2.hash, elem_1));
	ASSERT_TRUE(elem_1.has_value());
	EXPECT_EQ(std::get<EthInternalHistoryElement>(*elem_1), elem_2);

	elem_2.time_stamp = "123654789987";
	elem_2.hash = "34123452345";
	elem_2.from = "q345ergwertyser";
	elem_2.to = "adfgdsrtydsgsdrg";
	elem_2.value = "2345wertgsr5ysr5g";

	EXPECT_TRUE(database.AddUpdateEthLikeTransactionVerbose(elem_2));

	elem_1 = std::nullopt;
	EXPECT_TRUE(database.GetEthLikeTransactionVerbose(elem_2.hash, elem_1));
	ASSERT_TRUE(elem_1.has_value());
	EXPECT_EQ(std::get<EthInternalHistoryElement>(*elem_1), elem_2);
}

TEST(FreedomDatabase, AddUpdateErc20LikeTransactionVerbose)
{
	using namespace freedom::testutils;
	using namespace freedom::core;
	using namespace freedom::core::database;
	using namespace freedom::etherscan::erc20_history;

	[[maybe_unused]] ScopedDatabase db(*test_info_);
	FreedomDatabase database;

	std::optional<EthHistoryElementVariant> elem_1;
	EXPECT_TRUE(database.GetEthLikeTransactionVerbose("tx_hash", elem_1));
	EXPECT_FALSE(elem_1.has_value());

	Erc20HistoryElement elem_2;
	elem_2.block_number = "12345789";
	elem_2.time_stamp = "123456789";
	elem_2.hash = "hash";
	elem_2.nonce = "nonce";
	elem_2.block_hash = "block_hash";
	elem_2.from = "from";
	elem_2.contract_address = "contract_address";
	elem_2.to = "to";
	elem_2.value = "value";
	elem_2.token_name = "token_name";
	elem_2.token_symbol = "token_symbol";
	elem_2.token_decimal = "token_decimal";
	elem_2.transaction_index = "transaction_index";
	elem_2.gas = "gas";
	elem_2.gas_price = "gas_price";
	elem_2.gas_used = "gas_used";
	elem_2.cumulative_gas_used = "cumulative_gas_used";
	elem_2.input = "input";
	elem_2.confirmations = "confirmations";

	EXPECT_TRUE(database.AddUpdateEthLikeTransactionVerbose(elem_2));

	EXPECT_TRUE(database.GetEthLikeTransactionVerbose("tx_hash", elem_1));
	EXPECT_FALSE(elem_1.has_value());

	EXPECT_TRUE(database.GetEthLikeTransactionVerbose(elem_2.hash, elem_1));
	ASSERT_TRUE(elem_1.has_value());
	EXPECT_EQ(std::get<Erc20HistoryElement>(*elem_1), elem_2);

	elem_2.block_hash = "fgsfgsdfga";
	elem_2.time_stamp = "123654789987";
	elem_2.hash = "34123452345";
	elem_2.nonce = "adsfasdrgfw5t";
	elem_2.transaction_index = "qwersdfgdrtys5t";
	elem_2.from = "q345ergwertyser";
	elem_2.to = "adfgdsrtydsgsdrg";
	elem_2.value = "2345wertgsr5ysr5g";

	EXPECT_TRUE(database.AddUpdateEthLikeTransactionVerbose(elem_2));

	elem_1 = std::nullopt;
	EXPECT_TRUE(database.GetEthLikeTransactionVerbose(elem_2.hash, elem_1));
	ASSERT_TRUE(elem_1.has_value());
	EXPECT_EQ(std::get<Erc20HistoryElement>(*elem_1), elem_2);
}

TEST(FreedomDatabase, AddUpdateMixedEthLikeTransactionVerbose)
{
	using namespace freedom::testutils;
	using namespace freedom::core;
	using namespace freedom::core::database;
	using namespace freedom::etherscan::eth_history;
	using namespace freedom::etherscan::eth_internal_history;
	using namespace freedom::etherscan::erc20_history;

	[[maybe_unused]] ScopedDatabase db(*test_info_);
	FreedomDatabase database;

	EthHistoryElement elem_1;
	elem_1.hash = "hash";
	elem_1.block_number = "123456789";
	elem_1.block_hash = "block_hash";
	elem_1.time_stamp = "123456789";
	elem_1.nonce = "nonce";
	elem_1.transaction_index = "transaction_index";
	elem_1.from = "from";
	elem_1.to = "to";
	elem_1.value = "value";
	elem_1.gas = "gas";
	elem_1.gas_price = "gas_price";
	elem_1.input = "input";
	elem_1.contract_address = "contract_address";
	elem_1.cumulative_gas_used = "cumulative_gas_used";
	elem_1.is_error = "is_error";
	elem_1.txreceipt_status = "txreceipt_status";
	elem_1.gas_used = "gas_used";
	elem_1.confirmations = "confirmations";

	EthInternalHistoryElement elem_2;
	elem_2.hash = elem_1.hash;
	elem_2.block_number = "123456789";
	elem_2.time_stamp = "123456789";
	elem_2.from = "from";
	elem_2.to = "to";
	elem_2.value = "value";
	elem_2.contract_address = "contract_address";
	elem_2.input = "input";
	elem_2.type = "type";
	elem_2.gas = "gas";
	elem_2.gas_used = "gas_used";
	elem_2.trace_id = "trace_id";
	elem_2.is_error = "is_error";
	elem_2.err_code = "err_code";

	Erc20HistoryElement elem_3;
	elem_3.hash = elem_1.hash;
	elem_3.block_number = "12345789";
	elem_3.time_stamp = "123456789";
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

	// Get inexistent elems
	std::optional<EthHistoryElementVariant> elem_4;
	EXPECT_TRUE(database.GetEthLikeTransactionVerbose(elem_1.hash, elem_4));
	EXPECT_FALSE(elem_4.has_value());
	EXPECT_TRUE(database.GetEthLikeTransactionVerbose(elem_2.hash, elem_4));
	EXPECT_FALSE(elem_4.has_value());
	EXPECT_TRUE(database.GetEthLikeTransactionVerbose(elem_3.hash, elem_4));
	EXPECT_FALSE(elem_4.has_value());

	// Insert elem_1
	EXPECT_TRUE(database.AddUpdateEthLikeTransactionVerbose(elem_1));
	EXPECT_TRUE(database.GetEthLikeTransactionVerbose(elem_1.hash, elem_4));
	ASSERT_TRUE(elem_4.has_value());
	EXPECT_EQ(elem_1, std::get<EthHistoryElement>(*elem_4));

	// Update elem_1
	elem_4 = std::nullopt;
	EXPECT_TRUE(database.AddUpdateEthLikeTransactionVerbose(elem_1));
	EXPECT_TRUE(database.GetEthLikeTransactionVerbose(elem_1.hash, elem_4));
	ASSERT_TRUE(elem_4.has_value());
	EXPECT_EQ(elem_1, std::get<EthHistoryElement>(*elem_4));

	// Update to elem_2
	elem_4 = std::nullopt;
	EXPECT_TRUE(database.AddUpdateEthLikeTransactionVerbose(elem_2));
	EXPECT_TRUE(database.GetEthLikeTransactionVerbose(elem_2.hash, elem_4));
	ASSERT_TRUE(elem_4.has_value());
	EXPECT_EQ(elem_2, std::get<EthInternalHistoryElement>(*elem_4));

	// Update elem_2
	elem_4 = std::nullopt;
	EXPECT_TRUE(database.AddUpdateEthLikeTransactionVerbose(elem_2));
	EXPECT_TRUE(database.GetEthLikeTransactionVerbose(elem_2.hash, elem_4));
	ASSERT_TRUE(elem_4.has_value());
	EXPECT_EQ(elem_2, std::get<EthInternalHistoryElement>(*elem_4));

	// Update to elem_3
	elem_4 = std::nullopt;
	EXPECT_TRUE(database.AddUpdateEthLikeTransactionVerbose(elem_3));
	EXPECT_TRUE(database.GetEthLikeTransactionVerbose(elem_3.hash, elem_4));
	ASSERT_TRUE(elem_4.has_value());
	EXPECT_EQ(elem_3, std::get<Erc20HistoryElement>(*elem_4));

	// Update elem_3
	elem_4 = std::nullopt;
	EXPECT_TRUE(database.AddUpdateEthLikeTransactionVerbose(elem_3));
	EXPECT_TRUE(database.GetEthLikeTransactionVerbose(elem_3.hash, elem_4));
	ASSERT_TRUE(elem_4.has_value());
	EXPECT_EQ(elem_3, std::get<Erc20HistoryElement>(*elem_4));
}

TEST(FreedomDatabase, GetMaxBlockHeightEthLikeTransactionVerbose)
{
	using namespace freedom::testutils;
	using namespace freedom::core;
	using namespace freedom::core::database;
	using namespace freedom::etherscan::eth_history;

	std::srand(std::time(nullptr));

	[[maybe_unused]] ScopedDatabase db(*test_info_);
	FreedomDatabase database;

	EthHistoryElement elem_1;
	elem_1.block_hash = "block_hash";
	elem_1.time_stamp = "123456789";
	elem_1.nonce = "nonce";
	elem_1.transaction_index = "transaction_index";
	elem_1.from = "from";
	elem_1.to = "to";
	elem_1.value = "value";
	elem_1.gas = "gas";
	elem_1.gas_price = "gas_price";
	elem_1.input = "input";
	elem_1.contract_address = "contract_address";
	elem_1.cumulative_gas_used = "cumulative_gas_used";
	elem_1.is_error = "is_error";
	elem_1.txreceipt_status = "txreceipt_status";
	elem_1.gas_used = "gas_used";
	elem_1.confirmations = "confirmations";

	constexpr int size = 100;
	int max_number = 0;
	for (int i = 0; i < size; ++i)
	{
		const int number = std::rand();
		if (number > max_number)
			max_number = number;

		elem_1.block_number = QString::number(number).toStdString();
		elem_1.hash = elem_1.block_number;
		EXPECT_TRUE(database.AddUpdateEthLikeTransactionVerbose(elem_1));
	}

	std::optional<std::string> block_height;
	EXPECT_TRUE(database.GetMaxBlockHeightEthLikeTransactionVerbose(block_height));
	ASSERT_TRUE(block_height);
	EXPECT_EQ(max_number, std::stoi(*block_height));
}

TEST(FreedomDatabase, AddUpdateSwap)
{
	using namespace freedom::testutils;
	using namespace freedom::core;
	using namespace freedom::core::database;
	using namespace freedom::peertopeer;

	[[maybe_unused]] ScopedDatabase db(*test_info_);
	FreedomDatabase database;

	std::optional<Swap> swap_1;
	EXPECT_TRUE(database.GetSwap("order_id", swap_1));
	EXPECT_FALSE(swap_1.has_value());

	Swap swap_2;

	EXPECT_TRUE(database.AddUpdateSwap(swap_2));

	EXPECT_TRUE(database.GetSwap("order_id", swap_1));
	EXPECT_FALSE(swap_1.has_value());

	EXPECT_TRUE(database.GetSwap(swap_2.id, swap_1));
	ASSERT_TRUE(swap_1.has_value());
	EXPECT_EQ(*swap_1, swap_2);

	Order order;
	order.id = "32165498798654";
	order.valid_until = 12345789;
	order.is_accepted = true;
	order.is_swapping = false;
	{
		Asset::Ledger ledger;
		ledger.name = "ledger";
		ledger.network = "daf6g4ad65fasdfg";
		ledger.chain_id = 6576654;

		Asset asset;
		asset.ledger = ledger;
		asset.name = "asset";
		asset.nominal_amount = "nominal_amount";

		order.alpha_asset = asset;
		order.beta_asset = asset;
	}
	{
		Order::Participant participant;
		participant.id = "participant";
		participant.network_identity.id = "network_identity";

		order.maker = participant;
		order.taker = participant;
	}

	Swap swap_3(order, Swap::Role::Alice);
	swap_3.SetState(Swap::State::ReadyForRedeem);
	swap_3.SetWantRefund(true);
	swap_3.SwapRequestReceived(SwapRequest());
	swap_3.SwapAcceptReceived(SwapAccept());
	swap_3.SwapDeclineReceived(SwapDecline());

	swap_3.GetAlphaBlockchainStateMutable().deploy_tx = "deploy_tx";
	swap_3.GetAlphaBlockchainStateMutable().fund_tx = "fund_tx";
	swap_3.GetAlphaBlockchainStateMutable().htlc_location =
		BlockchainState::BtcLikeHtlcLocation{"txid", "htlc_address", -654654};
	swap_3.GetAlphaBlockchainStateMutable().redeem_tx = "redeem_tx";
	swap_3.GetAlphaBlockchainStateMutable().refund_tx = "refund_tx";

	swap_3.GetBetaBlockchainStateMutable().deploy_tx = "deploy_tx";
	swap_3.GetBetaBlockchainStateMutable().fund_tx = "fund_tx";
	swap_3.GetBetaBlockchainStateMutable().htlc_location =
		BlockchainState::EthLikeHtlcLocation{"txid", "htlc_address"};
	swap_3.GetBetaBlockchainStateMutable().redeem_tx = "redeem_tx";
	swap_3.GetBetaBlockchainStateMutable().refund_tx = "refund_tx";

	swap_3.GetCommunicationMutable().alpha_expiry = 65984651654;
	swap_3.GetCommunicationMutable().alpha_redeem_identity = "alpha_redeem_identity";
	swap_3.GetCommunicationMutable().alpha_refund_identity = "alpha_refund_identity";
	swap_3.GetCommunicationMutable().beta_expiry = 65984651654;
	swap_3.GetCommunicationMutable().beta_redeem_identity = "beta_redeem_identity";
	swap_3.GetCommunicationMutable().beta_refund_identity = "beta_refund_identity";
	swap_3.GetCommunicationMutable().secret_hash = "secret_hash";

	swap_3.AppendLog(SwapLog(SwapLog::None, "None"));
	swap_3.AppendLog(SwapLog(SwapLog::InProgress, "InProgress"));
	swap_3.AppendLog(SwapLog(SwapLog::Done, "Done"));
	swap_3.AppendLog(SwapLog(SwapLog::Failed, "Failed"));
	swap_3.AppendLog(SwapLog(SwapLog::Failed, "Failed"));
	swap_3.UpdateLastLog(SwapLog(SwapLog::Done, "Done Done Done!"));
	swap_3.GetLastLog()->SetState(SwapLog::InProgress);

	EXPECT_TRUE(database.AddUpdateSwap(swap_3));

	swap_1 = std::nullopt;
	EXPECT_TRUE(database.GetSwap(swap_3.id, swap_1));
	ASSERT_TRUE(swap_1.has_value());
	EXPECT_EQ(*swap_1, swap_3);
	EXPECT_NE(*swap_1, swap_2);
}

TEST(FreedomDatabase, AddUpdateP2pMessage)
{
	using namespace freedom::testutils;
	using namespace freedom::core;
	using namespace freedom::core::database;
	using namespace freedom::peertopeer::protocol;

	const auto compare_message = [](const auto& lhs, const auto& rhs)
	{
		if (lhs.ByteSizeLong() != rhs.ByteSizeLong())
			return false;

		QByteArray data_1(static_cast<int>(lhs.ByteSizeLong()), 0);
		if (!lhs.SerializeToArray(data_1.data(), data_1.size()))
			return false;

		QByteArray data_2(static_cast<int>(rhs.ByteSizeLong()), 0);
		if (!rhs.SerializeToArray(data_2.data(), data_2.size()))
			return false;

		return (data_1 == data_2);
	};

	[[maybe_unused]] ScopedDatabase db(*test_info_);
	FreedomDatabase database;

	std::optional<GenericP2pMessage> message_1;
	EXPECT_TRUE(database.GetP2pMessage("order_id", swap_accept, message_1));
	EXPECT_FALSE(message_1.has_value());

	GenericP2pMessage message_2;
	EXPECT_FALSE(database.AddUpdateP2pMessage(message_2));

	message_2.set_message_type(swap_accept);
	*message_2.mutable_swap_accept() = SwapAccept();
	message_2.mutable_swap_accept()->set_order_id("order_id");
	EXPECT_TRUE(database.AddUpdateP2pMessage(message_2));

	EXPECT_TRUE(database.GetP2pMessage("order_id", swap_accept, message_1));
	ASSERT_TRUE(message_1.has_value());
	EXPECT_TRUE(compare_message(*message_1, message_2));

	message_2.set_message_type(swap_decline);
	*message_2.mutable_swap_decline() = SwapDecline();
	message_2.mutable_swap_decline()->set_order_id("order_id");
	EXPECT_TRUE(database.AddUpdateP2pMessage(message_2));

	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id", swap_accept, message_1));
	EXPECT_TRUE(message_1.has_value());
	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id", swap_decline, message_1));
	ASSERT_TRUE(message_1.has_value());
	EXPECT_TRUE(compare_message(*message_1, message_2));

	message_2.set_message_type(swap_request);
	*message_2.mutable_swap_request() = SwapRequest();
	message_2.mutable_swap_request()->set_order_id("order_id");
	EXPECT_TRUE(database.AddUpdateP2pMessage(message_2));

	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id", swap_accept, message_1));
	EXPECT_TRUE(message_1.has_value());
	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id", swap_decline, message_1));
	EXPECT_TRUE(message_1.has_value());
	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id", swap_request, message_1));
	ASSERT_TRUE(message_1.has_value());
	EXPECT_TRUE(compare_message(*message_1, message_2));

	message_2.set_message_type(fund_notification);
	*message_2.mutable_fund_notification() = FundNotification();
	message_2.mutable_fund_notification()->set_order_id("order_id");
	EXPECT_TRUE(database.AddUpdateP2pMessage(message_2));

	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id", swap_accept, message_1));
	EXPECT_TRUE(message_1.has_value());
	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id", swap_decline, message_1));
	EXPECT_TRUE(message_1.has_value());
	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id", swap_request, message_1));
	EXPECT_TRUE(message_1.has_value());
	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id", fund_notification, message_1));
	ASSERT_TRUE(message_1.has_value());
	EXPECT_TRUE(compare_message(*message_1, message_2));

	message_2.set_message_type(redeem_notification);
	*message_2.mutable_redeem_notification() = RedeemNotification();
	message_2.mutable_redeem_notification()->set_order_id("order_id");
	EXPECT_TRUE(database.AddUpdateP2pMessage(message_2));

	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id", swap_accept, message_1));
	EXPECT_TRUE(message_1.has_value());
	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id", swap_decline, message_1));
	EXPECT_TRUE(message_1.has_value());
	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id", swap_request, message_1));
	EXPECT_TRUE(message_1.has_value());
	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id", fund_notification, message_1));
	EXPECT_TRUE(message_1.has_value());
	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id", redeem_notification, message_1));
	ASSERT_TRUE(message_1.has_value());
	EXPECT_TRUE(compare_message(*message_1, message_2));

	message_2.set_message_type(refund_notification);
	*message_2.mutable_refund_notification() = RefundNotification();
	message_2.mutable_refund_notification()->set_order_id("order_id");
	EXPECT_TRUE(database.AddUpdateP2pMessage(message_2));

	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id", swap_accept, message_1));
	EXPECT_TRUE(message_1.has_value());
	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id", swap_decline, message_1));
	EXPECT_TRUE(message_1.has_value());
	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id", swap_request, message_1));
	EXPECT_TRUE(message_1.has_value());
	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id", fund_notification, message_1));
	EXPECT_TRUE(message_1.has_value());
	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id", redeem_notification, message_1));
	EXPECT_TRUE(message_1.has_value());
	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id", refund_notification, message_1));
	ASSERT_TRUE(message_1.has_value());
	EXPECT_TRUE(compare_message(*message_1, message_2));

	message_2.set_message_type(refund_notification);
	*message_2.mutable_refund_notification() = RefundNotification();
	message_2.mutable_refund_notification()->set_order_id("order_id_1");
	EXPECT_TRUE(database.AddUpdateP2pMessage(message_2));

	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id_1", swap_accept, message_1));
	EXPECT_FALSE(message_1.has_value());
	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id_1", swap_decline, message_1));
	EXPECT_FALSE(message_1.has_value());
	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id_1", swap_request, message_1));
	EXPECT_FALSE(message_1.has_value());
	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id_1", fund_notification, message_1));
	EXPECT_FALSE(message_1.has_value());
	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id_1", redeem_notification, message_1));
	EXPECT_FALSE(message_1.has_value());
	message_1 = std::nullopt;
	EXPECT_TRUE(database.GetP2pMessage("order_id_1", refund_notification, message_1));
	ASSERT_TRUE(message_1.has_value());
	EXPECT_TRUE(compare_message(*message_1, message_2));
}

TEST(FreedomDatabase, AsyncReadWrite)
{
	using namespace freedom::testutils;
	using namespace freedom::core;
	using namespace freedom::core::database;
	using namespace freedom::etherscan::eth_history;

	[[maybe_unused]] ScopedDatabase db(*test_info_);

	std::srand(std::time(nullptr));

	const auto create_elem = []()
	{
		EthHistoryElement elem;
		elem.block_number = QString::number(std::rand()).toStdString();
		elem.block_number = QString::number(std::rand()).toStdString();
		elem.block_hash = QString::number(std::rand()).toStdString();
		elem.time_stamp = QString::number(std::rand()).toStdString();
		elem.nonce = QString::number(std::rand()).toStdString();
		elem.transaction_index = QString::number(std::rand()).toStdString();
		elem.from = QString::number(std::rand()).toStdString();
		elem.to = QString::number(std::rand()).toStdString();
		elem.value = QString::number(std::rand()).toStdString();
		elem.gas = QString::number(std::rand()).toStdString();
		elem.gas_price = QString::number(std::rand()).toStdString();
		elem.input = QString::number(std::rand()).toStdString();
		elem.contract_address = QString::number(std::rand()).toStdString();
		elem.cumulative_gas_used = QString::number(std::rand()).toStdString();
		elem.is_error = QString::number(std::rand()).toStdString();
		elem.txreceipt_status = QString::number(std::rand()).toStdString();
		elem.gas_used = QString::number(std::rand()).toStdString();
		elem.confirmations = QString::number(std::rand()).toStdString();
		return elem;
	};

	const auto write_elem_global_lock = [create_elem](const std::pair<int, int> range, const int count)
	{
		for (int i = 0; i < count; ++i)
		{
			[[maybe_unused]] const SqlScopedLock database_lock;
			FreedomDatabase database;

			EthHistoryElement elem_1 = create_elem();
			elem_1.hash = QString::number(range.first + std::rand() % (range.second - range.first + 1)).toStdString();
			EXPECT_TRUE(database.AddUpdateEthLikeTransactionVerbose(elem_1));

			std::optional<EthHistoryElementVariant> elem_2;
			EXPECT_TRUE(database.GetEthLikeTransactionVerbose(elem_1.hash, elem_2));
			ASSERT_TRUE(elem_2.has_value());
			EXPECT_EQ(elem_1, std::get<EthHistoryElement>(*elem_2));
		}
	};

	const auto write_elem_local_lock = [create_elem](const std::pair<int, int> range, const int count)
	{
		for (int i = 0; i < count; ++i)
		{
			FreedomDatabase database;

			EthHistoryElement elem_1 = create_elem();
			elem_1.hash = QString::number(range.first + std::rand() % (range.second - range.first + 1)).toStdString();
			EXPECT_TRUE(database.AddUpdateEthLikeTransactionVerbose(elem_1));

			std::optional<EthHistoryElementVariant> elem_2;
			EXPECT_TRUE(database.GetEthLikeTransactionVerbose(elem_1.hash, elem_2));
			ASSERT_TRUE(elem_2.has_value());
		}
	};

	constexpr int count_tests = 300;

	// No intersects
	{
		std::thread thread_1(write_elem_global_lock, std::pair{1, 10}, count_tests);
		std::thread thread_2(write_elem_global_lock, std::pair{11, 20}, count_tests);

		if (thread_1.joinable())
			thread_1.join();

		if (thread_2.joinable())
			thread_2.join();
	}

	// Has intersects
	{
		std::thread thread_1(write_elem_global_lock, std::pair{1, 15}, count_tests);
		std::thread thread_2(write_elem_global_lock, std::pair{5, 20}, count_tests);

		if (thread_1.joinable())
			thread_1.join();

		if (thread_2.joinable())
			thread_2.join();
	}

	// No intersects
	{
		std::thread thread_1(write_elem_local_lock, std::pair{1, 10}, count_tests);
		std::thread thread_2(write_elem_local_lock, std::pair{11, 20}, count_tests);

		if (thread_1.joinable())
			thread_1.join();

		if (thread_2.joinable())
			thread_2.join();
	}

	// Has intersects
	{
		std::thread thread_1(write_elem_local_lock, std::pair{1, 15}, count_tests);
		std::thread thread_2(write_elem_local_lock, std::pair{5, 20}, count_tests);

		if (thread_1.joinable())
			thread_1.join();

		if (thread_2.joinable())
			thread_2.join();
	}

	// Mixed
	{
		std::thread thread_1(write_elem_global_lock, std::pair{1, 10}, count_tests);
		std::thread thread_2(write_elem_global_lock, std::pair{1, 10}, count_tests);
		std::thread thread_3(write_elem_local_lock, std::pair{1, 10}, count_tests);
		std::thread thread_4(write_elem_local_lock, std::pair{1, 10}, count_tests);

		if (thread_1.joinable())
			thread_1.join();

		if (thread_2.joinable())
			thread_2.join();

		if (thread_3.joinable())
			thread_3.join();

		if (thread_4.joinable())
			thread_4.join();
	}
}
