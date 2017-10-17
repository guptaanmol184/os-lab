import random

class process(object):
	"""Create an object for a process

	Attributes:
		pid: An integer representing the uique process id
		at: An integer representing the arrival time of the process
		bt: An integer representing the burst time of the process
	"""

	def __init__(self, at, bt, priority):
		"""Returns a process object whose arrival time is *at*, 
		pid is a randomly generated integer between 0 and 9999
		and bt is the *burst time*
		"""
		self.pid = random.randint(0, 9999) #assigns a random pid
		self.at = at
		self.bt = bt
		self.ct = '-'
		self.tat = '-'
		self.wt = '-'
		self.priority = priority

class scheduler(object):
	"""Creates an object for a scheduler
	"""

	def __init__(self, process_list):
		"""Returns a scheduler object with the process_list as
		passed. Used to schedule the processes.
		"""
		self.process_list = process_list

	def print_processes(self):
		"""Prints the details of the processes in process_list
		"""
		print("\nPID\tAT\tBT\tCT\tTAT\tWT\tPri")
		for i in range(len(self.process_list)):
			print(str(self.process_list[i].pid) + "\t" +
			      str(self.process_list[i].at)  + "\t" +
			      str(self.process_list[i].bt)  + "\t" +
			      str(self.process_list[i].ct)  + "\t" +
			      str(self.process_list[i].tat) + "\t" + 
			      str(self.process_list[i].wt)  + "\t" +
			      str(self.process_list[i].priority) )

	def print_average(self):
		"""Prints the average values of waiting time and 
		turn around time of the processes in the process_list
		"""
		print("Average wt: ", sum(p.wt for p in self.process_list)/len(self.process_list))
		print("Average tat: ", sum(p.tat for p in self.process_list)/len(self.process_list))

	def calsulate_params(self):
		"""Calculates the values of waiting time and 
		turn around time based on the values of arrival 
		time, burst time and completion time

		tat = ct  - at
		wt  = tat - bt
		"""
		for i in range(len(self.process_list)):
			self.process_list[i].tat = self.process_list[i].ct - self.process_list[i].at
			self.process_list[i].wt = self.process_list[i].tat - self.process_list[i].bt 

	def fcfs(self):
		"""Performs scheduling based on the 'First come
		first serve' scheme
		"""
		print('\nPerforming FCFS...')
		self.process_list.sort(key=lambda x: (x.at, x.pid))
		self.process_list[0].ct = self.process_list[0].at + self.process_list[0].bt
		for i in range(1, len(self.process_list)):
			self.process_list[i].ct = self.process_list[i-1].ct + self.process_list[i].bt
		self.calsulate_params()

	def sjf(self):
		"""Performs scheduling based on the 'Shortest job
		first' scheme
		"""
		print('\nPerforming SJF...')
		# sort by arrival time and if arrival time is same,
		# then by pid
		self.process_list.sort(key=lambda x: (x.at, x.pid))

		# execute first process
		self.process_list[0].ct = self.process_list[0].at + self.process_list[0].bt

		# complete process list
		completed_process_list = []
		completed_process_list.append(self.process_list[0])
		ct_of_last = self.process_list[0].ct
		self.process_list.remove(self.process_list[0])

		
		for i in range(len(self.process_list[:])):
			temp_process_list = []
			# get list of variables with ct less than last ct
			for i in range(len(self.process_list)):
				if self.process_list[i].at <= ct_of_last and self.process_list[i].ct == '-':
					temp_process_list.append(self.process_list[i])

			# sort by bt, at, pid
			temp_process_list.sort(key=lambda x: (x.bt, x.at, x.pid))

			# complete first process
			completed_process_list.append(temp_process_list[0])
			self.process_list.remove(temp_process_list[0])
			temp_process_list[0].ct = ct_of_last + temp_process_list[0].bt
			ct_of_last = temp_process_list[0].ct
			del temp_process_list[:]
		self.process_list = completed_process_list
		self.calsulate_params()
        
	def srt(self):
		"""Performs scheduling based on 'Shortest remaining
		time' scheme
		"""
		print('\nPerforming SRT...')
		# sort by arrival time and if arrival time is same,
		# then by pid
		self.process_list.sort(key=lambda x: (x.at, x.pid))

		# add remaining time equal to burst time to all the objects
		for i in range(len(self.process_list)):
			self.process_list[i].rt = self.process_list[i].bt
		
		# set time to -1
		time = -1
		# set no of completed processes to 0
		completed_process_list = []
		# processes to process list
		to_run = []
		
		# while all processes are not completed do this
		while(1):
			#increment time
			time += 1

			# check if any process has arrived and add to to_run list
			for i in range(len(self.process_list)):
				if self.process_list[i].at == time:
					to_run.append(self.process_list[i])

			# sort processes by remaining time
			to_run.sort(key=lambda x: (x.rt,x.at))

			# run first process fot 1 unit time
			# only if to_run is not empty
			if to_run:
				to_run[0].rt=to_run[0].rt-1

				# check if it's rt is 0, increment complete processes, remove from to run list
				# update ct, add to completed list
				if to_run[0].rt == 0:
					to_run[0].ct = time+1
					completed_process_list.append(to_run[0])
					to_run.remove(to_run[0])

			# check if completed process list == process_list
			if len(completed_process_list) == len(self.process_list):
				break

		# print completed list
		self.process_list = completed_process_list
		#for i in range(len(completed_process_list)):
		#	print(str(completed_process_list[i].pid) + "\t" + str(completed_process_list[i].at) + "\t" + str(completed_process_list[i].bt) + "\t" + str(completed_process_list[i].ct) + "\t" + str(completed_process_list[i].tat) + "\t" + str(completed_process_list[i].wt))

		# calculate params
		self.calsulate_params()

	def priority_npe(self):
		"""Performs scheduling based on the 'Highest priority
		- in a non-premptive fashion' scheme
		"""
		print('\nPerforming priority_npe...')

		# sort by arrival time and if arrival time is same,
		# then by pid
		self.process_list.sort(key=lambda x: (x.at, x.pid))

		# execute first process
		self.process_list[0].ct = self.process_list[0].at + self.process_list[0].bt

		# complete process list
		completed_process_list = []
		completed_process_list.append(self.process_list[0])
		ct_of_last = self.process_list[0].ct
		self.process_list.remove(self.process_list[0])

		
		for i in range(len(self.process_list[:])):
			temp_process_list = []
			# get list of variables with ct less than last ct
			for i in range(len(self.process_list)):
				if self.process_list[i].at <= ct_of_last and self.process_list[i].ct == '-':
					temp_process_list.append(self.process_list[i])

			# sort by (-)priority, at, pid
			temp_process_list.sort(key=lambda x: (-x.priority, x.at, x.pid))

			# complete first process
			completed_process_list.append(temp_process_list[0])
			self.process_list.remove(temp_process_list[0])
			temp_process_list[0].ct = ct_of_last + temp_process_list[0].bt
			ct_of_last = temp_process_list[0].ct
			del temp_process_list[:]
		self.process_list = completed_process_list
		self.calsulate_params()

	def hrrn(self):
		"""Performs scheduling based on the 'Highest response ratio next' 
		scheme
		"""

		print('\nPerforming HRRN...')

		# sort by arrival time and if arrival time is same,
		# then by pid
		self.process_list.sort(key=lambda x: (x.at, x.pid))

		# execute first process
		self.process_list[0].ct = self.process_list[0].at + self.process_list[0].bt

		# complete process list
		completed_process_list = []
		completed_process_list.append(self.process_list[0])
		ct_of_last = self.process_list[0].ct
		self.process_list.remove(self.process_list[0])

		# while there are processes to be scheduled
		while self.process_list:
			temp_process_list = []
			# for processes whose at is <= ct_of_last
			for i in range(len(self.process_list[:])):
				if self.process_list[i].at <=ct_of_last and self.process_list[i].ct == '-':
					temp_process_list.append(self.process_list[i])
					# rr = ( (ct_of_lat - at) + bt )/ bt
					self.process_list[i].rr = ( (ct_of_last - self.process_list[i].at) + self.process_list[i].bt )/self.process_list[i].bt

			# sort by (-)rr, at, pid
			temp_process_list.sort(key=lambda x: (-x.rr, x.at, x.pid))

			# complete first process
			completed_process_list.append(temp_process_list[0])
			self.process_list.remove(temp_process_list[0])
			temp_process_list[0].ct = ct_of_last + temp_process_list[0].bt
			ct_of_last = temp_process_list[0].ct
			del temp_process_list[:]
			
		self.process_list = completed_process_list
		self.calsulate_params()

	def rr(self):
		"""Performs scheduling based on the 'Round Robin' 
		scheme

		Time Quantum = 2
		"""

		print('\nPerforming RR...')

		# sort by arrival time and if arrival time is same,
		# then by pid
		self.process_list.sort(key=lambda x: (x.at, x.pid))

		# define the time quantum
		tq = int(input("Time quantum: "))
		print("TQ set to " + str(tq))

		# rq = []							# ready queue
		completed_process_list = []		# completed_process_list
		input_process_count = len(self.process_list)

		# initialisation of rq
		time = self.process_list[0].at
		# add all processes with arrival time = time
		rq = [x for x in self.process_list if x.at == time]
		self.process_list = [x for x in self.process_list if x not in rq]
		flag=0

		# start loop
		while len(completed_process_list) != input_process_count:
			# if rq is not empty
			if rq:
				# case one - burst time more than tq
				if rq[0].bt > tq:
					time = time + tq
					rq[0].bt = rq[0].bt-tq
					rq[0].ct = time
					temp_process = rq[0]
					rq.remove(rq[0])

					flag=1 # shows below that it was case 1
				# case two - busrt time equal to tq 
				elif rq[0].bt == tq:
					time = time + tq
					rq[0].bt = 0
					rq[0].ct = time
					# add to complete list
					completed_process_list.append(rq[0])
					#print(str(rq[0].at) + ' '+ str(rq[0].bt))
					del rq[0]
				#case three - bt less than tq
				else:
					time = time + rq[0].bt
					rq[0].bt=0
					rq[0].ct = time
					completed_process_list.append(rq[0])
					#print(str(rq[0].at) + ' '+ str(rq[0].bt))
					del rq[0]
			else:
				# sort by arrival time and if arrival time is same,
				# then by pid
				self.process_list.sort(key=lambda x: (x.at, x.pid))
				# pick topmost process from the list and shift time
				# like initialisation
				time = self.process_list[0].at
				# add all processes with arrival time = time
				rq.extend([x for x in self.process_list if x.at == time])
				# remove the processes gone to rq from process_list
				self.process_list = [x for x in self.process_list if x not in rq]	

			# find new processes from process list
			for i in range(len(self.process_list)):
				if self.process_list[i].at <= time:
					rq.append(self.process_list[i])

			# append the temp process back to rq
			# since it is not over
			if flag == 1:
				rq.append(temp_process)
				flag=0 # set flag back to 0

			# remove rq processes from process_list
			self.process_list = [x for x in self.process_list if x not in rq]

		self.process_list = completed_process_list
		self.calsulate_params()

	"""
	def psome(self, plist):
		for i in range(len(plist)):
			print(str(plist[i].pid) + "\t" +
			      str(plist[i].at)  + "\t" +
			      str(plist[i].bt)  + "\t" +
			      str(plist[i].ct)  + "\t" +
			      str(plist[i].tat) + "\t" + 
			      str(plist[i].wt)  + "\t" +
			      str(plist[i].priority) )
	"""

	def priority_pe(self):
		"""Performs scheduling based on the 'Highest priority
		- in a premptive fashion' scheme
		"""
		print('\nPerforming priority_pe...')

		# sort by arrival time and if arrival time is same,
		# then by pid
		self.process_list.sort(key=lambda x: (x.at, x.pid))
		
		# set time to -1
		time = -1
		# set no of completed processes to 0
		completed_process_list = []
		# processes to process list
		to_run = []
		
		# while all processes are not completed do this
		while(1):
			#increment time
			time += 1

			# check if any process has arrived and add to to_run list
			for i in range(len(self.process_list)):
				if self.process_list[i].at == time:
					to_run.append(self.process_list[i])

			# sort processes by priority time
			to_run.sort(key=lambda x: (-x.priority,x.at))

			# run first process fot 1 unit time
			# only if to_run is not empty
			if to_run:
				to_run[0].bt=to_run[0].bt-1

				# check if it's bt is 0, increment complete processes, remove from to run list
				# update ct, add to completed list
				if to_run[0].bt == 0:
					to_run[0].ct = time+1
					completed_process_list.append(to_run[0])
					to_run.remove(to_run[0])

			# check if completed process list == process_list
			if len(completed_process_list) == len(self.process_list):
				break

		# print completed list
		self.process_list = completed_process_list
		self.calsulate_params()
			

