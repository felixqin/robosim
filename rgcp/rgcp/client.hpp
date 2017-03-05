#pragma once

#include <boost/asio.hpp>

namespace rgcp
{
    class client
    {
    public: // ctor & dtor
        client() : socket_(io_service_) {}

    public: // method
        template<typename StringT>
        void open(StringT addr, unsigned short port=8101)
        {
            using namespace boost::asio;
            ip::tcp::endpoint ep(ip::address::from_string(addr),port);
            boost::system::error_code ec;
            socket_.close();
            socket_.connect(ep,ec);
            if (ec)
            {
                throw boost::system::system_error(ec);
            }
        }
        void close()
        {
            socket_.close();
        }
        std::string invoke(std::string const& r)
        {
            if (r.empty()) return std::string();

            socket_.write_some(boost::asio::buffer(r));
            char buf[256];
            boost::system::error_code ec;
            size_t bytes = socket_.read_some(boost::asio::buffer(buf),ec);
            if (ec)
            {
                throw boost::system::system_error(ec);
            }
            return std::string(&buf[0],&buf[0]+bytes);
        }

    public: // status
        bool is_open() const { return socket_.is_open(); }

    private:
        boost::asio::io_service io_service_;
        boost::asio::ip::tcp::socket socket_;
    };
}
