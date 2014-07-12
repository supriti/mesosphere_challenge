#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>

using namespace std;

class task_t
{
	unsigned long startTime;
	unsigned long endTime;
	int resources;
	int reqTime;
public:
	task_t(int inResources, int inTime)
	{
		resources = inResources;
		reqTime = inTime;
	}

	void setStartTime(unsigned long inStartTime)
	{
		startTime = inStartTime;
		endTime = inStartTime + reqTime;
	}

	int getReqTime()
	{
		return reqTime;
	}

	int getEndTime()
	{
		return endTime;
	}

	int getResources()
	{
		return resources;
	}
};

class computeNode_t
{
	int id;
	int numResources;
	int freeResources;
	list<task_t> taskList;
	int waitTime;

public:
	
	computeNode_t(int inId, int inNumResources)
	{
		id = inId;
		numResources = inNumResources;
		freeResources = inNumResources;
		waitTime = 0;
	}
	
	int getId()
	{
		return id;
	}
	
	int insertTask(task_t task, unsigned long globalTime)
	{
		task.setStartTime(globalTime);
		taskList.push_back(task);
		freeResources = freeResources - task.getResources();
	}

	int step(unsigned long globalTime)
	{
		int tasksRunning = 0;

		cout << "Node " << id << ": Total Resouces = " << numResources << "; Free Resources " << freeResources << endl; 

		for(list<task_t>::iterator task_it = taskList.begin(); task_it != taskList.end(); )
		{
			task_t task = *task_it;
			if(task.getEndTime() == globalTime)
			{
				freeResources = freeResources + task.getResources();
				taskList.erase(task_it++);
			}
			else
			{
				// if here means one of the task is still running
				tasksRunning++;
				task_it++;
			}	
		}
		return tasksRunning;
	}

	int getFreeResources()
	{
		return freeResources;
	}
};

vector<computeNode_t> nodes;
list<task_t> taskList;
unsigned long globalTime = 0;

/** 
 * Function to advance by one time step.
 */
int step()
{	
	// increment global time
	int runningTasks = 0;
	
	cout <<endl<< "Global time = " << globalTime << endl;
	globalTime++;
	// iterate over all compute nodes
	for(vector<computeNode_t>::iterator node_it = nodes.begin() ; node_it != nodes.end(); ++node_it)
	{
		// call the step function for each node to progress the running processes
		computeNode_t node = *node_it;
		runningTasks += node_it->step(globalTime);

		// if free resources in node, are more than required by the first task in Task List,
		list<task_t>::iterator firstTask_it = taskList.begin();
		if (firstTask_it != taskList.end())
		{
			task_t firstTask = *firstTask_it;
			if (node_it->getFreeResources() >= firstTask.getResources())
			{
				// load the task into compute node_it->
				node_it->insertTask(firstTask, globalTime);

				// remove task from Global task list.
				taskList.erase(firstTask_it);
			}
		}
	}
	runningTasks += taskList.size();
	return runningTasks;
}

/**
 * Function to insert task in Global Task List. The tasks are inserted in 
 * a shortest job first manner.
 */
void insertInTaskList(task_t newTask)
{
	list<task_t>::iterator task_it = taskList.begin();
	for(; task_it != taskList.end(); ++task_it)
	{
		task_t task = *task_it;
		if (task.getReqTime() <= newTask.getReqTime())
			continue;
		else
			break;
	}
	if(task_it == taskList.end())
	{
		taskList.push_back(newTask);
	}
	else
	{
		taskList.insert(task_it, newTask);
	}
}

unsigned long totalTimeForTasks = 0;

void stepForAllTasks()
{
	int runningTasks;
	while(runningTasks = step());
}

int main(int argc, char* argv[])
{
	char *fileName;
	if (argc > 1)
	{
		fileName = argv[1];
	}

	ifstream input(fileName, ifstream::in);
	if (!input.good())
	{
		cout << "Cannot open file!" << endl;
		exit(1);
	}

	// vector<computeNode_t> nodes;

	{
		string line;
		getline(input, line);

		istringstream iss (line);

		// Read one tuple
		while(iss.peek() != '!')
		{
			if(iss.peek() == ' ')
				iss.ignore();
			if(iss.peek() == '(')
				iss.ignore();
			int nodeId;
			iss >> nodeId;
			if(iss.peek() == ' ')
				iss.ignore();
			int numResources;
			iss >> numResources;
			if(iss.peek() == ')')
				iss.ignore();

			cout << nodeId << " - " << numResources << endl;
			
			computeNode_t node (nodeId, numResources);
			nodes.push_back(node);
		}
	}

	// list<task_t> taskList;

	while(!input.eof())
	{
		string line;
		getline(input, line);

		istringstream iss (line);

		// Read one tuple
	
		if(iss.peek() == '(')
			iss.ignore();
		int resourcesNeeded;
		iss >> resourcesNeeded;
		if(iss.peek() == ' ')
			iss.ignore();
		int timeNeeded;
		iss >> timeNeeded;
		if(iss.peek() == ')')
			iss.ignore();

		task_t task (resourcesNeeded, timeNeeded);
		totalTimeForTasks += timeNeeded;
		insertInTaskList(task);
		step();
	}

	stepForAllTasks();

	cout << "Tasks for " << totalTimeForTasks << "units of time were scheduled!\n";
	cout << "All tasks were comlpeted in " << globalTime << "units of time." << endl;
	return 0;
}