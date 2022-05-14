#!/bin/bash
set -eux

echo "Downloading YC certificate..."
mkdir -p ~/.postgresql && \
wget "https://storage.yandexcloud.net/cloud-certs/CA.pem" -O ~/.postgresql/root.crt && \
chmod 0600 ~/.postgresql/root.crt

echo "Starting runner with arguments $*"
mkdir /Thesis/cmaes-optimizer/build || true
cd /Thesis/cmaes-optimizer/build
#cmake -DCMAKE_BUILD_TYPE=Release ..
#make glucose runner

for experiment in /Thesis/experiments/8/8_mcper_{1,10,100,1000}.json
do
  ./runner $experiment &
done

for experiment in /Thesis/experiments/8/8_mcper_p_{1,10,100,1000}.json
do
  ./runner $experiment &
done

wait
