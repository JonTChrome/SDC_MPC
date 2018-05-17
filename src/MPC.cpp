#include "MPC.h"
#include <cppad/cppad.hpp>
#include <cppad/ipopt/solve.hpp>
#include "Eigen-3.3/Eigen/Core"

using CppAD::AD;

// TODO: Set the timestep length and duration
size_t N = 10;

// This value assumes the model presented in the classroom is used.
//
// It was obtained by measuring the radius formed by running the vehicle in the
// simulator around in a circle with a constant steering angle and velocity on a
// flat terrain.
//
// Lf was tuned until the the radius formed by the simulating the model
// presented in the classroom matched the previous radius.
//
// This is the length from front to CoG that has a similar radius.



//Reference values
const double ref_v = 50;

//WEIGHTS

const double w_cte = 3000;
const double w_epsi = 3000;
const double w_v = 1;
const double w_delta = 5;
const double w_a = 5;
const double w_delta_s = 1e7;
const double w_a_s = 1e7;

//Start indices
const double xstart = 0;
const double ystart = xstart + N;
const double psi_start = ystart + N;
const double v_start = psi_start + N;
const double cte_start = v_start + N;
const double epsi_start = cte_start + N;
const double delta_start = epsi_start + N;
const double a_start = delta_start + N - 1;

const double steering_bound = 0.436;
const double delta_bound =  1.0e19;
const double a_bound = 1;

class FG_eval {
 public:
  // Fitted polynomial coefficients
  Eigen::VectorXd coeffs;
  FG_eval(Eigen::VectorXd coeffs) { this->coeffs = coeffs; }
    

  typedef CPPAD_TESTVECTOR(AD<double>) ADvector;
  void operator()(ADvector& fg, const ADvector& vars) {
    // TODO: implement MPC
    // `fg` a vector of the cost constraints, `vars` is a vector of variable values (state & actuators)
    // NOTE: You'll probably go back and forth between this function and
    // the Solver function below.
    //CTE
    fg[0] = 0;
      //reference to current state
    for(int t = 0; t < N; t++) {
        fg[0] += w_cte * CppAD::pow(vars[cte_start + t], 2);
        fg[0] += w_epsi * CppAD::pow(vars[epsi_start + t], 2);
        fg[0] += w_v * CppAD::pow(vars[v_start + t] - ref_v, 2);
    }
    //actuators
    for(int t = 0; t < N - 1; t++) {
        fg[0] += w_delta * CppAD::pow(vars[delta_start + t], 2);
        fg[0] += w_a * CppAD::pow(vars[a_start + t], 2);
    }
    
    //sequential smoothing
    for(int t = 0; t < N - 2; t++) {
        fg[0] += w_delta_s * CppAD::pow(vars[delta_start + t + 1] - vars[delta_start + t], 2);
        fg[0] += w_a_s * CppAD::pow(vars[a_start + t + 1] - vars[a_start + t], 2);
    }
     
      fg[1 + xstart] = vars[xstart];
      fg[1 + ystart] = vars[ystart];
      fg[1 + psi_start] = vars[psi_start];
      fg[1 + v_start] = vars[v_start];
      fg[1 + cte_start] = vars[cte_start];
      fg[1 + epsi_start] = vars[epsi_start];
      
      for(int t = 0; t < N - 1; t++) {
          AD<double> x0 = vars[xstart + t];
          AD<double> y0 = vars[ystart + t];
          AD<double> psi0 = vars[psi_start + t];
          AD<double> v0 = vars[v_start + t];
          AD<double> cte0 = vars[cte_start + t];
          AD<double> epsi0 = vars[epsi_start + t];
          AD<double> delta0 = vars[delta_start + t];
          AD<double> a0 = vars[a_start + t];
          
          AD<double> x1 = vars[xstart + t + 1];
          AD<double> y1 = vars[ystart + t + 1];
          AD<double> psi1 = vars[psi_start + t + 1];
          AD<double> v1 = vars[v_start + t + 1];
          AD<double> cte1 = vars[cte_start + t + 1];
          AD<double> epsi1 = vars[epsi_start + t + 1];
          if (t > 1) {
              a0 = vars[a_start + t - 2];
              delta0 = vars[delta_start + t - 2];
          }
          AD<double> f0 = 0.0;
          for (int i = 0; i < coeffs.size(); i++) {
              f0 += coeffs[i] * CppAD::pow(x0, i);
          }
          AD<double> psides0 = 0.0;
          AD<double> psi_des0 = CppAD::atan(coeffs[0] + 2*coeffs[1]*x0 + 3*coeffs[2]*pow(x0,2));

          fg[2 + xstart + t] = x1 - (x0 + v0 * CppAD::cos(psi0) * dt);
          fg[2 + ystart + t] = y1 - (y0 + v0 * CppAD::sin(psi0) * dt);
          fg[2 + psi_start + t] = psi1 - (psi0 + v0 / Lf * delta0 * dt);
          fg[2 + v_start + t] = v1 - (v0 + a0 * dt);
          fg[2 + cte_start + t] = cte1 - ((f0 - y0) + (v0 * CppAD::sin(epsi0) * dt));
          fg[2 + epsi_start + t] = epsi1 - ((psi0 - psides0) + v0 / Lf * delta0 * dt);
      }
  }
};

//
// MPC class definition implementation.
//
MPC::MPC() {}
MPC::~MPC() {}

vector<double> MPC::Solve(Eigen::VectorXd state, Eigen::VectorXd coeffs) {
  bool ok = true;
//  size_t i;
  typedef CPPAD_TESTVECTOR(double) Dvector;

  // TODO: Set the number of model variables (includes both states and inputs).
  // For example: If the state is a 4 element vector, the actuators is a 2
  // element vector and there are 10 timesteps. The number of variables is:
  //
  // 4 * 10 + 2 * 9
    const double x = state[0];
    const double y = state[1];
    const double psi = state[2];
    const double v = state[3];
    const double cte = state[4];
    const double epsi = state[5];
    
  size_t n_constraints = N * 6;
  size_t n_vars = n_constraints + (N - 1) * 2;
  //

  // Initial value of the independent variables.
  // SHOULD BE 0 besides initial state.
  Dvector vars(n_vars);
  for (int i = 0; i < n_vars; i++) {
    vars[i] = 0;
  }
    
    vars[xstart] = x;
    vars[ystart] = y;
    vars[psi_start] = psi;
    vars[v_start] = v;
    vars[cte_start] = cte;
    vars[epsi_start] = epsi;


  Dvector vars_lowerbound(n_vars);
  Dvector vars_upperbound(n_vars);
  // TODO: Set lower and upper limits for variables.
    for (int i = 0; i < delta_start; i++) {
        vars_lowerbound[i] = -delta_bound;
        vars_upperbound[i] = delta_bound;
    }
    
    for (int i = delta_start; i < a_start; i++) {
        vars_lowerbound[i] = -steering_bound;
        vars_upperbound[i] = steering_bound;
    }
    // Acceleration/decceleration upper and lower limits
    for (int i = a_start; i < n_vars; i++) {
        vars_lowerbound[i] = -a_bound;
        vars_upperbound[i] = a_bound;
    }

  // Lower and upper limits for the constraints
  // Should be 0 besides initial state.
  Dvector constraints_lowerbound(n_constraints);
  Dvector constraints_upperbound(n_constraints);
  for (int i = 0; i < n_constraints; i++) {
      constraints_lowerbound[i] = 0;
      constraints_upperbound[i] = 0;
  }
  
    constraints_lowerbound[xstart] = x;
    constraints_lowerbound[ystart] = y;
    constraints_lowerbound[psi_start] = psi;
    constraints_lowerbound[v_start] = v;
    constraints_lowerbound[cte_start] = cte;
    constraints_lowerbound[epsi_start] = epsi;
    
    constraints_upperbound[xstart] = x;
    constraints_upperbound[ystart] = y;
    constraints_upperbound[psi_start] = psi;
    constraints_upperbound[v_start] = v;
    constraints_upperbound[cte_start] = cte;
    constraints_upperbound[epsi_start] = epsi;
  // object that computes objective and constraints
  FG_eval fg_eval(coeffs);

  //
  // NOTE: You don't have to worry about these options
  //
  // options for IPOPT solver
  std::string options;
  // Uncomment this if you'd like more print information
  options += "Integer print_level  0\n";
  // NOTE: Setting sparse to true allows the solver to take advantage
  // of sparse routines, this makes the computation MUCH FASTER. If you
  // can uncomment 1 of these and see if it makes a difference or not but
  // if you uncomment both the computation time should go up in orders of
  // magnitude.
  options += "Sparse  true        forward\n";
  options += "Sparse  true        reverse\n";
  // NOTE: Currently the solver has a maximum time limit of 0.5 seconds.
  // Change this as you see fit.
  options += "Numeric max_cpu_time          0.5\n";

  // place to return solution
  CppAD::ipopt::solve_result<Dvector> solution;

  // solve the problem
  CppAD::ipopt::solve<Dvector, FG_eval>(
      options, vars, vars_lowerbound, vars_upperbound, constraints_lowerbound,
      constraints_upperbound, fg_eval, solution);

  // Check some of the solution values
  ok &= solution.status == CppAD::ipopt::solve_result<Dvector>::success;

  // Cost
  auto cost = solution.obj_value;
  std::cout << "Cost " << cost << std::endl;

  // TODO: Return the first actuator values. The variables can be accessed with
  // `solution.x[i]`.
  //
  // {...} is shorthand for creating a vector, so auto x1 = {1.0,2.0}
  // creates a 2 element double vector.

    vector<double> result;
    result.push_back(solution.x[delta_start]);
    result.push_back(solution.x[a_start]);
    for (int i = 0; i < N - 1; ++i) {
       result.push_back(solution.x[xstart + i + 1]);
       result.push_back(solution.x[ystart + i + 1]);
    }
    
    return result;
}