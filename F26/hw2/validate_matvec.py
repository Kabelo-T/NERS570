from scipy.io import mmread
import numpy as np
import matplotlib.pyplot as plt

matrix_names = ["fs_183_1",  "impcol_e", "lns_3937",  "nnc666", "s1rmq4m1"]

for name in matrix_names:
    print(f"Validating {name}")

    A = mmread("mats/" + name + ".mtx")
    x = np.loadtxt("vecs/" + name + ".vecin")
    b = A @ x
    
    b_ref = np.loadtxt("vecs/" + name + ".vecout")
    
    print(f"Max abs diff: {max(abs(b - b_ref))}\n")
