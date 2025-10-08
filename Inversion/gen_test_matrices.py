import numpy as np

numRows = 5
numCols = numRows

numMatrices = 1000


for i in range(0, numMatrices):
    A = (np.random.randint(20, size=(numRows, numCols))) - 10
    try:
        Ainv = np.linalg.inv(A)
    except np.linalg.LinAlgError:
        continue  # skip singular matrices
    # concatenate both together
    if i < 1:
        bigMat = np.concatenate((A, Ainv), axis=1)
    else:
        bigMat = np.concatenate((bigMat, np.concatenate((A, Ainv), axis=1)), axis=0)

#save all the matrices to a .csv file
np.savetxt("test_matrices.csv", bigMat, delimiter="," , header=str(numRows))