pragma solidity ^0.8.25;

contract HTLC {
    // Events
    event funded(bytes32 indexed exchangeId, address indexed receiver, uint256 amount);
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

    function fund(bytes32 hashlock, uint timelock, address receiverAddress) public payable
    {
        require(msg.value > 0, "Amount must be greater than 0");
        require(timelock > block.timestamp, "Unlock time must be in the future");
        require(receiverAddress != address(0), "Invalid receiver address");

        bytes32 exchangeId = sha256(abi.encodePacked(msg.sender, receiverAddress, hashlock));

        require(exchanges[exchangeId].state == ExchangeState.Invalid, "Duplicate exchange");

        exchanges[exchangeId] = Exchange(
            hashlock,
            timelock,
            msg.value,
            msg.sender,
            receiverAddress,
            ExchangeState.Funded
        );

        emit funded(exchangeId, receiverAddress, msg.value);
    }

    function redeem(bytes32 exchangeId, bytes32 secret) public
    {
        Exchange storage exchange = exchanges[exchangeId];

        require(exchange.state == ExchangeState.Funded, "Invalid exchange state");
        require(exchange.receiver == msg.sender, "Only receiver can redeem");
        require(sha256(abi.encodePacked(secret)) == exchange.hashlock, "Invalid secret");

        (bool sent, ) = payable(msg.sender).call{value: exchange.value}("");

        require(sent, "Failed to send Ether");

        exchange.state = ExchangeState.Redeemed;

        emit redeemed(exchangeId, secret);
    }

    function refund(bytes32 exchangeId) public
    {
        Exchange storage exchange = exchanges[exchangeId];

        require(exchange.state == ExchangeState.Funded, "Invalid exchange state");
        require(block.timestamp >= exchange.timelock, "Exchange not expired");
        require(msg.sender == exchange.sender, "Only sender can refund");

        (bool sent, ) = payable(msg.sender).call{value: exchange.value}("");

        require(sent, "Failed to send Ether");

        exchange.state = ExchangeState.Refunded;

        emit refunded(exchangeId);
    }
}
