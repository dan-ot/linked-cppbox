#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>

#include <crow.h>
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
using bsoncxx::builder::basic::make_document;
using mongocxx::cursor;

using namespace std;
using namespace crow;
using namespace crow::mustache;

void get_view(response &res, const string &filename, context &x) {
    res.set_header("Content-Type", "text/html");
    auto text = load(filename + ".html").render_string(x);
    res.write(text);
    res.end();
}

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

void notFound(response &res, const string &message) {
    res.code = 404;
    res.write(message + ": Not found");
    res.end();
}

int main() {
    crow::SimpleApp app;

    mongocxx::instance inst {};
    string mongo_connect = string(getenv("AZURE_COSMOS_CONNECTIONSTRING"));
    mongocxx::client conn {mongocxx::uri {mongo_connect}};
    auto collection = conn["hello"]["contacts"];

    CROW_ROUTE(app, "/contact/<string>")
        ([&collection](const request &req, response &res, string email) {
            auto doc = collection.find_one(make_document(kvp("email", email)));
            crow::json::wvalue dto;

            dto = json::load(bsoncxx::to_json(doc.value().view()));
            return get_view(res, "contact", dto);
        });

    CROW_ROUTE(app, "/contact/<string>/<string>")
        ([&collection](const request &req, response &res, string firstname, string lastname) {
            auto doc = collection.find_one(make_document(kvp("firstName", firstname), kvp("lastName", lastname)));

            if (!doc) {
                return notFound(res, "Contact");
            }
            crow::json::wvalue dto;

            dto = json::load(bsoncxx::to_json(doc.value().view()));
            return get_view(res, "contact", dto);
        });

    CROW_ROUTE(app, "/contacts")
        ([&collection](const request &req, response &res) {
            mongocxx::options::find opts;
            opts.skip(9);
            opts.limit(10);
            auto docs = collection.find({}, opts);
            crow::json::wvalue dto;
            vector<crow::json::rvalue> contacts;

            contacts.reserve(10);
            for (auto doc : docs) {
                contacts.push_back(json::load(bsoncxx::to_json(doc)));
            }

            dto["contacts"] = contacts;

            return get_view(res, "contacts", dto);
        });

    CROW_ROUTE(app, "/rest_test").methods(HTTPMethod::Post, HTTPMethod::Get, HTTPMethod::Put)
        ([](const request &req, response &res){
            string method = method_name(req.method);
            res.set_header("Content-Type", "text/plain");
            res.write(method + " rest_test");
            res.end();
        });

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

    CROW_ROUTE(app, "/add/<int>/<int>")
        ([](const request &req, response &res, int a, int b) {
            res.set_header("Content-Type", "text/plain");
            ostringstream os;
            os << "Integer " << a << " + " << b << " = " << a + b << "\n";
            res.write(os.str());
            res.end();
        });
    CROW_ROUTE(app, "/add/<double>/<double>")
        ([](const request &req, response &res, double a, double b) {
            res.set_header("Content-Type", "text/plain");
            ostringstream os;
            os << "Double " << a << " + " << b << " = " << a + b << "\n";
            res.write(os.str());
            res.end();
        });
    CROW_ROUTE(app, "/add/<string>/<string>")
        ([](const request &req, response &res, string a, string b) {
            res.set_header("Content-Type", "text/plain");
            ostringstream os;
            os << "String " << a << " + " << b << " = " << a + b << "\n";
            res.write(os.str());
            res.end();
        });

    char *port = getenv("PORT");
    uint16_t iport = static_cast<uint16_t>(port != nullptr ? stoi(port) : 18080);
    std::cout << "PORT = " << iport << "\n";
    app.port(iport).multithreaded().run();

    return 0;
}