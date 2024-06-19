#include "network.hpp"
#include <iostream>
#include <boost/asio.hpp>

void handleCommand(boost::asio::ip::tcp::socket& socket, const std::string& command) {
    boost::asio::write(socket, boost::asio::buffer(command));

    char reply[1024];
    boost::system::error_code error;
    size_t reply_length = socket.read_some(boost::asio::buffer(reply), error);

    if (error == boost::asio::error::eof)
        return; // Connection closed cleanly by peer.
    else if (error)
        throw boost::system::system_error(error); // Some other error.

    std::cout << "Reply: ";
    std::cout.write(reply, reply_length);
    std::cout << "\n";
}

int main() {
    try {
        boost::asio::io_context io_context;
        boost::asio::ip::tcp::resolver resolver(io_context);
        boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "12345");

        boost::asio::ip::tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        for (;;) {
            std::cout << "Enter command (CREATE/DELETE/WRITE/READ/LIST): ";
            std::string command;
            std::getline(std::cin, command);

            std::istringstream iss(command);
            std::string operation;
            iss >> operation;

            if (operation == "CREATE" || operation == "DELETE" || operation == "READ") {
                std::cout << "Enter path: ";
                std::string path;
                std::getline(std::cin, path);
                handleCommand(socket, command + " " + path);
            } else if (operation == "WRITE") {
                std::cout << "Enter path: ";
                std::string path;
                std::getline(std::cin, path);
                std::cout << "Enter content: ";
                std::string content;
                std::getline(std::cin, content);
                handleCommand(socket, command + " " + path + " " + content);
            } else if (operation == "LIST") {
                std::cout << "Enter directory: ";
                std::string directory;
                std::getline(std::cin, directory);
                handleCommand(socket, command + " " + directory);
            } else {
                std::cout << "Invalid command. Try again." << std::endl;
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
