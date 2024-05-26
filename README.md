# Proxy
Proxy for modifed and monitoring game packet

> Note: This project build only target for Linux if you using windows please use WSL

## How This Actualy Work

Well this project just emulate how are game is acutauly work. We just make 1 ENet server and 1 ENet client, client used for relay packet from local server to main server and server is a local server to be used for connection to game.

In this project we only used 2 ENet struct `ENetServer` and `ENetRelay`. The `ENetRelay` is a ENet client for sending packet from local server into actual server and the `ENetServer` actualy is a local server, So basicly game will connect into local server and server will be relay packet into `ENetRelay` and `ENetRelay` will be give you packet from actual server and give `ENetServer` the result

## Limitation

Well this proxy only can accept for one client runtime. So if you want to make like can handle multiple connection on same time you can start contibute.

## Setup Your Development Enviroment

To works with this project you need cmake at least version 3.24 or newer. And dont forget to copy `config.conf`. 

```bash
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
$ make
```
> Optional for clangd LSP

```bash
$ sh gen_clangd.sh
```
## Contribute

Everyone can share your ideas in here just simple fork this project and make a features. Don't forget to pull request so we can add your ideas.

## Project License
This project are open-source for everyone and under [MIT](LICENSE) license.
