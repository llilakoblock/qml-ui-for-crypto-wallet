# нужен http и gear-js
# pip install requests gear-js

import requests
from gear import GearApi, Keyring, Message

def get_crypto_prices(crypto_ids):
    url = "https://api.coingecko.com/api/v3/simple/price"
    params = {
        'ids': ','.join(crypto_ids),
        'vs_currencies': 'usd',
    }
    response = requests.get(url, params=params)
    if response.status_code == 200:
        return response.json()
    else:
        raise Exception("Error CoinGecko API")

def push_prices_to_oracle(contract_address, crypto_prices, interval):
    api = GearApi.create("ws://46.101.149.175:9944")
    keyring = Keyring.from_json("keyring.json", "12345678")
    
    for crypto, prices in crypto_prices.items():
        price = prices['usd']
        message = Message(
            destination=contract_address,
            payload={
                "SetPrice": {
                    "name": crypto,
                    "price": price,
                    "interval": interval
                }
            }
        )
        extrinsic = api.message_submit(message, keyring)
        api.send_and_sign(extrinsic)
        print(f"Price for {crypto} set to {price} USD for {interval} interval.")

def main():
    crypto_ids = ['bitcoin', 'ethereum', 'vara', 'usdt']
    contract_address = "0x"
    intervals = ['minutely', 'hourly', 'weekly']

    for interval in intervals:
        try:
            crypto_prices = get_crypto_prices(crypto_ids)
            push_prices_to_oracle(contract_address, crypto_prices, interval)
        except Exception as e:
            print(f"Error: {e}")

if __name__ == "__main__":
    main()