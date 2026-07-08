<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a id="readme-top"></a>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]



<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/GauthierMalfilatre/kronknet">
    <img src="images/logo.png" alt="Logo" width="200">
  </a>

<h3 align="center">KronkNet</h3>

  <p align="center">
    A simple and fast asynchrone network library
    <br />
    <a href="https://github.com/GauthierMalfilatre/kronknet"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/GauthierMalfilatre/kronknet">View Demo</a>
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
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

<!-- [![Product Name Screen Shot][product-screenshot]](https://example.com) -->

KronkNet is a high-performance asynchronous TCP network library.

Built around the `poll()` system call and an event-driven architecture, it allows for the management of numerous simultaneous connections on a single thread. KronkNet integrates an internal ring buffer system to prevent OS blockage during massive data transfers (Fast Path/Slow Path mechanism with POLLOUT).

Key Features:

- **100% Non-Blocking**: The server never freezes the main game loop.

- **O(1) Binding**: Dual-pointer (user_data) system for lookup-free interaction between the network engine and application entities.

- **Multi-protocol support**: Currently, kronknet supports both TCP and UDP. In TCP mode, connections are stored in a dynamic array, whereas in UDP mode, they are kept in a hash table, offering O(1) lookup complexity in most cases.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With

* [![C][C_]][C-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

To get a local copy up and running follow these simple example steps.

### Prerequisites

have CMake installed
```sh
apt install cmake
```

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/GauthierMalfilatre/kronknet.git
   cd kronknet/
   ```
2. Use provided `tools.sh` file
   ```sh
   ./tools.sh -r
   ```
3. Installing the library
    ```sh
    cd Release/
    sudo make install
    ```
<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

You can find an exemple of usage [here](https://github.com/GauthierMalfilatre/kronknet/blob/main/test/server/test.c)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

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
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
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
[linkedin-url]: https://linkedin.com/in/linkedin_username
[product-screenshot]: images/screenshot.png
<!-- Shields.io badges. You can a comprehensive list with many more badges at: https://github.com/inttter/md-badges -->
[C_]: https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white
[C-url]: https://www.c-language.org
[Next.js]: https://img.shields.io/badge/next.js-000000?style=for-the-badge&logo=nextdotjs&logoColor=white
[Next-url]: https://nextjs.org/
[React.js]: https://img.shields.io/badge/React-20232A?style=for-the-badge&logo=react&logoColor=61DAFB
[React-url]: https://reactjs.org/
[Vue.js]: https://img.shields.io/badge/Vue.js-35495E?style=for-the-badge&logo=vuedotjs&logoColor=4FC08D
[Vue-url]: https://vuejs.org/
[Angular.io]: https://img.shields.io/badge/Angular-DD0031?style=for-the-badge&logo=angular&logoColor=white
[Angular-url]: https://angular.io/
[Svelte.dev]: https://img.shields.io/badge/Svelte-4A4A55?style=for-the-badge&logo=svelte&logoColor=FF3E00
[Svelte-url]: https://svelte.dev/
[Laravel.com]: https://img.shields.io/badge/Laravel-FF2D20?style=for-the-badge&logo=laravel&logoColor=white
[Laravel-url]: https://laravel.com
[Bootstrap.com]: https://img.shields.io/badge/Bootstrap-563D7C?style=for-the-badge&logo=bootstrap&logoColor=white
[Bootstrap-url]: https://getbootstrap.com
[JQuery.com]: https://img.shields.io/badge/jQuery-0769AD?style=for-the-badge&logo=jquery&logoColor=white
[JQuery-url]: https://jquery.com 
