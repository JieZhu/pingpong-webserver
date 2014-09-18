# Ping-Pong and World Wide Web

##Group Member
Wenbo Bu, Li Shen, Jie Zhu

##Introduction

In this project, a "ping-pong" client program and a server program are developed. The "ping-pong" client and the server together are used to measure the network's performance in transmitting messages of various sizes.

In addition to responding to "ping-pong" client's messages, the server has an alternate execution mode in which it acts as a simple web server. In this mode, the server can answer HTTP GET requests from web browsers.

## Quick Start

The "ping-pong" client program should take 4 command line parameters, in the following order:

- **hostname** The host where the server is running. It should be specified by domain name.

- **port** The port on which the server is running. (on CLEAR, the usable range is 18000 <= port <= 18200).

- **size** The size in bytes of each messsage to send (10 <= size <= 65535).

- **count** The number of message exchanges to perform (1 <= count <= 10000).

The server program should take 1 mandatory parameter and 2 optional parameters, in the following order:

- **port** The port on which the server should run (on CLEAR, the usable range is 18000 <= port <= 18200).

- **mode (optional)** The mode of the server. If this is "www", then the server should run in web server mode. If it is anything else, or left out, then the server should run in ping-pong mode.

- **root_directory (required for www mode)** This is the directory where the web server should look for documents. If the server is not in web server mode, ignore this parameter.
