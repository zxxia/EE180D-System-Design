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
	#peaks_fname 	= "acceleration_output.csv"
	#strides_fname = "acceleration_strides.csv"
	signal_fname 	= "s3_30.csv"
	peaks_fname 	= "peak.csv"
	strides_fname = "stride.csv"

	# read data from file
	signal 	= read_file(signal_fname)
	peaks 	= read_file(peaks_fname)
	strides = read_file(strides_fname)

	# associate signals to names from signal matrix
	t1 = signal[:, 0]
	t2 = signal[:, 1]
	t = [sum(x) for x in zip(t1, t2)]
	t[:] = [x - t1[0] for x in t]
	x = signal[:, 2]

	print(t)
	#z = signal[:, 3]

	# associate peaks and troughs to names from signal matrix
	# P_i = peaks[:, 0]
	# P_t = peaks[:, 1]
	# P_y = peaks[:, 2]
	# T_i = peaks[:, 3]
	# T_t = peaks[:, 4]
	# T_y = peaks[:, 5]

	# # associate stridess from signal matrix
	# S_i = strides[:, 0];
	# S_t = strides[:, 1];
	# S_x = strides[:, 2];

	# fig, ax = plt.subplots()
	# ax.plot(t, z, linewidth=0.8)
	# ax.scatter(P_t, P_y, marker='^', color='g', s=50)
	# ax.scatter(T_t, T_y, marker='h', color='r', s=50)
	# #ax.scatter(S_t, S_x, marker='h', color='b', s=50)
	# ax.scatter(S_t, S_x, marker='|', color='r', s=500000)
	# plt.show()
	
if __name__ == "__main__":
	main()