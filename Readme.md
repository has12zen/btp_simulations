# Assumptions:
the adversary has access to f nodes. 
the faulty nodes can send decision message or can be silent.

# Algorithm:
PBFT used for checking safety.

# Requirements:
1. Consensus should be maintained even if f nodes are faulty. (Safety)
2. The Client should eventually receive a decision message. (Liveness)

# Results:
Let N be the total number of nodes in the system.
Let f be the number of faulty nodes in the system.

for Safety:
N > f+1
ie N >=f+2
ie 2 non faulty nodes in presence of f faulty nodes can still achieve consensus

### Safety simulation 
safety simulation is done in rpbft.cpp file.

for Liveness:
N >= 2f+1

### Liveness simulation 
safety simulation is done in rpbft.cpp file.


### Safety Results

|   N   |   f   | Result for assumption  | Result for Byzantine Faults |
| :---: | :---: | :--------------------: | :-------------------------- |
|   3   |   1   | No Consensus violation | Consensus Violation         |
|   4   |   1   | No Consensus violation | No Consensus Violation      |
|   4   |   2   | No Consensus violation | Consensus Violation         |
|   5   |   1   | No Consensus violation | No Consensus Violation      |
|   5   |   2   | No Consensus violation | Consensus Violation         |
|   5   |   3   | No Consensus violation | Consensus Violation         |


### Liveness Results

|   N   |   F   | Result      |
| :---: | :---: | :---------- |
|   2   |   1   | No Liveness |
|   3   |   1   | Liveness    |
|   4   |   1   | Liveness    |
|   3   |   2   | No Liveness |
|   4   |   2   | No Liveness |
|   5   |   2   | Liveness    |