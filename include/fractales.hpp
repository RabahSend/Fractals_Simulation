#ifndef FRACTALES_HPP
#define FRACTALES_HPP

#include "ez-draw++.hpp"

class Fractale : public EZWindow {
    private:
        int power, max_iterations;
        double xmin,xmax,ymin,ymax; // Les valeurs minimum et maximum de la fenetre de visibilite.

    public:
        Fractale(int w,int h, const char *name, int _power, int _max_iterations, double _xmin, double _xmax, double _ymin, double _ymax, unsigned short _pixel_step);
        Fractale(const Fractale&);
        inline ~Fractale() {}
        void trace_fractale();
        inline void expose() { trace_fractale(); }
        void keyPress(EZKeySym);
};

class App : public EZDraw {
 private:
  Fractale frac;
 public:
  App(int power, int max_it)
   : frac(800, 800, "Mandelbrot fractal", power, max_it, -2., +1, -1.5, +1.5, 2)
  {}
};

#endif
