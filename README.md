# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---

## Background

This project aims to drive the car around the track in the Udacity [simulator](https://github.com/udacity/self-driving-car-sim/releases) by leveraging PID control loop.

Note the cross track error (CTE) is provided by the simulator, to information the algorithm how far away the car is from the center of the track at every communication instance.

## Tuning PID hyper-parameters (Manual)

Manual tuning to start is necessary due to the fact that once the car leaves the track, no more information can be gathered for automatic tuning.

This is my method to tune the PID manually:
- Kp: Start by increasing Kp until the car is capable of staying on track for the entire loop. Overshooting is okay here.
- Kd: Next increase Kd to minimize overshoot to a reasonable amount.
- Ki: Finally increase Ki to improve tracking the center of the track.

I get (Kp=0.5, Ki=0.001, Kd=1.0) respectively from the manual tuning.

Note Having high Kd makes sense as the controlled variable, steering, affects the derivative of the CTE. This makes the system very prone to overshoot. High Kd is used to compensate for this effect.

This gets the car to drive by itself endlessly for automatic tuning.

## Tuning PID hyper-parameters (Twiddle)

Twiddle (Coordinate Ascent) is used here for optimization. The cost function comprises the sum of CTE and the sum of lateral acceleration in a single loop. The lateral acceleration is included here to reduce fish-tailing. Fish-tailing contributes little to CTE but much to passenger discomfort.

The initial twiddle step size is set to 1/5 of the manual tuned hyper-parameters. The sum of lateral acceleration is magnified 80 times to balance effect of the sum of CTE.

After running the simulator for a long time we have (Kp=0.305626, Ki=0.00160386, Kd=2.31116).

## Example
<p align="center">
  <img src="example.gif" alt="PID Control Loop at Work"/>
</p>

## Other Notes
While the tuned car travels is capable of staying on track endlessly, some fish tailing is still observed. This can be due to the latency between the simulator and the controller, the randomness of the cost function, local optima, etc. It would be nice to add some randomness into the Twiddle algorithm (Ex. Random Hill-Climb), but that risks running the car off the track during tuning. In the end more time can be spent to fine tune the hyper-parameter but to get the ideal result some manual work will be required.
