pragma solidity ^0.8.25;

import "./TetherToken.sol";

contract HTLC_ERC20 {
    // Events
    event funded(bytes32 indexed exchangeId, address indexed tokenAddress, address indexed receiver, uint256 amount);
    event redeemed(bytes32 indexed exchangeId, bytes32 secret);
    event refunded(bytes32 indexed exchangeId);

    // Exchange state
    enum ExchangeState {
        Invalid,
        Funded,
        Redeemed,
        Refunded,
        Expired
    }

    struct Exchange {
        bytes32 hashlock;
        uint timelock;
        uint value;
        address token;
        address sender;
        address receiver;
        ExchangeState state;
    }

    mapping (bytes32 => Exchange) public exchanges;

    function state(bytes32 exchangeId) public view returns (ExchangeState)
    {
        Exchange storage exchange = exchanges[exchangeId];

        if (exchange.timelock < block.timestamp) {
            return ExchangeState.Expired;
        }

        return exchange.state;
    }

    function fund(bytes32 hashlock, uint256 timelock, uint256 amount, address tokenAddress, address receiverAddress) public
    {
        require(timelock > block.timestamp, "Unlock time must be in the future");
        require(amount > 0, "Amount must be greater than 0");
        require(tokenAddress != address(0), "Invalid token address");
        require(receiverAddress != address(0), "Invalid receiver address");
        require(TetherToken(tokenAddress).allowance(msg.sender, address(this)) >= amount, "Insufficient allowance");

        bytes32 exchangeId = sha256(abi.encodePacked(msg.sender, tokenAddress, receiverAddress, hashlock));

        require(exchanges[exchangeId].state == ExchangeState.Invalid, "Duplicate exchange");

        TetherToken(tokenAddress).transferFrom(msg.sender, address(this), amount);

        exchanges[exchangeId] = Exchange(
            hashlock,
            timelock,
            amount,
            tokenAddress,
            msg.sender,
            receiverAddress,
            ExchangeState.Funded
        );

        emit funded(exchangeId, tokenAddress, receiverAddress, amount);
    }

    function redeem(bytes32 exchangeId, bytes32 secret) public
    {
        Exchange storage exchange = exchanges[exchangeId];

        require(exchange.state == ExchangeState.Funded, "Invalid exchange state");
        require(exchange.receiver == msg.sender, "Only receiver can redeem");
        require(sha256(abi.encodePacked(secret)) == exchange.hashlock, "Invalid secret");

        TetherToken(exchange.token).transfer(msg.sender, exchange.value);

        exchange.state = ExchangeState.Redeemed;

        emit redeemed(exchangeId, secret);
    }

    function refund(bytes32 exchangeId) public
    {
        Exchange storage exchange = exchanges[exchangeId];

        require(exchange.state == ExchangeState.Funded, "Invalid exchange state");
        require(block.timestamp >= exchange.timelock, "Exchange not expired");
        require(msg.sender == exchange.sender, "Only sender can refund");

        TetherToken(exchange.token).transfer(msg.sender, exchange.value);

        exchange.state = ExchangeState.Refunded;

        emit refunded(exchangeId);
    }
}
