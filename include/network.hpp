#pragma once

#include <boost/asio.hpp>
#include <iostream>

class Network {
public:
    Network();
    void startServer();
    void startClient();
private:
    void handleClient(boost::asio::ip::tcp::socket socket);
};
