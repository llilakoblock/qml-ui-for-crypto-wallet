const { buildModule } = require("@nomicfoundation/hardhat-ignition/modules");

module.exports = buildModule("HtlcModule", (m) => {
  const token = m.contract("HTLC");

  return { token };
});
