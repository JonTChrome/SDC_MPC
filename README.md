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
## Cost Function
So there are 7 components to influence the cost function and we were tasked with tuning the weights for each component to influence the behavior of the model.  By far the largest weight were the ones associated with the delta of subsequent actuator values.  This smoothes out the actions and tries to implement a smooth ride without sharp turns or changes in speed.  We gave a reference velocity to compare the error of the velocity value which we weighted the lowest because of the implemented physics of the simulator and instead weighted the steering error and cte higher.

## Time Steps
I chose N as 10 and dt as .1 for a total T of 1.  This was a kind of guess based on the environment of the simulators general speeds and turns.  This actually affects the way that the vehicle behaves and influences the reference speed to reduce the error on.  This is because it is the path planned in seconds ahead of the car which can vary in speed

## Pre-Process
The coordinates are pre-processed by transforming them to the vehicle's coordinate system with (0, 0) being the vehicles coordinate and the path coming from there.

## Latency
Since my chosed timestep (dt) = .1 = 100 milliseconds then we can just the previous actuation instead of the current one in calcuting the predicted states.  This is done in MPC.cpp line #107


