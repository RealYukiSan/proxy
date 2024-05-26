# Proxy
Proxy for modifed and monitoring game packet

> Note: This project build only target for Linux if you using windows please use WSL

## How This Actualy Work

Well this projecg just emulate how are game is work we just make 1 ENet server and 1 ENet client. For client it's used for relay packet from local server to main server and server is a local server that's used for connection to game.
In this project i used 2 ENet struct `ENetServer` and `ENetRelay`. `ENetRelay` is a ENet client for sending packet from local server into actual server and `ENetServer` actualy is a local server, So basicly game will connect into local server and server will be relay packet into `ENetRelay` and `ENetRelay` will be give you packet from actual server and give `ENetServer` the result

## Setup Your Development Enviroment

To works with this project you need cmake at least version 3.24 or newer. And dont forget to copy `config.conf`. 


## Project License
This project are open-source for everyone
