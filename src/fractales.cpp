#include "../include/fractales.hpp"
#include <sstream>
#include <thread>
#include <cmath>
#include <complex>

Fractale::Fractale(int w,int h,const char *name, int _power, int _max_iterations, double _xmin, double _xmax, double _ymin, double _ymax, unsigned short _pixel_step)
    : EZWindow(w,h,name), power(_power), max_iterations(_max_iterations), xmin(_xmin), xmax(_xmax), ymin(_ymin), ymax(_ymax)
{setDoubleBuffer(true);}

Fractale::Fractale(const Fractale& fractale) // Copy constructor
    : Fractale(800, 800, "Fractale", fractale.power, fractale.max_iterations, -3.5, +3.5, -1.2, +1.2, 3)
{}

std::complex<double> getPower(std::complex<double> z, const int p) {
    std::complex<double> result = 1; // We start at 1
    for (int i = 0; i < p; ++i) {
        result *= z; // We multiply by z p times
    }
    return result;
}

void display_loading_bar(int time_loading, std::string& sep) {
    std::cout << "\033[0G"; // Put the cursor at the begin of the line
    std::cout << sep;
    sep.replace(time_loading + 1, 1, "#"); // Replace the next space by a #
    std::cout.flush(); //clean the line
}

void Fractale::trace_fractale() {
    std::complex<double> z, c;

    // We calculate the scale of the fractal, based on the minimum and maximum values for x and y,
    // and the window size
    double xscale = (xmax - xmin) / getWidth(), yscale = (ymax - ymin) / getHeight();
    std::string separator = "[        ]";
    int count, time_loading;
    std::cout << "In progress. . ." << std::endl;
    for (int i = 0; i < getWidth(); ++i) {
        for (int k = 0; k < getHeight(); ++k) {
            // The loading bar will be "incremented" every 100 pixels scanned.
            if (i == time_loading * 100 && k == time_loading* 100) display_loading_bar(time_loading, separator);
            
            // we calculate the real and imaginary part of the number c, based on the pixel location
            // and zoom and position values
            c.real(k * xscale + xmin);
            c.imag(i * yscale + ymin);

            // we start z at (0, 0)
            z.real(0.);
            z.imag(0.);

            count = 0;

            // Iterate until z moves more than 2 units away from (0, 0),
            // or until we've iterated too many times
            while ((pow(z.real(),2) + pow(z.imag(), 2) < 4) && (count < max_iterations)) {
                // Calculate the Mandelbrot function

                // z = z^POWER + c where z and c are complex numbers
                z = getPower(z, power) + c;
  
                // count is the number of iterations
                count++;
            }
  
            // Display the result as a pixel
            if (count > max_iterations/2 && count < max_iterations) setColor(EZColor::cyan);
            else if (count >= max_iterations) setColor(EZColor::black); // If it diverges
            else setColor(EZColor::blue); // If it converges

            drawPoint(i, k);
        }
    }
    
    display_loading_bar(time_loading, separator); // We display the end of the loading bar.
    std::cout << std::endl << "finished !" << std::endl;
}

void Fractale::keyPress(EZKeySym keysym) {
    switch (keysym) {
        case EZKeySym::Escape:
        case EZKeySym::q :
          EZDraw::quit(); // If the user presses q or Escape, we quit the program
          break;
        default:
          break;
     }
}
