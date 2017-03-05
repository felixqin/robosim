#pragma once

#include <array>
#include <memory>
#include <boost/asio.hpp>
//#include "dispatcher.h"


namespace rgcp {


class server : public boost::noncopyable
{
public:
    typedef std::string buffer_type;
    typedef std::tr1::function<buffer_type (buffer_type const&)> reply_function_type;

private:
    typedef boost::system::error_code error_t;
    typedef boost::asio::ip::tcp tcp_t;
    typedef boost::asio::ip::tcp::socket socket_t;

    //socket 和 buffer 都以引用计数方式使用, 自动管理它们的生存期
    typedef std::tr1::shared_ptr<socket_t>      socket_sp;
    typedef std::tr1::shared_ptr<buffer_type>   buffer_sp;

    enum { max_buffer_size = 510 };

public: //ctor & dtor
    server(reply_function_type reply,short port=8101)
        :io_service_()
        ,acceptor_(io_service_, tcp_t::endpoint(tcp_t::v4(), port))
        ,reply_(reply)
    {
        new_session();
    }
public: //method
    void run() { io_service_.run(); }
    void stop() { io_service_.post(std::tr1::bind(&server::handle_stop,this)); }

private:
    void new_session();
    void handle_accept(socket_sp, const error_t& error);
    void handle_stop();

    void read_start  (socket_sp, buffer_sp);
    void handle_read (socket_sp, buffer_sp, error_t const&, size_t);
    void handle_write(socket_sp, buffer_sp, error_t const&);
private:
    boost::asio::io_service         io_service_;
    boost::asio::ip::tcp::acceptor  acceptor_;
    reply_function_type             reply_;
};


///////////////////////////////////////////////////////

inline
void server::new_session()
{
    socket_sp ps(new socket_t(io_service_));
    acceptor_.async_accept(*ps,
        std::tr1::bind(&server::handle_accept, this, ps, std::tr1::placeholders::_1));
}

inline
void server::handle_accept(socket_sp ps, error_t const& error)
{
    if (!error)
    {
        buffer_sp pb(new buffer_type);
        read_start(ps,pb);
        new_session();
    }
}

inline
void server::handle_stop()
{
    acceptor_.close();
}

///////////////////////////////////////////////////////////////

inline
void server::read_start(socket_sp ps, buffer_sp pb)
{
    using boost::asio::buffer;

    pb->resize(max_buffer_size);    //分配足够内存供缓存使用
    ps->async_read_some(buffer(&(*pb)[0],pb->size()),
        std::tr1::bind(&server::handle_read,this,ps,pb,
            std::tr1::placeholders::_1,
            std::tr1::placeholders::_2));
}

inline
void server::handle_read(socket_sp ps, buffer_sp pb, error_t const& error, size_t bytes)
{
    using boost::asio::buffer;

    if (!error)
    {
        pb->resize(bytes);  //缩减大小以调用reply_function
        *pb = reply_(*pb);
        boost::asio::async_write(*ps, buffer(&(*pb)[0],pb->size()),
            std::tr1::bind(&server::handle_write, this, ps,pb,
                std::tr1::placeholders::_1));
    }
}

inline
void server::handle_write(socket_sp ps, buffer_sp pb, error_t const& error)
{
    if (!error)
    {
        read_start(ps,pb);
    }
}


}
