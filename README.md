# Fractals of Mandelbrot simulation.
_This is a Mandelbrot fractal simulator written in C++ for Linux. It generates fractals by iterating the suite_ $z_{n+1} = z_n^p + c$ _for each pixel in the image, where_ $z$ _and_ $c$ _are complex numbers, and_ $n$ _the index of the sequence. If the absolute value of_ $z$ _exceeds a certain threshold, the pixel is colored based on how many iterations were required to exceed the threshold._

Requirements:

- C++ compiler (tested with g++ 9.3.0)
- [EZ-Draw++](http://eric.remy3.free.fr/EZ-Draw++/EZ-Draw++-1.2-6/html/index.html) library
- GNU Make (optional)

---

### Usage

To build the simulator, run ```make``` in the project directory. This will generate an executable file called ```fractal```. To run the simulator, execute ```./fractal``` with the following optional command-line arguments:

- ```-p <power>``` : The power of the Mandelbrot fractal (default is 2)
- ```-i <iterations>``` : Maximum number of iterations per pixel (default is 30)

For example, To generate a mandelbrot fractal to the power of 2 with a maximum iteration of 80, here is the command to write :

```./fractal -p 2 -i 80```

The result should be:

![Example of a fractal](/images/exampleFractal.png)

You can quit the program with ```escape``` or the letter ```q```.

For clean all compilation traces, you can run ```make clean```.

---

### License

This project is licensed under the GPL License. See the LICENSE file for details.

---

### Acknowledgements

This project was inspired by the Mandelbrot set and the many great examples available online. Thanks to the EZ-Draw library for making it easy to create graphical applications in C++.
