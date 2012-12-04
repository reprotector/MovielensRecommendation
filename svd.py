from numpy import *

A = floor(random.rand(4,4)*20-10) # generating a random
b = floor(random.rand(4,1)*20-10) # system Ax=b

U,s,V = linalg.svd(A) # SVD decomposition of A

# computing the inverse using pinv
pinv = linalg.pinv(A)
# computing the inverse using the SVD decomposition
pinv_svd = dot(dot(V.T,linalg.inv(diag(s))),U.T)

print "Inverse computed by lingal.pinv()\n",pinv
print "Inverse computed using SVD\n",pinv_svd