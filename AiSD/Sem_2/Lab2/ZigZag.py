import numpy as np
def zigzag(matrix):
    matrix = np.array(matrix)
    rows, cols = matrix.shape
    result = []
    for s in range(rows + cols - 1):
        if s % 2 == 0:
            y = min(s, rows - 1)
            x = s - y
            while y >= 0 and x < cols:
                result.append(matrix[y,x])
                y -= 1
                x += 1
        else:
            x = min(s, cols - 1)
            y = s - x
            while x >= 0 and y < rows:
                result.append(matrix[y,x])
                y += 1
                x -= 1
    return  np.array(result, dtype=np.int32)

def deZigzag(L, rows, cols):
    matrix = np.zeros((rows, cols))
    i = 0
    for s in range(rows + cols - 1):
        if s % 2 == 0:
            y = min(s, rows - 1)
            x = s - y
            while y >= 0 and x < cols:
                matrix[y,x]=L[i]
                y -= 1
                x += 1
                i+=1
        else:
            x = min(s, cols - 1)
            y = s - x
            while x >= 0 and y < rows:
                matrix[y,x]=L[i]
                y += 1
                x -= 1
                i+=1
    return matrix.astype(int32)
