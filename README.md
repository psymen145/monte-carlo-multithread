## Assignment 4 - Calculating PI using many threads

In this homework, a group of threads will be used to estimate the value of PI (3.1415926...) using a Monte Carlo method. The main thread will create all the additional threads which will perform the actual calculation.

The main thread reads two command line arguments. The first is the total number of additional threads, numthreads, and the second is N, the number of Monte Carlo samples per thread.

The calculation of PI is a Monte Carlo estimate of the integral 

∫(x^2 + y^2) dxdy 

with the constraint that (x^2 + y^2) ≤ 1. This corresponds to calculating the area of a quarter circle, I, whose radius is 1 and so, PI = 4*I.

Monte Carlo estimate of pi:

A Monte Carlo estimate of an integral is given by 

I ≅ (∑f(i))/N, N = number of values of i 

where f(i) is the integrand which is randomly chosen in the allowed domain. In our case, the domain is 0 ≤ x ≤ 1 and 0 ≤ y ≤ 1. 

        	1    	(x^2 + y^2) ≤ 1 
f(x,y) = 
        	0    	(x^2 + y^2) > 1 

x and y are randomly chosen in the range (0,1) using the math library function rand() or random() (read the man pages describing them). Repeat the sampling of x and y, N times, and sum the values of f(x,y) (which will be either 0 or 1 from above) and divide by N. PI is then approximately 4 times the result.
