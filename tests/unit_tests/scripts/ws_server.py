#!/usr/bin/python3
import asyncio
import websockets
import subprocess


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

start_server = websockets.serve(handle_websocket, 'localhost', 8888)

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()
