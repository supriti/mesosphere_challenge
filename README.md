Scheduling Algorithm Simulator

Input File Format

1. First line contains tuples denoting Compute Node id and available resource units. 
	The line ends with '!'.
	eg. 
	(1 4) (2 5)!
	There are 2 compute nodes. Node 1 has 4 resource units and Node 2 has 5 resource units.

2. The second line onwards contains tuples of tasks describing required resource unit 
	and units of time required to complete tasks.
	eg. 
	(3 4)
	(4 10)
	First job requires 3 resource units for 4 units of time. 


Assumptions:
	1. I assume that the tasks are announced at each time step.


Scheduling Algorithm:
	1. When a new task is announced, I insert the task in a 'taskList' which is a global list for 
		storing the pending tasks which have not yet been alloted to a compute node.
	2. The taskList maintains tasks in a Shortest Job First Order. So when a new task arrives, we
		iterate over the list, and insert it in increasing order of execution time required.
	3. Tasks will be alloted to Compute Nodes when there is vacancy. Running tasks in a Compute Node
		are maintained in a private 'taskList' for the node.
	4. I simulate progress of time in the 'step()' function. At each step, the global time is incremented
		and the processes in the compute node progress. 
	4. I loop over the running processes in a compute node, and when a process completes, a new process
		is loaded from the global 'taskList' of pending tasks.

Output:
	At each step in time, 
		1. I print the Global Time.
		2. Status of each node.
	At the end, I print the total time units of tasks that were assigned, and the total time units needed
	to execute them using my algorithm.

Possibilities for Improvement;
	1. Code Quality can be improved.
	2. A better analysis of the performance can be done. eg. calculating the overall resource utilization per node.
	3. Algorithm can be improved.
