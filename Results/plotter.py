import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("/home/user/Desktop/Linear_Solver_Parallel/Results/Convergence_Data.csv") # your path should be very similar to this

plt.semilogy(df['Iteration'], df['Residual (Jacobi)'], label='Jacobi', marker='o')
plt.semilogy(df['Iteration'], df['Residual (Gauss-Seidel)'], label='Gauss-Seidel', marker='s')
plt.semilogy(df['Iteration'], df['Residual (SOR)'], label='SOR (1.7)', marker='^') # NOTE: inside main.cpp, thats where you adjust the relaxation factor; if you are changing it, reflect that change in the SOR title here

plt.xlabel('Number of Iterations')
plt.ylabel('Residual (log scale)')
plt.title('Convergence of Parallel: Jacobi, Gauss-Seidel & SOR Algorithms')
plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.legend()
plt.tight_layout()
plt.savefig('Convergence_Data_Plot.png', dpi=300) # can also use: .jpg, pdf ...
plt.show()
