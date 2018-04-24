#!/bin/sh

cd ./src
npm i
cd ..

cd ./coturn
sh ./download-coturn.sh
cd ..

cd ./nodejs
sh ./download-nodejs.sh
cd ..

docker build -t global-network .