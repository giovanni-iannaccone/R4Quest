#include "R4Quest.h"

#include <algorithm>

HTTPClient::HTTPClient() {
    set_header("User-Agent", HTTP_ARDUINO_USER_AGENT);
    set_header("Connection", "close");
}

HTTPClient::~HTTPClient(){
    clear_headers();
}

void HTTPClient::clear_headers() {
    headers.clear();
}

String HTTPClient::receive_response() {

    String response {};

    char buffer[R4QUEST_BUFSIZ + 1];
    const char* const end = std::end(buffer) - 1;
    char *bptr = buffer;

    // Buffering is used because concatenating a character each
    // iteration it's slow, because triggers a reallocation of the
    // string each time, with the buffering we reduce significantly
    // the numbers of reallocations.
 
    while (client.available()) {
        if(bptr == end) {
            *bptr = '\0';
            response.concat(buffer);
            bptr = buffer;
        }

        *bptr = client.read();

        if(*bptr == -1) {
            break;
        }

        bptr++;
    }

    *bptr = '\0';
    response.concat(buffer);

    return response;
}

void HTTPClient::remove_header(const String& name) {

    // @see Erase-Remove C++ Idiom: https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom

    const auto end = headers.end();
    const auto begin = std::remove_if(headers.begin(), end,
                                      [&name](const HTTPHeader& header) -> bool {
                                          return header.name == name; 
                                      });

    headers.eraese(begin, end);
}

String HTTPClient::request_delete(const String& url, int port, const String& body) {
    return send_request(url, port, HTTPMethod::Delete, body);
}

String HTTPClient::request_get(const String& url, int port, const String& body) {
    return send_request(url, port, HTTPMethod::Get, body);
}

String HTTPClient::request_post(const String& url, int port, const String& body) {
    return send_request(url, port, HTTPMethod::Post, body);
}

String HTTPClient::request_put(const String& url, int port, const String& body) {
    return send_request(url, port, HTTPMethod::Put, body);
}

void HTTPClient::send_headers() {
    if (!client.available()) {
        return;
    }

    for (const auto& header : headers) {
        client.print(header.name);
        client.print(": ");
        client.println(header.value);
    }
}

String HTTPClient::send_request(const String& url, int port, HTTPMethod method, const String& body) {
    String response {};

    if (!client.connect(url, port)) {
        return reponse;
    }
        
    switch(method) {
        case HTTPMethod::Get:
            client.print("GET");
            break;
        case HTTPMethod::Post:
            client.print("POST");
            break;
        case HTTPMethod::Delete:
            client.print("DELETE");
            break;
        case HTTPMethod::Put:
            client.print("PUT");
            break;
    }
        
    client.print(method);
    client.println(" / HTTP/1.1");
    client.print("Host: ");
    client.println(url);
        
    send_headers();

    client.println(body);
    client.println();

    response = receive_response();
    client.stop();

    return response;
}

void HTTPClient::set_authorization(String auth) {
    set_header("Authorization", std::move(auth));
}

HTTPClient& HTTPClient::set_header(String key, String value) {
    
    const auto end = headers.end();
    auto headerPos = std::find_if(headers.begin(), end, 
                        [&name](const HTTPHeader& header) -> bool {
                            return header.name == name;
                        });

    if(headerPos != end) {
        headerPos->value = value;
    } else {
        headers.emplace_back(std::move(key), std::move(value));
    }

    return *this;
}
