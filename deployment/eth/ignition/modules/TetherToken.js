const { buildModule } = require("@nomicfoundation/hardhat-ignition/modules");

const INITIAL_SUPLY = 1_000_000_000_000_000n;
const NAME = "TetherToken";
const SYMBOL = "USDT";
const DECIMALS = 2;

module.exports = buildModule("TetherTokenModule", (m) => {
  const token = m.contract("TetherToken", [
    INITIAL_SUPLY,
    NAME,
    SYMBOL,
    DECIMALS,
  ]);

  return { token };
});
