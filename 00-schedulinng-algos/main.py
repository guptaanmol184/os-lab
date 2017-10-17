from kernel import *

# get the number of processes
no_of_processes = int(input("Enter number of input processes: "))
processes = []  # create a list for storing processes

# ask if they want priority
is_priority = str(input("Do you want to specify priority? (y/n)"))
is_priority = is_priority.lower()


# get input processes
if is_priority == "n":
	for i in range(no_of_processes):
		arrival_time = int(input("Enter the arrival time: "))
		burst_time = int(input("Enter the burst time: "))
		# create process object
		temp_process = process(arrival_time, burst_time, "-")
		processes.append(temp_process)
else:
	for i in range(no_of_processes):
		arrival_time = int(input("Enter the arrival time: "))
		burst_time = int(input("Enter the burst time: "))
		priority = int(input("Enter priority: "))
		# create process object
		temp_process = process(arrival_time, burst_time, priority)
		processes.append(temp_process)


# schedule the processes using the scheduler object
s = scheduler(processes)  # pass processes to the scheduler
s.print_processes()       # print the input processes
s.rr()			  # perform function
s.print_processes()		  # print the processes with waiting time petails
s.print_average()		  # print the values of avg wt and avg tat
