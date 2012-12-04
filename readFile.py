import shlex

f = open('u1.test')
lines = f.readlines()
f.close()

#users x movies
table= [ [ -1 for i in range(1682) ] for j in range(943) ]


for line in lines:
	split = shlex.split(line)
	user =  int(split[0])
	movie = int(split[1])
	rate = int(split[2])
	table[user][movie] = rate
	print "user: ", user, "Movie: ", movie, "Rate: ", table[user][movie]

#for row in table:
    #print row
	