README
Assuming you are in the correct directory.
game.c can be compiled using "gcc game.c -l pthreads -o game" or "gcc game.c -l pthreads", which allows game.c to then be run using "./game" or "./a.out" respectively.
game.c needs one variable in order to run properly which will be the seed for the random variable that will also be a part of the log file.
Thus the command will look like this "./game 12" in order to run properly.
                                            /|\
						         |
						       seed "seed" being an integer representing seed used for the random variable.