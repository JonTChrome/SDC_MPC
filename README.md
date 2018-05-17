# Model Predictive Control
Self-Driving Car Engineer Nanodegree Program Term 2

This project is an introduction to PID Control methods and the infulence that each component has on the behavior of the output, in this case the steering wheel angle of a simulated vehicle.  This project is an implementation of a PID controller in C++ with tuned weights for each component in PID.  The base project code can be found [here](https://github.com/udacity/CarND-PID-Control-Project)

## Basic Build Instructions
(From original project repo)

1. mkdir build
2. cd build
3. cmake ..
4. make
5. ./particle_filter

# PID Components
## Proportional 
This is the part that is directly proportional to the cte and is responsible for steering the vehicle towards the center.  This term alone will result in an oscilation around the desired minimized cte.

## Integral
Integral portion is used to eliminate the bias of the controlled system.  For the simulator, no bias is present and therefore the weight is set to 0.

## Derivative
This part is proportional to the derivative of the cte and is used to reduce oscillations about the center.

# Parameter Tuning
So I tried twiddle for a bit but I was not able to get great results so I moved on to manual tuning.  Starting with the parameters given in the lesson of [0.2, 0.004, 3.0] ([P,I,D]) as a jumping off point I used trial and error for different values and eventually settled on the final results.  [0.10, 0.0, 3.0]

