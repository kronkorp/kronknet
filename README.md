<a id="readme-top"></a>

<!-- PROJECT SHIELDS -->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]



<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/kronkorp">
    <img src="images/kronkcorp-k.svg" alt="Kronk Corp logo" width="200" align="middle">
  </a>
  &emsp;
  <img src="images/separator.svg" alt="" width="1" height="170" align="middle">
  &emsp;
  <a href="https://github.com/kronkorp/kronknet">
    <img src="images/logo.png" alt="KronkNet logo" width="200" align="middle">
  </a>

<h3 align="center">KronkNet</h3>

  <p align="center">
    A simple and fast asynchrone network library
    <br />
    <a href="https://github.com/kronkorp/kronknet#usage"><strong>Explore the usage »</strong></a>
    <br />
    <br />
    <a href="https://github.com/kronkorp/kronknet/tree/main/examples">View Examples</a>
    &middot;
    <a href="https://github.com/GauthierMalfilatre/kronknet/issues/new?labels=bug&template=bug-report---.md">Report Bug</a>
    &middot;
    <a href="https://github.com/GauthierMalfilatre/kronknet/issues/new?labels=enhancement&template=feature-request---.md">Request Feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
        <li><a href="#windows">Windows</a></li>
      </ul>
    </li>
    <li>
      <a href="#usage">Usage</a>
      <ul>
        <li><a href="#creating-a-server">Creating a server</a></li>
      </ul>
    </li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

KronkNet is a high-performance asynchronous TCP network library.

Built around the `poll()` system call and an event-driven architecture, it allows for the management of numerous simultaneous connections on a single thread. KronkNet integrates an internal ring buffer system to prevent OS blockage during massive data transfers (Fast Path/Slow Path mechanism with POLLOUT).

Key Features:

- **100% Non-Blocking**: The server never freezes the main game loop.

- **O(1) Binding**: Dual-pointer (user_data) system for lookup-free interaction between the network engine and application entities.

- **Multi-protocol support**: Currently, kronknet supports both TCP and UDP. In TCP mode, connections are stored in a dynamic array, whereas in UDP mode, they are kept in a hash table, offering O(1) lookup complexity in most cases.

- **IPv4 and IPv6**: A server takes both on a single dual-stack socket (IPv4 only on a host without IPv6), and a client connects to either: `knClient_connect(client, "::1", 4242)`. IPv4 clients keep showing as IPv4 (`127.0.0.1`).

<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With

* [![C][C_]][C-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

To get a local copy up and running, follow these steps.

### Prerequisites

CMake and a C compiler:
```sh
sudo apt install cmake build-essential
```

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/GauthierMalfilatre/kronknet.git
   cd kronknet/
   ```
2. Build with the provided `tools.sh` script
   ```sh
   ./tools.sh -r
   ```
3. Install the library
   ```sh
   cd Release/
   sudo make install
   ```

### Windows

kronknet builds on Windows (Vista or later) with MSVC or MinGW:
```sh
cmake -S . -B build
cmake --build build --config Release
```
You get `kronknet.dll` (with its import library) and a static library
(`kronknet_static.lib` with MSVC, `libkronknet.a` with MinGW). The static library needs `ws2_32`:
with CMake, linking the `kronknet-static` or `kronknet-shared` target takes care of everything.

The unit tests need `fork()`, so Windows only runs the smoke test (`-DBUILD_TESTS=ON`, then `ctest -C Release`).

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

### Creating a server

A server is a port, a protocol, and up to four callbacks. KronkNet does the polling, buffering and accepting; you only react to events.

```c
#include <kronknet/server/server.h>
#include <kronknet/connection/connection.h>
#include <kronknet/macros/errdef.h>
#include <stdio.h>

static int on_connect(knServer *server, knConnection *conn)
{
    (void)server;
    printf("+ %s:%hu\n", knConnection_getIp(conn), knConnection_getPort(conn));
    return KNEVTOK;
}

static int on_read(knConnection *conn, const void *data, size_t size)
{
    return knConnection_send(conn, data, size); // echo it back
}

static int on_disconnect(knServer *server, knConnection *conn)
{
    (void)server;
    printf("- %s:%hu\n", knConnection_getIp(conn), knConnection_getPort(conn));
    return KNEVTOK;
}

int main(void)
{
    knServer *server = knServer_create(4242, knTCP);

    if (!server)
        return 1;
    knServer_setOnConnect(server, on_connect);
    knServer_setOnRead(server, on_read);
    knServer_setOnDisconnect(server, on_disconnect);
    knServer_run(server); // blocks until knServer_stop()
    knServer_destroy(server);
    return 0;
}
```

Build and try it:

```sh
gcc echo.c -o echo -lkronknet
./echo &
nc localhost 4242
```

- Pass `knUDP` instead of `knTCP` to `knServer_create` for a UDP server.
- Callbacks return `KNEVTOK` on success, or `KNEVTKICK` to drop the connection.
- Need to tick the server inside your own loop (a game loop, for instance)? Use `knServer_runOnce(server, timeoutMs)` instead of `knServer_run`.
- Attach your own data with `knServer_setUserPtr` and `knConnection_setUserPtr`, and get it back in any callback.

More complete examples: [chat](examples/chat), [hangman](examples/hangman) and the [test server](test/server/test.c).

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
The full guidelines (setup, tests, branches, commit style) are in [CONTRIBUTING.md](CONTRIBUTING.md).
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feat/amazing-feature dev`)
3. Commit your Changes (`git commit -m '[ADD] Add some amazing feature'`)
4. Push to the Branch (`git push origin feat/amazing-feature`)
5. Open a Pull Request against `dev`

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Top contributors:

<a href="https://github.com/GauthierMalfilatre/kronknet/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=GauthierMalfilatre/kronknet" alt="contrib.rocks image" />
</a>



<!-- LICENSE -->
## License

Distributed under the MIT license. See `LICENSE` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Gauthier Malfilatre - [@GauthierMalfilatre](https://www.linkedin.com/in/gauthier-malfilatre-648001339/) - gauthier.malfilatre@epitech.eu

Project Link: [https://github.com/GauthierMalfilatre/kronknet](https://github.com/GauthierMalfilatre/kronknet)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- MARKDOWN LINKS & IMAGES -->
[contributors-shield]: https://img.shields.io/github/contributors/GauthierMalfilatre/kronknet.svg?style=for-the-badge
[contributors-url]: https://github.com/GauthierMalfilatre/kronknet/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/GauthierMalfilatre/kronknet.svg?style=for-the-badge
[forks-url]: https://github.com/GauthierMalfilatre/kronknet/network/members
[stars-shield]: https://img.shields.io/github/stars/GauthierMalfilatre/kronknet.svg?style=for-the-badge
[stars-url]: https://github.com/GauthierMalfilatre/kronknet/stargazers
[issues-shield]: https://img.shields.io/github/issues/GauthierMalfilatre/kronknet.svg?style=for-the-badge
[issues-url]: https://github.com/GauthierMalfilatre/kronknet/issues
[license-shield]: https://img.shields.io/github/license/GauthierMalfilatre/kronknet.svg?style=for-the-badge
[license-url]: https://github.com/GauthierMalfilatre/kronknet/blob/main/LICENSE
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/gauthier-malfilatre-648001339/
[C_]: https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white
[C-url]: https://www.c-language.org
