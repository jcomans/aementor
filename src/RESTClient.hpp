#ifndef RESTCLIENT_HPP
#define RESTCLIENT_HPP

#include <string>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

class RESTClient
{
public:
  RESTClient(std::string base_api_url);
  ~RESTClient();

  nlohmann::json request(const std::string& req) const;

private:
  CURL* curl_handle_;
  std::string base_api_url_;
};

#endif //RESTCLIENT_HPP
