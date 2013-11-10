#include <stdio.h>
#include <string>

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <iostream>

using namespace Poco::Net;
using namespace Poco;
using namespace std;

namespace Model
{
    class Menus {

    HTTPClientSession session;
    string path;

    public:
        string id;
        string name;
        string main_id;

    public:
        Menus(){
            URI uri("http://192.168.3.7:3000");
            session.setHost(uri.getHost());
            session.setPort(uri.getPort());

            // prepare path
            path = uri.getPathAndQuery();
            if (path.empty()) path = "/";
        }

        long get(string& ans){
            try
            {
                // send request
                HTTPRequest req(HTTPRequest::HTTP_GET, path + "menus.text", HTTPMessage::HTTP_1_1);
                session.sendRequest(req);

                // get response
                HTTPResponse res;

                // print response
                istream &is = session.receiveResponse(res);
                StreamCopier::copyToString(is, ans, 8192);
            }

            catch (Exception &ex)
            {
                cerr << ex.displayText() << endl;
                return -1;
            }
            return 0;
        }

        long create(){
            try
            {
                // send request
                HTTPRequest req(HTTPRequest::HTTP_POST, path + "menus.text", HTTPMessage::HTTP_1_1);
                session.sendRequest(req);
        //        req.setContentType("application/x-www-form-urlencoded\r\n");
                req.setKeepAlive(true);

                string key_name("menu[name]=");
                string delimiter("&");
                string key_main_id("menu[main_id]=");

                string reqBody(key_name + name + delimiter + key_main_id + main_id);

                req.setContentLength( reqBody.length() );

                session.sendRequest(req) << reqBody;

                // get response
                HTTPResponse res;
                cout << res.getStatus() << " " << res.getReason() << endl;

                // print response
                istream &is = session.receiveResponse(res);
                StreamCopier::copyToString(is, id, 8192);
            }

            catch (Exception &ex)
            {
                cerr << ex.displayText() << endl;
                return -1;
            }
            return 0;
        }
    };
}