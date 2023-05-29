#!/usr/bin/python3
import ssl
import asyncio
import websockets

async def handle_websocket(websocket, path):
    print("[+] Started Websocket Server")
    while True:
        try:
            message = await websocket.recv()
            print(f"[+] Message received: {message}")
            response = f"You sent: {message}"
            await websocket.send(response)
        except websockets.exceptions.ConnectionClosedOK:
            print("[-] Websocket connection closed")
            break

ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
ssl_context.load_cert_chain('certificate.crt', 'private.key')

start_server = websockets.serve(
    handle_websocket, 'localhost', 8443, ssl=ssl_context
)

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()

