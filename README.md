# webserv
Implementation of a minimalistic webserver for ecole 42


## Table of Contents

- [webserv](#webserv)
  - [Table of Contents](#table-of-contents)
- [Web Server Configuration](#web-server-configuration)
  - [Server Block](#server-block)
  - [Location Block](#location-block)
  - [Limit\_except Block](#limit_except-block)


# Web Server Configuration

This document provides a guide for configuring your web server using a syntax similar to Nginx. The configuration file allows you to define server blocks, location blocks, and limit_except blocks to tailor your web server's behavior.


## Server Block

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

## Location Block

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

## Limit_except Block

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


