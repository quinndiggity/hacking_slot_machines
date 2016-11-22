import edit_distance
import itertools
import numpy as np


Y = np.fromiter(itertools.permutations(range(4), 'i4,i4,i4,i4'))

X = np.array()[np.eye(4)[i] for i in Y])
