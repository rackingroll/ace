SRCS = LSH_Ourlier.cpp LSH.cpp SignedRandomProjection.cpp
ALL = clean experiment 

all: $(ALL)

experiment:
	g++ -fopenmp --std=c++11 -o experiment $(SRCS) LSH_Experiment.cpp

clean:
	rm -rf $(ALL) target/
