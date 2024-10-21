#include <crow.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>

#include <bsoncxx/v_noabi/bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/v_noabi/bsoncxx/json.hpp>
#include <bsoncxx/v_noabi/bsoncxx/oid.hpp>
#include <mongocxx/v_noabi/mongocxx/client.hpp>
#include <mongocxx/v_noabi/mongocxx/stdx.hpp>
#include <mongocxx/v_noabi/mongocxx/uri.hpp>
#include <mongocxx/v_noabi/mongocxx/instance.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::basic::kvp;
using mongocxx::cursor;

using namespace std;
using namespace crow;

void send_file(response &res, string filename, string content_type) {
    ifstream in("public/" + filename, ifstream::in);

    if (in) {
        ostringstream contents;
        contents << in.rdbuf();
        in.close();
        res.set_header("Content-Type", content_type);
        res.write(contents.str());
    } else {
        res.write("Not found");
        res.code = 404;
    }

    res.end();
}

void send_html(response &res, string document) {
    send_file(res, document + ".html", "text/html");
}

void send_image(response &res, string filename) {
    send_file(res, "media/" + filename, "image/jpeg");
}

void send_script(response &res, string script) {
    send_file(res, "scripts/" + script + ".js", "text/javascript");
}

void send_style(response &res, string style) {
    send_file(res, "styles/" + style + ".css", "text/css");
}

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
        ([](const request &req, response &res){
            send_html(res, "index");
        });

    CROW_ROUTE(app, "/styles/<string>")
        ([](const request &req, response &res, string filename){
            send_style(res, filename);
        });

    CROW_ROUTE(app, "/scripts/<string>")
        ([](const request &req, response &res, string filename){
            send_script(res, filename);
        });

    CROW_ROUTE(app, "/media/<string>")
        ([](const request &req, response &res, string filename){
            send_image(res, filename);
        });

    CROW_ROUTE(app, "/<string>")
        ([](const request &req, response &res, string page){
            send_html(res, page);
        });

    char *port = getenv("PORT");
    uint16_t iport = static_cast<uint16_t>(port != nullptr ? stoi(port) : 18080);
    cout << "PORT = " << iport << "\n";
    app.port(iport).multithreaded().run();

    return 0;
}