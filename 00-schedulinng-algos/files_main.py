from kernel import *
from os import stat

no_of_processes = 5
processes = []

# get processes
for i in ['a','b','c','d','e']:
	# create process object
	temp_process = process(int(stat(i).st_mtime), stat(i).st_size)
	processes.append(temp_process)

# print processes
print("\nPID\tAT\t\tBT\tCT\tTAT\tWT")
for i in range(len(processes)):
	print(str(processes[i].pid) + "\t" + str(processes[i].at) + "\t" + str(processes[i].bt) + "\t" + str(processes[i].ct) + "\t" + str(processes[i].tat)
		+ "\t" + str(processes[i].wt))

# schedule
s = scheduler(processes)
s.fcfs()
print("\nPID\tAT\t\tBT\tCT\t\tTAT\tWT")
for i in range(len(processes)):
	print(str(processes[i].pid) + "\t" + str(processes[i].at) + "\t" + str(processes[i].bt) + "\t" + str(processes[i].ct) + "\t" + str(processes[i].tat) + "\t" + str(processes[i].wt))

print("Average wt: ", sum(p.wt for p in processes)/len(processes))
print("Average tat: ", sum(p.tat for p in processes)/len(processes))