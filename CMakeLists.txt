cmake_minimum_required(VERSION 3.25)
project(webserv)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(src/config)
include_directories(src/core)
include_directories(src/http)
include_directories(src/utils)

add_executable(webserv
        include/colors.hpp
        src/config/Config.cpp
        src/config/Config.hpp
        src/config/ConfigFile.cpp
        src/config/ConfigFile.hpp
        src/config/ConfigGet.cpp
        src/config/ConfigParsing.cpp
        src/config/ConfigParsing.hpp
        src/config/Line.cpp
        src/config/Line.hpp
        src/config/Location.cpp
        src/config/Location.hpp
        src/config/LocationGet.cpp
        src/core/CgiConnector.cpp
        src/core/CgiConnector.hpp
        src/core/Receive.cpp
        src/core/ServerCluster.cpp
        src/core/ServerCluster.hpp
        src/core/Socket.cpp
        src/core/Socket.hpp
        src/core/Webserver.cpp
        src/core/Webserver.hpp
        src/http/Request.cpp
        src/http/Request.hpp
        src/http/Response.cpp
        src/http/Response.hpp
        src/http/Status.cpp
        src/http/Status.hpp
        src/utils/FindFile.cpp
        src/utils/FindFile.hpp
        src/utils/GetContentDisposition.cpp
        src/utils/GetContentDisposition.hpp
        src/utils/IsNumber.cpp
        src/utils/IsNumber.hpp
        src/utils/StreamOperators.hpp
        src/utils/SwapColumns.hpp
        src/utils/ToString.hpp
        src/utils/Trim.cpp
        src/utils/Trim.hpp
        src/utils/Types.hpp
        src/utils/Utils.hpp
        src/main.cpp
        Makefile)
