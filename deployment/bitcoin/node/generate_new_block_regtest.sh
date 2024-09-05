# Script to generate a new block every 3 minutes
#!/bin/bash

echo "Generating a block every 3 minutes. Press [CTRL+C] to stop.."

address=`/opt/bitcoin/current/bin/bitcoin-cli --conf=/blockchain/bitcoin/data/bitcoin.conf getnewaddress`

while :
do
        echo "Generate a new block `date '+%d/%m/%Y %H:%M:%S'`"
        /opt/bitcoin/current/bin/bitcoin-cli --conf=/blockchain/bitcoin/data/bitcoin.conf generatetoaddress 1 $address
        sleep 180
done