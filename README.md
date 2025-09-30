# Threat Analysis and Probabilistic Quantification of Cyber-attack occurrence on Connected Vehicles using Window of Opportunity

This repository provides the datasets used for prior information from Highways England, as well as the VEINS code, which is utilised as likelihood information for both Bluetooth and DSRC case studies, respectively.  


As a part of this research, the Window of Opportunity attack feasibility Likelihood Value Estimation through a Simulation-based approach (WOLVES) framework has been developed for computing Attack feasibility using the Window of Opportunity parameter in line with the TARA from ISO/SAE 21434 Standard. 

## WOLVES framework

<img width="10455" height="5740" alt="WOframework_2024_10_31" src="https://github.com/user-attachments/assets/c1acb2bc-9466-4181-a860-ed54263960d4" />

## Window of Opportunity is computed based on the Law of Total Probability
<img width="206" height="50" alt="image" src="https://github.com/user-attachments/assets/ed562b57-1b0a-43c9-88a4-106af3f44e33" />

<img width="446" height="60" alt="image" src="https://github.com/user-attachments/assets/319eb4d5-e0cc-4204-8089-afedae970463" />



## Prior information processing
The prior data has been collected from Highways England. Then, Data cleaning, processing, and PCU factor conversion were performed for the selected 3 months of data. 

<img width="9355" height="2255" alt="Prior Probability Architecture" src="https://github.com/user-attachments/assets/8eef737d-081c-4833-83f2-bb9da8cfed4d" />


## Road Network
For simulations, a road network has been created using NetEdit. The same road network has been used in both the case studies and all the scenarios. The only change required is the maximum free-flow speed of the network based on Single carriageway (60 mph) and Dual carriageway (70 mph). 
<img width="686" height="170" alt="image" src="https://github.com/user-attachments/assets/18d9e02b-762b-4afa-a5b6-9464dd0e42c6" />



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

Additionally, the performance of the anomaly detection model has been improved using the Youden J index.




## Vulnerability data
Additionally, some vulnerability information is provided; this can also be incorporated in the Window of Opportunity likelihood computation if particular vehicle models and makes that are affected are known. 




### Citation 
```bibtex
@article{kamtam2025wolves,
  title={WOLVES: Window of Opportunity attack feasibility likelihood value estimation through a simulation-based approach},
  author={Kamtam, Suraj Harsha and Lu, Qian and Rakib, Abdur and Ramli, Muhamad Azfar and Mepparambath, Rakhi Manohar and Shaikh, Siraj Ahmed and Nguyen, Hoang Nga},
  journal={Computers \& Security},
  pages={104549},
  year={2025},
  publisher={Elsevier}
}



