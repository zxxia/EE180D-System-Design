import numpy as np
import matplotlib.pyplot as plt
import csv

def read_file(fname):
	data = []
	with open(fname, "r") as ifile:
		reader = csv.reader(ifile)
		for row in reader:
			try:
				data.append([float(i) for i in row])
			# this will discard any rows that can't 
			# be converted to floats
			except ValueError:
				pass
				
	data = np.array(data)	
	return data
	
def main(): 
	#signal_fname 	= "Acceleration_Walk_Dataset.csv"
	#extremes_fname 	= "acceleration_strides.csv"
	signal_fname 	= "s5_17.csv"
	extremes_fname 	= "s5_stride.csv"

	# read data from file
	signal 	= read_file(signal_fname)
	extremes = read_file(extremes_fname)

	# associate signals to names from signal matrix
	t1 = signal[:,0]
	start_time = t1[0]
	t2 = signal[:, 1]
	t = [sum(x)/2.0 - start_time for x in zip(t1,t2)]
	y = signal[:, 2]
	x = signal[:, 3]

	#z = signal[:, 3]

	# associate peaks and troughs to names from signal matrix
	#S_i = extremes[:, 0]
	S_t = extremes[:, 0]
	S_x = extremes[:, 1]
	#S_tt = extremes[:, 2]
	#S_tx = extremes[:, 3]
	

	fig, ax = plt.subplots()
	fig.suptitle('Acceleration along x axis of acceleromter')
	ax.plot(t, x, linewidth=0.8)
	plt.xlabel('Time(s)')
	#plt.xlim([0, 35])
	plt.ylabel('Acceleration(m/s^2)')
	#plt.ylim([-3.5, 4.5])
	ax.scatter(S_t, S_x, marker='o', color='g', s=50)
	#ax.scatter(S_tt, S_tx, marker='o', color='r', s=50)
	# for i in range(0, len(S_i)):
	# 	if(S_x[i] > 0):
	# 		ax.axvline(S_t[i], color='r', linestyle='--')
	plt.show()
	
if __name__ == "__main__":
	main()