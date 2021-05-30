#include "RESTClient.hpp"

#include <iostream>

namespace
{
  CURL* new_curl_instance()
  {
    curl_global_init(CURL_GLOBAL_ALL);
    return curl_easy_init();
  }

  size_t write_curl_data(void* data, size_t , size_t n_bytes, void* output_object)
  {
    std::string& output_stream = *(reinterpret_cast<std::string*>(output_object));
    output_stream.append(reinterpret_cast<const char*>(data), n_bytes);
    return n_bytes;
  }
}

RESTClient::RESTClient(std::string base_api_url)
  : curl_handle_(new_curl_instance())
  , base_api_url_(std::move(base_api_url))
{
  curl_easy_setopt(curl_handle_, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, write_curl_data);
}

RESTClient::~RESTClient()
{
  curl_easy_cleanup(curl_handle_);
}

nlohmann::json RESTClient::request(const std::string& req) const
{
  std::string body_data;
  curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, &body_data);

  const auto url = base_api_url_ + req;
  curl_easy_setopt(curl_handle_, CURLOPT_URL, url.c_str());

  // Currently, no checking whatsoever is done. Everything breaks horribly when a request 
  // doesn't return a nice json message.
  curl_easy_perform(curl_handle_);


  return nlohmann::json::parse(body_data);
}
