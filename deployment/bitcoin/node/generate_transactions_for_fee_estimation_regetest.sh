#!/usr/bin/env bash
# jq tool is needed
# bc tool is needed
# generatetoaddress insert any valid address
# /opt/bitcoin/current/bin/bitcoin-cli --conf=/blockchain/bitcoin/data/bitcoin.conf loadwallet before running any of this scripts

set -eo pipefail

cont=true

smartfee=$(/opt/bitcoin/current/bin/bitcoin-cli --conf=/blockchain/bitcoin/data/bitcoin.conf estimatesmartfee 6)
if [[ "$smartfee" == *"\"feerate\":"* ]]; then
    cont=false
fi
while true
do
    counterb=0
    range=$(( $RANDOM % (10 - 1 + 1) + 1 ))
    while [ $counterb -lt $range ]
    do
        power=$(( $RANDOM % 29 ))
        randfee=`echo "scale=8; 0.00001 * (1.1892 ^ $power)" | bc`
        newaddress=$(/opt/bitcoin/current/bin/bitcoin-cli --conf=/blockchain/bitcoin/data/bitcoin.conf getnewaddress)
        rawtx=$(/opt/bitcoin/current/bin/bitcoin-cli --conf=/blockchain/bitcoin/data/bitcoin.conf createrawtransaction "[]" "[{\"$newaddress\":0.005}]")
        fundedtx=$(/opt/bitcoin/current/bin/bitcoin-cli --conf=/blockchain/bitcoin/data/bitcoin.conf fundrawtransaction "$rawtx" "{\"feeRate\": \"0$randfee\"}" | jq -r ".hex")
        signedtx=$(/opt/bitcoin/current/bin/bitcoin-cli --conf=/blockchain/bitcoin/data/bitcoin.conf signrawtransactionwithwallet "$fundedtx" | jq -r ".hex")
        senttx=$(/opt/bitcoin/current/bin/bitcoin-cli --conf=/blockchain/bitcoin/data/bitcoin.conf sendrawtransaction "$signedtx")
        ((++counterb))
        echo "Created $counterb transactions this block"
    done
    /opt/bitcoin/current/bin/bitcoin-cli --conf=/blockchain/bitcoin/data/bitcoin.conf generatetoaddress 1 "bcrt1q2t7qtcfu3fm4qcx0mj5ks4th3lcjyj3m5qfnjd"
    smartfee=$(/opt/bitcoin/current/bin/bitcoin-cli --conf=/blockchain/bitcoin/data/bitcoin.conf estimatesmartfee 6)
    if [[ "$smartfee" == *"\"feerate\":"* ]]; then
        cont=false
    fi
    sleep 180
done

/opt/bitcoin/current/bin/bitcoin-cli --conf=/blockchain/bitcoin/data/bitcoin.conf generatetoaddress 6 "bcrt1q2t7qtcfu3fm4qcx0mj5ks4th3lcjyj3m5qfnjd"
sleep 180