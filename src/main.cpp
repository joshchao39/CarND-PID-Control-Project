#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include "Twiddle.h"
#include <math.h>

#define TWIDDLE_ENABLE true

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main()
{
  uWS::Hub h;

//  int batch_size = 1850; // throttle 0.2
//  int batch_size = 1400; // throttle 0.3
  int batch_size = 1000; // throttle 0.4
  int batch_i = 0;
  double cte_accumulator = 0;
  std::vector<double> cte_vec;

  // Initialize the pid variable.
//  std::vector<double> K{0.225388, 0.00144675, 1.77894}; // 0.306122, 0.00163659, 2.01707
  std::vector<double> K{0.306122, 0.00163659, 2.01707}; // 0.306122, 0.00163659, 2.01707
  std::vector<double> dK{K[0] / 5, K[1] / 5, K[2] / 5};
  Twiddle twiddle{0.2};
  std::vector<double> curr_K = twiddle.Reset(K, dK);
  PID pid;
  pid.SetK(curr_K);

  h.onMessage([&pid, &twiddle, &batch_size, &batch_i, &cte_accumulator, &cte_vec](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
//          double speed = std::stod(j[1]["speed"].get<std::string>());
//          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value;
          /*
          * Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */

          cte_accumulator += cte * cte;
          cte_vec.push_back(cte);
          if (TWIDDLE_ENABLE && batch_i == batch_size) {
            std::cout << "cte_accumulator:" << cte_accumulator << std::endl;
            std::vector<double> new_K = twiddle.ComputeNewK(cte_accumulator);
            pid.SetK(new_K);
            std::cout << "new_K:" << new_K[0] << ", " << new_K[1] << ", " << new_K[2] << std::endl;
            batch_i = 0;
            cte_accumulator = 0;
          }

          steer_value = pid.GetSteeringAngle(cte);

          batch_i++;

          // DEBUG
          std::cout << "Batch:" << batch_i << " CTE:" << cte << " Steering Value:" << steer_value << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = 0.4;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
