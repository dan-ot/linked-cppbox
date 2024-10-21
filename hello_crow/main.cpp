#include <crow.h>
using namespace std;
using namespace crow;

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
        ([](const request &req, response &res){
            ifstream in("public/index.html", ifstream::in);

            if (in) {
                ostringstream contents;
                contents << in.rdbuf();
                in.close();
                res.write(contents.str());
            } else {
                res.write("Not found");
                res.code = 404;
            }

            res.end();
        });

    char *port = getenv("PORT");
    uint16_t iport = static_cast<uint16_t>(port != nullptr ? stoi(port) : 18080);
    cout << "PORT = " << iport << "\n";
    app.port(iport).multithreaded().run();

    return 0;
}