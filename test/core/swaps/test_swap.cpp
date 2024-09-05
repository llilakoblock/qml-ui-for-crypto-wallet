#include "swap_accept.pb.h"
#include "swap_decline.pb.h"
#include "swap_request.pb.h"
#include "swaps/swap.h"

#include <gtest/gtest.h>

TEST(Swap, ToJsonFromJson)
{
	using namespace freedom::peertopeer;

	const auto compare_swaps = [](const auto& swap)
	{
		nlohmann::json swap_json_1;
		to_json(swap_json_1, swap);

		Swap swap_2;
		from_json(swap_json_1, swap_2);
		nlohmann::json swap_json_2;
		to_json(swap_json_2, swap_2);

		EXPECT_EQ(swap, swap_2);
		EXPECT_EQ(swap_json_1, swap_json_2);
	};

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

	Swap swap(order, Swap::Role::Alice);
	compare_swaps(swap);

	swap.SetState(Swap::State::ReadyForRedeem);
	compare_swaps(swap);
	swap.SetState(Swap::State::Empty);
	compare_swaps(swap);
	swap.SetState(Swap::State::ReadyForFund);
	compare_swaps(swap);
	swap.SetState(Swap::State::Funding);
	compare_swaps(swap);
	swap.SetState(Swap::State::FundFailed);
	compare_swaps(swap);
	swap.SetState(Swap::State::FundSucceed);
	compare_swaps(swap);
	swap.SetState(Swap::State::ReadyForRedeem);
	compare_swaps(swap);
	swap.SetState(Swap::State::Redeeming);
	compare_swaps(swap);
	swap.SetState(Swap::State::RedeemFailed);
	compare_swaps(swap);
	swap.SetState(Swap::State::RedeemSucceed);
	compare_swaps(swap);
	swap.SetState(Swap::State::ReadyForRefund);
	compare_swaps(swap);
	swap.SetState(Swap::State::Refunding);
	compare_swaps(swap);
	swap.SetState(Swap::State::RefundFailed);
	compare_swaps(swap);
	swap.SetState(Swap::State::RefundSucceed);
	compare_swaps(swap);
	swap.SetState(Swap::State::Finished);
	compare_swaps(swap);
	swap.SetState(Swap::State::Error);
	compare_swaps(swap);

	swap.SetWantRefund(true);
	compare_swaps(swap);
	swap.SetWantRefund(false);
	compare_swaps(swap);

	swap.SwapRequestReceived(SwapRequest());
	compare_swaps(swap);
	swap.SwapAcceptReceived(SwapAccept());
	compare_swaps(swap);
	swap.SwapDeclineReceived(SwapDecline());
	compare_swaps(swap);

	swap.GetAlphaBlockchainStateMutable().deploy_tx = "deploy_tx";
	swap.GetAlphaBlockchainStateMutable().fund_tx = "fund_tx";
	swap.GetAlphaBlockchainStateMutable().htlc_location =
		BlockchainState::BtcLikeHtlcLocation{"txid", "htlc_address", -654654};
	swap.GetAlphaBlockchainStateMutable().redeem_tx = "redeem_tx";
	swap.GetAlphaBlockchainStateMutable().refund_tx = "refund_tx";
	compare_swaps(swap);

	swap.GetAlphaBlockchainStateMutable().htlc_location =
		BlockchainState::EthLikeHtlcLocation{"txid", "htlc_address"};
	compare_swaps(swap);

	swap.GetBetaBlockchainStateMutable().deploy_tx = "deploy_tx";
	swap.GetBetaBlockchainStateMutable().fund_tx = "fund_tx";
	swap.GetBetaBlockchainStateMutable().htlc_location =
		BlockchainState::BtcLikeHtlcLocation{"txid", "htlc_address", -654654};
	swap.GetBetaBlockchainStateMutable().redeem_tx = "redeem_tx";
	swap.GetBetaBlockchainStateMutable().refund_tx = "refund_tx";
	compare_swaps(swap);

	swap.GetBetaBlockchainStateMutable().htlc_location = BlockchainState::EthLikeHtlcLocation{"txid", "htlc_address"};
	compare_swaps(swap);

	swap.GetCommunicationMutable().alpha_expiry = 65984651654;
	swap.GetCommunicationMutable().alpha_redeem_identity = "alpha_redeem_identity";
	swap.GetCommunicationMutable().alpha_refund_identity = "alpha_refund_identity";
	swap.GetCommunicationMutable().beta_expiry = 65984651654;
	swap.GetCommunicationMutable().beta_redeem_identity = "beta_redeem_identity";
	swap.GetCommunicationMutable().beta_refund_identity = "beta_refund_identity";
	swap.GetCommunicationMutable().secret_hash = "secret_hash";
	compare_swaps(swap);

	EXPECT_EQ(swap.GetLastLog(), nullptr);

	swap.AppendLog(SwapLog(SwapLog::None, "None"));
	compare_swaps(swap);
	EXPECT_NE(swap.GetLastLog(), nullptr);

	swap.AppendLog(SwapLog(SwapLog::InProgress, "InProgress"));
	compare_swaps(swap);
	EXPECT_NE(swap.GetLastLog(), nullptr);

	swap.AppendLog(SwapLog(SwapLog::Done, "Done"));
	compare_swaps(swap);
	EXPECT_NE(swap.GetLastLog(), nullptr);

	swap.AppendLog(SwapLog(SwapLog::Failed, "Failed"));
	compare_swaps(swap);
	EXPECT_NE(swap.GetLastLog(), nullptr);

	swap.UpdateLastLog(SwapLog(SwapLog::Done, "Done Done Done!"));
	compare_swaps(swap);
	ASSERT_NE(swap.GetLastLog(), nullptr);

	swap.GetLastLog()->SetState(SwapLog::InProgress);
	compare_swaps(swap);
	EXPECT_NE(swap.GetLastLog(), nullptr);
}
