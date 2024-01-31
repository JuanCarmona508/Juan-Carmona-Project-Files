README
Assuming you are in the correct directory
pr1.cpp can be compiled using "g++ pr1.cpp -o pr1" or "g++ pr1.cpp", which allows p1.cpp to then be run using "./pr1" or "./a.out" respectively.
However pr1.cpp needs three variables in order to run properly which will be the arrival rate/lambda and the service times of the cpu and disk which are 0.02, and 0.06.
Thus the command will look like this "./pr1 12.0 0.02 0.06" 
                                            /|\
						         |
						       lambda "lambda" being a float representing arrival rate.