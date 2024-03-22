<a name="readme-top"></a>

<h1 align="center">Webserv</h1>
<p align="center">
	<img alt="GitHub code size in bytes" src="https://img.shields.io/github/languages/code-size/oph-design/webserv?color=lightblue" />
	<img alt="Code language count" src="https://img.shields.io/github/languages/count/oph-design/webserv?color=yellow" />
	<img alt="GitHub top language" src="https://img.shields.io/github/languages/top/oph-design/webserv?color=blue" />
	<img alt="GitHub last commit" src="https://img.shields.io/github/created-at/oph-design/webserv?color=green" />
</p>


<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#usage">Usage</a></li>
      </ul>
    </li>
     <li>
      <a href="#web-server-configuration">Configuration</a>
      <ul>
        <li><a href="#server-block">Server Block</a></li>
        <li><a href="#location-block">Location Block</a></li>
        <li><a href="#limit_except-block">Limit Except Block</a></li>
      </ul>
    </li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>


<!-- ABOUT THE PROJECT -->
## About The Project

<p align="center">
<img width="200" src="https://github.com/oph-design/webserv/assets/115570424/302d6a9b-c5ce-48e9-9a11-c6fa005613bd"/>
</p>



Webserv is a project from the 42 curriculum, aiming to build a webserver based on NGINX. The implementation follows the latest guidelines from the RFCs for the HTML protocol and CGI protocol. The server is completely build in C++ and uses poll as the multiplexer to manage the socket connections. The implementation covers the following features:
* Handling of `GET`, `POST` and `DELETE` requests
* Serving of every common Filetype
* Nginx-like configuration with config files
* Common Gateway Interface for every requests type
* Autoindexing


## Getting Started

The following contains a description of how to use the program. It is recommend to run in an unix (linux or mac) environment.
As installing make and compilers in windows can be quite tedious. You can find a tutorial and setting up an ubuntu virtual machine <a href="https://www.linkedin.com/pulse/how-install-ubuntu-virtualbox-md-emamul-mursalin/">here</a>

### Prerequisites

You need to have make and gcc installed. For mac you need also xcode to perform the installation.
* linux (Debian based)
  ```sh
  sudo apt install build-essential
  ```
* MAC-OS
  ```sh
  xcode-select --install
  ```

### Usage

1. Clone the repo
   ```sh
   git clone https://github.com/oph-design/webserv.git
   ```
2. Build the executabel at the root of the repository
   ```sh
   make webserv
   ```
3. Run the executable with your desired config file
   ```sh
   ./webserv conf/webserv.conf
   ```
4. Open localhost on your browser with the specified port number
   ```sh
   curl localhost:1234
   ```

## Web Server Configuration

This section provides a guide for configuring your web server using a syntax similar to Nginx. The configuration file allows you to define server blocks, location blocks, and limit_except blocks to tailor your web server's behavior.


### Server Block

A server block defines the settings for a specific virtual server. Below is the syntax for a server block:

```nginx
server {
    listen PORT;
    client_max_body_size SIZE;
    server_name DOMAIN;
    index INDEX_FILE;
    root ROOT_DIRECTORY;
    error_page CODE PATH;
    timeout TIMEOUT

    # Add location blocks and limit_except blocks here
}
```

- `PORT`: The port number the server will listen on.
- `SIZE`: Maximum body size for client requests.
- `DOMAIN`: The domain name associated with the server.
- `INDEX_FILE`: Default file to serve if no specific file is requested.
- `ROOT_DIRECTORY`: The root directory for serving files.
- `CODE`: HTTP status code for which to display the custom error page.
- `PATH`: Path to the custom error page.
- `TIMEOUT`: The time until a client should time out in seconds.

---

### Location Block

A location block defines settings for specific URL paths within a server block. Below is the syntax for a location block:

```nginx
location PATH {
    autoindex on|off;
    client_max_body_size SIZE;
    index INDEX_FILE;
    root ROOT_DIRECTORY;
    cgi_pass CGI_DIRECTORY;
    error_page CODE PATH;
    cgi_processing CGI;

    # Add limit_except block here if wanted
}
```

- `PATH`: URL path to apply the location settings to.
- `autoindex`: Enable or disable directory listing.
- `SIZE`: Maximum body size for client requests.
- `INDEX_FILE`: Default file to serve for this location.
- `ROOT_DIRECTORY`: The directory to serve files from.
- `CGI_DIRECTORY`: Path where the CGI-Scripts are located.
- `CODE`: HTTP status code for which to display the custom error page.
- `PATH`: Path to the custom error page.
- `CGI`: Allowed CGIs.

---

### Limit_except Block

A limit_except block restricts the HTTP methods allowed on a specific URL path. Below is the syntax for a limit_except block:

```nginx
limit_except METHODS {
    deny all;
}
```

- `METHODS`: List of HTTP methods to restrict. (e.g., GET, POST, PUT, DELETE, etc.)

---

Remember to replace placeholders (PORT, SIZE, DOMAIN, etc.) with actual values relevant to your web server setup.

**Note:** This configuration is an abstraction and not an actual web server configuration. Refer to your web server's documentation for implementation details and further customization.

## Contact
<p>Michael Graefen </p>
<p><a href="https://github.com/migrae"><img src="https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white"/></a>
<a href="https://www.linkedin.com/in/michael-graefen-a88180124/"><img src="https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white"/></a></p>
<p>Florian Sandel </p>
<p></p><a href="https://github.com/fsandel"><img src="https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white"/></a>
<a href="https://www.linkedin.com/in/florian-sandel-855369263/"><img src="https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white"/></a></p>
<p>Ole-Paul Heinzelmann </p>
<p></p><a href="https://github.com/oph-design"><img src="https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white"/></a>
<a href="https://www.linkedin.com/in/ole-paul-heinzelmann-a08304258/"><img src="https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white"/></a></p>

<p align="right">(<a href="#readme-top">back to top</a>)</p>


