//
// Amity - cross-platform game engine
// https://github.com/aduros/amity/blob/master/LICENSE.txt

#pragma once

#include <vector>
#include <curl/curl.h>

class Http
{
public:
    Http ();
    ~Http ();

    void setUrl (const char* url);
    void setPost (bool post);

    bool send ();

    const char* getErrorMessage () const;
    const char* getData () const;

protected:

    CURL* _curl;
    std::vector<char> _data;
    char _errorMessage[CURL_ERROR_SIZE];
};
