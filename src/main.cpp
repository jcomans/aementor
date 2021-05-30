#include <iostream>

#include "RESTClient.hpp"

int main(int argc, char* argv[])
{
  const std::vector<std::string> arguments(argv, argv+argc);

  RESTClient rest_client("https://www.metaweather.com/api/");

  const auto city = arguments.size() == 2 ? arguments[1] : "Amsterdam";

  const auto location = rest_client.request("location/search/?query=" + city);

  if (location.size() > 0){
      const auto woeid = location[0]["woeid"].get<int>();

      std::cout << city << " woeid: " << woeid << "\n";

      const auto weather = rest_client.request("location/" + std::to_string(woeid) + "/");

      std::cout << "Here comes the weather:\n";
      for (const auto& day: weather["consolidated_weather"]) {
        std::cout << day["applicable_date"].get<std::string>()
                  << "\tmin: "    << day["min_temp"].get<double>() << " C"
                  << "\tmax: "    << day["max_temp"].get<double>() << " C"
                  << "\twind: "   << day["wind_speed"].get<double>() * 1.60934 << " km/h@" << day["wind_direction"].get<double>() << " deg"
                  << "\t state: " << day["weather_state_name"].get<std::string>()
                  << "\n";
      }
  }
  else {
      std::cout << city << " not found\n";
  }  
}
