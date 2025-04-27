# script to plot convergence rates 

import pandas as pd
import matplotlib.pyplot as plt
import os
import glob

def plot_convergence():

	# find the csv files
	csv_files = glob.glob("*_convergence.csv")

	# script unable to find files to plot
	if not csv_files:

		print("No CSV Files Found!")

		return

	# figure size for plot
	plt.figure(figsize=(10, 10))

	# iterate over all csv files
	for csvFile in csv_files:

		# read each csv file
		data = pd.read_csv(csvFile)

		# get appropriate iterative name for plotting
		methodName = os.path.splitext(csvFile)[0].replace("_convergence", "")

		# plot residuals vs. iteration
		plt.semilogy(data["Iteration"], data["Residual"], label=methodName, marker='o', markersize=4, linewidth=2)


	# labels for plot
	plt.xlabel("Iteration")
	plt.ylabel("Residual (log scale)")
	plt.title("Convergence Rates of Iterative Methods")
	plt.grid(True, which="both", linestyle="--", linewidth=0.5)
	plt.legend()
	plt.tight_layout()
	plt.show()


if __name__ == "__main__":
	plot_convergence()
