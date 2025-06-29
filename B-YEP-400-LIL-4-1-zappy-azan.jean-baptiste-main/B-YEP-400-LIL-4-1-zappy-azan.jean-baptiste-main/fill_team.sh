#!/bin/bash

if [ "$#" -ne 3 ]; then
  echo "Usage: $0 <port> <team_name> <number_of_people>"
  exit 1
fi

PORT=$1
TEAM_NAME=$2
NUMBER_OF_PEOPLE=$3

for i in $(seq 1 $NUMBER_OF_PEOPLE); do
  echo "./zappy_ai -p \"$PORT\" -n \"$TEAM_NAME\" -h 127.0.0.1"
done | xargs -P 0 -I {} bash -c '{}'