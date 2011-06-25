//
// Amity - cross-platform game engine
// https://github.com/aduros/amity/blob/master/LICENSE.txt

#include "net/Http.h"

#include <curl/curl.h>

#include "log.h"

static size_t onHttpReceived (char* buffer, size_t size, size_t elements, std::vector<char>* data)
{
    // Squirt it into our vector
    size_t byteSize = size*elements;
    data->insert(data->end(), buffer, buffer + byteSize);
    return byteSize;
}

Http::Http ()
{
    _curl = curl_easy_init();
    if (_curl == NULL) {
        LOGE("cURL returned NULL!? We're boned.");
    }
    curl_easy_setopt(_curl, CURLOPT_ERRORBUFFER, _errorMessage);
    curl_easy_setopt(_curl, CURLOPT_FAILONERROR, true);
    curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, true);
    curl_easy_setopt(_curl, CURLOPT_MAXREDIRS, 10);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &_data);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, onHttpReceived);
}

Http::~Http()
{
    curl_easy_cleanup(_curl);
}

void Http::setUrl (const char* url)
{
    curl_easy_setopt(_curl, CURLOPT_URL, url);
}

void Http::setPostData (const char* postData, size_t length)
{
    if (postData == NULL) {
        curl_easy_setopt(_curl, CURLOPT_HTTPGET, true);
    } else {
        curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, length);
        curl_easy_setopt(_curl, CURLOPT_COPYPOSTFIELDS, postData);
        curl_easy_setopt(_curl, CURLOPT_POST, true);
    }
}

bool Http::send ()
{
    _data.clear();
    bool success = (curl_easy_perform(_curl) == 0);
    _data.push_back('\0');
    return success;
}

const char* Http::getErrorMessage () const
{
    return _errorMessage;
}

const char* Http::getData () const
{
    return &_data.front();
}
