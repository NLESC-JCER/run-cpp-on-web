#ifndef H_NEWTONRAPHSON_H
#define H_NEWTONRAPHSON_H

class NewtonRaphson {
   public:
      NewtonRaphson(double tolerancein);
      double solve(double xin);
   private:
      double tolerance;
};
#endif
