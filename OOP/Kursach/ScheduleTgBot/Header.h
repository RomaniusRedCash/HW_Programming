#pragma once

#ifdef _WIN32
#define _WIN32_WINNT 0x0601
#endif

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/json/parse.hpp>
#include <boost/json/value.hpp>
#include <boost/json/serialize.hpp>

#define at_as_str(A) at(A).as_string().c_str()
#define at_as_int(A) at(A).as_int64()

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <thread>
#include <cctype>
#include <charconv>
#include <ctime>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = net::ssl;
namespace json = boost::json;