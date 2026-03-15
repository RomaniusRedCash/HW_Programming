#include "ClientBase.h"

bool ClientBase::connectServer() {
    std::chrono::milliseconds secCon(500);
    try {
        do {
            disconnectServer();
            ssl::context ctx(ssl::context_base::tlsv12_client);
            sslStream = new beast::ssl_stream<beast::tcp_stream>(ioc, ctx);
            ctx.set_default_verify_paths();
            ctx.set_verify_mode(ssl::verify_peer);
            beast::get_lowest_layer(*sslStream).connect(resolver.resolve(url, "443"), ec);
            SSL_set_tlsext_host_name(sslStream->native_handle(), url.c_str());
            sslStream->handshake(ssl::stream_base::client, ec);
            std::cout << "Connect with " << url << ": " << ec << std::endl;
            if (ec) {
                if (secCon > maxCooldown) {
                    std::cout << "Connect is fail" << std::endl;
                    return false;
                }
                std::cout << "reconnect server after " << secCon.count() << " ms." << std::endl;
                std::this_thread::sleep_for(secCon *= 1.5);
            }
        } while (ec);
    }
    catch (const boost::system::system_error& e) {
        if (secCon > maxCooldown) {
            std::cout << "Connect is fail" << std::endl;
            return false;
        }
        std::cout << "reconnect server after " << secCon.count() << " ms." << std::endl;
        std::this_thread::sleep_for(secCon *= 1.5);
    }
    return true;
}

void ClientBase::disconnectServer() {
    if (sslStream) {
        beast::get_lowest_layer(*sslStream).cancel();
        sslStream->shutdown(ec);
        beast::get_lowest_layer(*sslStream).close();
        delete sslStream;
        sslStream = nullptr;
    }
}

json::value ClientBase::someRequest(const http::verb& method, const json::value& jsonRequest) {
    for (int8_t i = 0; i < 2; i++) {
        try {
            http::request<http::string_body> req(method, jsonRequest.at_as_str("method"), 11);
            req.body() = json::serialize(jsonRequest);
            req.set(http::field::host, url);
            req.set(http::field::user_agent, clientName);
            req.set(http::field::content_type, "application/json");
            req.set(http::field::accept, "application/json");
            req.prepare_payload();

            http::write(*sslStream, req);

            beast::flat_buffer buffer;
            http::response<http::dynamic_body> res;
            http::read(*sslStream, buffer, res);

            json::value JvOut(json::parse(beast::buffers_to_string(res.body().data())));
            return JvOut;
        }
        catch (const boost::system::system_error& e) {
            std::cout << e.what() << std::endl;
        }
        connectServer();
    }
}

ClientBase::ClientBase(const char* url, const json::value& prop) : url(url), resolver(ioc), prop(prop) {
    connectServer();
}

ClientBase::~ClientBase() {
	disconnectServer();
    std::cout << "Connect with " << url << " off" << std::endl;
}