#/bin/bash
cd ./cmake
cmake --build . 
mkdir -p ../bin
cp crypto_query_service ../bin/crypto_query_service

