# Particle Simulation

This is a simulation of n particles in a 3D box. In this version it is reduced to a 2D simulation (by fixing z coordinates and setting z-velocity = 0) to animate it.

## Usage:

Requires ncurses:

```sudo apt-get install libncurses5-dev libncursesw5-dev```

Configure parameters in ``` simulation.cpp ```

then navigate to /src/ and (using g++ in this case) to compile and run execute

```g++ simulation.cpp environment.cpp particle.cpp utils.cpp ascii.cpp -lncurses && ./a.out```.

Tested only on ubuntu 18.04.

## Implementation

Collisions are calculated and stored in a priority queue, 
after initially calculating all particle-particle and particle-wall collisions,
only the particles involved in a collision need to be re-evaluated, as all other particles will continue in a straight line.

This reduces the time complexity of calculating collisions from O(n^2) (i.e. recalculating all collisions) -> O(n) (recalculating only impacted collisions).

Invalidated collisions events (i.e. those in which the colliding particle has
been involved in another collision since the creation of the event) are simply discarded.

Keeping the queue of collisions sorted has complexity O(log(n)).

This results in an efficient and fast simulation.

The ascii visualisation of the simulation (which can be set using the ascii bool in simulation.cpp) was implemented using nCurses.
