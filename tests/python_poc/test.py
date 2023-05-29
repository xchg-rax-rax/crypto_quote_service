import websocket
import json

# Define websocket url
ws_url = "wss://stream.binance.com:9443/ws/btcusdt@trade"

# Define what to do when the connection is opened
def on_open(ws):
    print('Connection established')

# Define what to do when a new message arrives
def on_message(ws, message):
    message_json = json.loads(message)
    print(f"New trade executed: {message_json}")

# Define what to do when the connection is closed
def on_close(ws, close_status_code, close_msg):
    print('Connection closed')

# Define what to do in case of an error
def on_error(ws, error):
    print(f'Error occurred: {error}')

# Establish a websocket connection
ws = websocket.WebSocketApp(
    ws_url,
    on_open=on_open,
    on_message=on_message,
    on_close=on_close,
    on_error=on_error)

ws.run_forever()

