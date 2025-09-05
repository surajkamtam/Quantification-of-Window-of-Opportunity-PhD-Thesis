# Threat Analysis and Probabilistic Quantification of Cyber-attack occurrence on Connected Vehicles using Window of Opportunity

This repository provides the code used for prior and posterior information, along with the datasets used for the Bluetooth and DSRC case study.   


## WOLVES framework


## Law of Total Probability

### Add some diagrams, equations, and algorithms




## Prior information processing
The data has been collected from Highways England. Then, Data cleaning, processing and PCU factor conversion have been performed for the selected 3 months of data. 

Add a map of the road segment from WebTris

## Road Network
For simulations, a road network has been created using NetEdit. The same road network has been used in both the case studies and all the scenarios. The only change required is the maximum free-flow speed of the network based on Single carriageway (60 mph) and Dual carriageway (70 mph). 


## VEINS Framework
### Diagram of the VEINS framework

### Steps for installing VEINS

### Steps for running VEINS



## VEINS FOLDER structure

### Important files to be modified


## Case Study-1
Monte Carlo simulations have been performed using only SUMO-based functionality from the VEINS framework for a short-range Bluetooth scenario. 
Five scenarios have been simulated:
1. Static Attacker on Single carriageway
2. Dynamic Attacker in the same direction as a target on a Single carriageway
3. Dynamic Attacker in the same direction as a target on a Dual carriageway
4. Dynamic Attacker in the opposite direction as a target on a Single carriageway
5. Dynamic Attacker in the opposite direction as a target on a Dual carriageway

Respective Python code to generate the scenario and save the data, processing the data can be found xxx 




## Case Study-2

Monte Carlo simulations have been performed using the DSRC stack of the VEINS framework for a short-range V2V DSRC scenario. 
Two scenarios have been simulated:
1. Single Carriageway
2. Dual Carriageway

For this case study, a dynamic threshold is used, for which a base and attack scenarios had to be considered. The datasets generated for the attack and base can be found in the folder xxx. 

The code used for training, testing, and validating the dynamic Threshold Computation using a GRU-Entropy-based anomaly detection model can be found in notebook xxx. The saved weights are xxx

Also, the anomaly detection model's performance has been improved using the Youden J index.




## Vulnerability data
Additional 



## Code for HPC
### Running VNC
### Running Notebook 


