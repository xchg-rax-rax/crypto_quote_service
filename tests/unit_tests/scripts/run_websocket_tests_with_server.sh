#!/bin/bash
$1/tests/unit_tests/scripts/wss_server.py &
$1/cmake/tests/unit_tests/Websocket_tests
