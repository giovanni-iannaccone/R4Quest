#ifndef __R4QUEST_H__
#define __R4QUEST_H__

#include <vector>
#include <WiFi.h>

#define R4QUEST_BUFSIZ 256
#define HTTP_ARDUINO_USER_AGENT "ArduinoWiFi/1.1"
#define HTTP_PORT 80

enum class HTTPMethod {
    Get,
    Post,
    Delete,
    Put
};

struct HTTPHeader {

    HTTPHeader(String name, String value)
        : name{std::move(name)},
          value{std::move(value)} {}

    String name;
    String value;
};

class HTTPClient {
public:

    HTTPClient();
    ~HTTPClient();

    void clear_headers();
    void remove_header(const String& name);

    String request_delete(const String& url, int port = HTTP_PORT, const String& body = "");
    String request_get(const String& url, int port = HTTP_PORT, const String& body = "");
    String request_post(const String& url, int port = HTTP_PORT, const String& body = "");
    String request_put(const String& url, int port = HTTP_PORT, const String& body = "");

    void set_authorization(String auth);
    HTTPClient& set_header(String name, String value);

private:

    String receive_response();
    void send_headers();
    String send_request(const String& url, int port, HTTPMethod method, const String& body);

    std::vector<HTTPHeader> headers;
    WiFiClient client;

};

#endif
