#include "network.hpp"
#include "file_system.hpp"

Network::Network() {}

void Network::startServer() {
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 12345));
    std::cout << "Server started, waiting for connections..." << std::endl;

    for (;;) {
        boost::asio::ip::tcp::socket socket(io_context);
        acceptor.accept(socket);
        std::thread(&Network::handleClient, this, std::move(socket)).detach();
    }
}

void Network::handleClient(boost::asio::ip::tcp::socket socket) {
    try {
        FileSystem fs;
        for (;;) {
            char data[512];
            boost::system::error_code error;
            size_t length = socket.read_some(boost::asio::buffer(data), error);

            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            std::string command(data, length);
            std::istringstream iss(command);
            std::string operation;
            iss >> operation;

            if (operation == "CREATE") {
                std::string path;
                iss >> path;
                bool result = fs.createFile(path);
                boost::asio::write(socket, boost::asio::buffer(result ? "SUCCESS" : "FAILURE"));
            } else if (operation == "DELETE") {
                std::string path;
                iss >> path;
                bool result = fs.deleteFile(path);
                boost::asio::write(socket, boost::asio::buffer(result ? "SUCCESS" : "FAILURE"));
            } else if (operation == "WRITE") {
                std::string path, content;
                iss >> path;
                std::getline(iss, content);
                bool result = fs.writeFile(path, content);
                boost::asio::write(socket, boost::asio::buffer(result ? "SUCCESS" : "FAILURE"));
            } else if (operation == "READ") {
                std::string path;
                iss >> path;
                std::string content = fs.readFile(path);
                boost::asio::write(socket, boost::asio::buffer(content));
            } else if (operation == "LIST") {
                std::string directory;
                iss >> directory;
                std::vector<std::string> files = fs.listFiles(directory);
                std::ostringstream oss;
                for (const auto& file : files) {
                    oss << file << "\n";
                }
                std::string fileList = oss.str();
                boost::asio::write(socket, boost::asio::buffer(fileList));
            } else {
                boost::asio::write(socket, boost::asio::buffer("INVALID COMMAND"));
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

void Network::startClient() {
    try {
        boost::asio::io_context io_context;
        boost::asio::ip::tcp::resolver resolver(io_context);
        boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "12345");

        boost::asio::ip::tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        for (;;) {
            std::cout << "Enter message: ";
            char request[512];
            std::cin.getline(request, 512);
            size_t request_length = std::strlen(request);
            boost::asio::write(socket, boost::asio::buffer(request, request_length));

            char reply[512];
            size_t reply_length = boost::asio::read(socket, boost::asio::buffer(reply, request_length));
            std::cout << "Reply is: ";
            std::cout.write(reply, reply_length);
            std::cout << "\n";
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}