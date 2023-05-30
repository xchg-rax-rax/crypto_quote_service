#!/bin/bash
cd $1/tests/unit_tests/scripts
./wss_server.py &
sleep 1
$1/cmake/tests/unit_tests/Websocket_tests
