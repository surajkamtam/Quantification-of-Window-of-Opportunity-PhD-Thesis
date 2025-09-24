import xml.etree.ElementTree as ET
import subprocess
import shutil
import os

def get_depart_value(rou_xml_file):
    tree = ET.parse(rou_xml_file)
    root = tree.getroot()

    vehicle = root.find('.//vehicle')
    if vehicle is not None:
        return int(vehicle.get('depart'))
    else:
        raise ValueError("No vehicle definition found in the XML file.")

def change_depart_value(rou_xml_file, new_depart_value, new_vehs_per_hour):
    tree = ET.parse(rou_xml_file)
    root = tree.getroot()

    for flow in root.findall('.//flow'):
        flow.set('vehsPerHour', str(new_vehs_per_hour))

    for vehicle in root.findall('.//vehicle[@id="v0"]'):
        vehicle.set('depart', str(new_depart_value))

    tree.write(rou_xml_file)

def change_seed_value(launchd_xml_file, new_seed_value):
    tree = ET.parse(launchd_xml_file)
    root = tree.getroot()

    for seed_element in root.iter('seed'):
        seed_element.set('value', str(new_seed_value))

    tree.write(launchd_xml_file)

def execute_simulation(command):
    subprocess.run(command, shell=True)

if __name__ == "__main__":
    base_folder = "/home/kamtams/Downloads/Code/losTest1"
    rou_xml_file_path = os.path.join(base_folder, "lostest1.rou.xml")
    launchd_xml_file_path = os.path.join(base_folder, "lostest1.launchd.xml")
    output_base_folder = "/SHARE/output_files"
    initial_depart_value = 890
    final_depart_value = 900
    simulation_runs = 50
    vehs_per_hour_values = [400,500, 700, 900, 1100]

    os.makedirs(output_base_folder, exist_ok=True)

    base_command = "opp_run -r 0 -m -u Cmdenv -n ..:../../src:../../../veins-5.2/examples/veins:../../../veins-5.2/src/veins --image-path=../../../veins-5.2/images -l ../../../veins-5.2/src/veins omnetpp.ini"

    for vehs_per_hour in vehs_per_hour_values:
        vehs_output_folder = os.path.join(output_base_folder, f"vehsPerHour_{vehs_per_hour}")
        os.makedirs(vehs_output_folder, exist_ok=True)

        for depart_value in range(initial_depart_value, final_depart_value):  # Loop through depart values 890 to 901
            depart_output_folder = os.path.join(vehs_output_folder, f"depart_{depart_value}")
            os.makedirs(depart_output_folder, exist_ok=True)

            for i in range(1, simulation_runs + 1):  # Run simulation 50 times for each depart value
                seed_value = i

                # Change depart value only for vehicle id="v0" in the rou.xml file
                change_depart_value(rou_xml_file_path, depart_value, vehs_per_hour)

                # Change seed value in the launchd.xml file
                change_seed_value(launchd_xml_file_path, seed_value)

                # Run simulation
                execute_simulation(base_command)

                # Copy output files directly to depart output folder
                for file_name in ["out.txt", "rt.txt", "test.txt"]:
                    src_file = os.path.join(base_folder, file_name)
                    dest_file = os.path.join(depart_output_folder, f"{file_name.split('.')[0]}_{i}.txt")
                    shutil.copy(src_file, dest_file)

