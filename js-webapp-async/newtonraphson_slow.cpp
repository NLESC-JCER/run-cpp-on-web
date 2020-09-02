#include "newtonraphson.hpp"
#include "algebra.cpp"
#include <math.h>
#include <iostream>
#include <chrono>
//#include <windows.h>
#include <stdlib.h>
#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
#else
#include <unistd.h> // for usleep
#endif

using namespace algebra;

namespace rootfinding {
  void sleep_ms(int milliseconds) // cross-platform sleep function
  {
  #ifdef WIN32
      Sleep(milliseconds);
  #elif _POSIX_C_SOURCE >= 199309L
      struct timespec ts;
      ts.tv_sec = milliseconds / 1000;
      ts.tv_nsec = (milliseconds % 1000) * 1000000;
      nanosleep(&ts, NULL);
  #else
      usleep(milliseconds * 1000);
  #endif
  }
   // Define the constructor method of NewtonRaphson instances
   NewtonRaphson::NewtonRaphson(double tolerancein) : tolerance(tolerancein) {}

   // Define the 'solve' method of NewtonRaphson instances
   double NewtonRaphson::solve(double xin) {
      double x = xin;
      double delta_x = equation(x) / derivative(x);

      while (fabs(delta_x) >= tolerance) {
         delta_x = equation(x) / derivative(x);
         x = x - delta_x;
      }
      sleep_ms(5000);
      return x;
   };
}
