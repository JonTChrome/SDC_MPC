# Model Predictive Control
Self-Driving Car Engineer Nanodegree Program Term 2

This project is an introduction to Model Predictive Control methods. This method involves simulating actuators, predicting the resulting output and chosing the inputs that will minimize the cost function. Keeping track of the state involves the state vector with the position and angle and velocity and the actuators are the accelerator input and the steering angle. The base project code can be found [here](https://github.com/udacity/CarND-MPC-Project)

## Basic Build Instructions
(From original project repo)

1. mkdir build
2. cd build
3. cmake ..
4. make
5. ./mpc

# Model

## MPC
This Model Predictive Control algorithm uses kinematic models to predict a tragectory based on the current state (position, velocity, angle) and choses actuator outputs for the next timestep that minimizes a cost function.  For each subsequent timestep, a new tragectory is calculated to try eliminate some of the error in the model.   The cost function is described in the following section.

The current state is described by a 2D set of state variables as follows:
* x, y : The Car position in a 2D grid
* psi : Car's Heading
* v : Car's Velocity
* cte : Cross-Track Error
* epsi : Orientation Error

The Kinematic model used is as follows (dt = deltaT, timestep):
'''
x[t + 1] = x[t] + v[t] * cos(psi[t]) * dt;
y[t + 1] = y[t] + v[t] * sin(psi[t]) * dt;
psi[t + 1] = psi[t] + v[t] / Lf * delta[t] * dt;
v[t + 1] = v[t] + a[t] * dt;
cte[t + 1] = f(x[t]) - y[t] + v[t] * sin(epsi[t]) * dt;
epsi[t + 1] = psi[t] - psiDes[t] + v[t] / Lf * dt * delta[t]
'''

## Cost Function
So there are 7 components to influence the cost function and we were tasked with tuning the weights for each component to influence the behavior of the model.  The largest weights were the ones associated with the delta of subsequent actuator values and the steering actuator.  This smoothes out the actions and tries to implement a smooth ride without sharp turns or changes in speed.  We gave a reference velocity to compare the error of the velocity value which we weighted the lowest because of the implemented physics of the simulator and instead weighted the steering error and cte higher.

## Time Steps
I chose N as 10 and dt as .1 for a total T of 1.  This was a kind of guess based on the environment of the simulators general speeds and turns.  This actually affects the way that the vehicle behaves and influences the reference speed to reduce the error on.  This is because it is the path planned in seconds ahead of the car which can vary in speed

## Pre-Process
The coordinates are pre-processed by transforming them to the vehicle's coordinate system with (0, 0) being the vehicles coordinate and the path coming from there.

## Latency
Since there is a control input lag of a given 100ms according to the project simulator, it is desired to have a more accurate state of the vehicle when the actuall actuator inputs are executed.  To account for this in the model, the current state is run through the kinematic model to calculate a prediction of where the state will be after the given latency time.  This new prediction is then used as the current state when calculating the predicted trajectory.


