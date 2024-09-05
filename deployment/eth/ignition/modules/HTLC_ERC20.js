const { buildModule } = require("@nomicfoundation/hardhat-ignition/modules");

module.exports = buildModule("HtlcErc20Module", (m) => {
  const token = m.contract("HTLC_ERC20");

  return { token };
});
