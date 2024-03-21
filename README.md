<h1 align="center">
	42cursus_Philosophers
</h1>

<p align="center">
	<b><i>Basics of process threading, and how work on the same memory space.</i></b><br>
</p>

---

Philosophers an individual project at [42](https://www.42.fr/42-network/) about introduction to the basics of process threading, and how work on the same memory space.
And learn about mutexes, semaphores, and shared memory.

## Introduction

In computer science, the dining philosophers problem is an example problem often used in concurrent algorithm design to illustrate synchronization issues and techniques for resolving them.

It was originally formulated in *1965 by Edsger Dijkstra* as a student exam exercise, presented in terms of computers competing for access to tape drive peripherals. Soon after, Tony Hoare gave the problem its present formulation

[More informations](https://en.wikipedia.org/wiki/Dining_philosophers_problem)

## Installation 🖥

To test the project, clone the repository and then run the command `make`. Then, you can test the project using the executable.

__usage :__

`./philo number_philosopher time_to_die time_to_eat time_to_sleep [number_of_time_each_philosophers_must_eat]`
arguments in square brackets are not required (number_of_time_each_philosophers_must_eat)

__example :__

`./philo_one 4 500 200 200`
