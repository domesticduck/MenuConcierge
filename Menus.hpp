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
    public:
        Menus(string p_url){
            url = p_url;
        }
        long get(string& ans){
            try
            {
                // HTTPリクエスト情報設定
                URI uri(url);
                HTTPClientSession session(uri.getHost(), uri.getPort());

                // prepare path
                string path(uri.getPathAndQuery());
                if (path.empty()) path = "/";

                // send request
                HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
                session.sendRequest(req);

                // get response
                HTTPResponse res;
                cout << res.getStatus() << " " << res.getReason() << endl;

                // print response
                istream &is = session.receiveResponse(res);
                // StreamCopier::copyStream(is, cout);

                StreamCopier::copyToString(is, ans, 8192);
            }

            catch (Exception &ex)
            {
                cerr << ex.displayText() << endl;
                return -1;
            }
            return 0;
        }
        
    protected:
        string url;
    };
}