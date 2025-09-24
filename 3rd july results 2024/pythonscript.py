import xml.etree.ElementTree as ET
import subprocess
import shutil
import os

def get_depart_value(rou_xml_file):
    tree = ET.parse(rou_xml_file)
    root = tree.getroot()

    # Find the first vehicle definition and get its depart value
    vehicle = root.find('.//vehicle')
    if vehicle is not None:
        return int(vehicle.get('depart'))
    else:
        raise ValueError("No vehicle definition found in the XML file.")

def change_depart_value(rou_xml_file, new_depart_value):
    tree = ET.parse(rou_xml_file)
    root = tree.getroot()

    # Find vehicle elements with id="v0" and update their depart value
    for vehicle in root.findall('.//vehicle[@id="v0"]'):
        vehicle.set('depart', str(new_depart_value))

    # Write the modified XML back to the file
    tree.write(rou_xml_file)

def change_seed_value(launchd_xml_file, new_seed_value):
    tree = ET.parse(launchd_xml_file)
    root = tree.getroot()

    # Find the seed element and update its value
    for seed_element in root.iter('seed'):
        seed_element.set('value', str(new_seed_value))

    # Write the modified XML back to the file
    tree.write(launchd_xml_file)

def execute_simulation(command):
    subprocess.run(command, shell=True)

if __name__ == "__main__":
    rou_xml_file_path = "/home/sidharth/Downloads/vant/simulations/losTest1/lostest1.rou.xml"
    launchd_xml_file_path = "/home/sidharth/Downloads/vant/simulations/losTest1/lostest1.launchd.xml"
    base_command = "opp_run -r 0 -m -u Cmdenv -n ..:../../src:../../../veins-5.2/examples/veins:../../../veins-5.2/src/veins --image-path=../../../veins-5.2/images -l ../../../veins-5.2/src/veins omnetpp.ini"
    output_base_folder = "/home/sidharth/Downloads/output_files"
    os.makedirs(output_base_folder, exist_ok=True)

    initial_depart_value = 890
    for depart_value in range(initial_depart_value, 901):  # Loop through depart values 900 to 910
        output_folder = os.path.join(output_base_folder, f"depart_{depart_value}")
        os.makedirs(output_folder, exist_ok=True)

        for i in range(1, 51):  # Run simulation 50 times for each depart value
            seed_value = i

            # Change depart value only for vehicle id="v0" in the rou.xml file
            change_depart_value(rou_xml_file_path, depart_value)

            # Change seed value in the launchd.xml file
            change_seed_value(launchd_xml_file_path, seed_value)

            # Run simulation
            execute_simulation(base_command)

            # Copy output files
            shutil.copy("/home/sidharth/Downloads/vant/simulations/losTest1/out.txt", os.path.join(output_folder, f"out_{i}.txt"))
            shutil.copy("/home/sidharth/Downloads/vant/simulations/losTest1/rt.txt", os.path.join(output_folder, f"rt_{i}.txt"))
            shutil.copy("/home/sidharth/Downloads/vant/simulations/losTest1/test.txt", os.path.join(output_folder, f"test_{i}.txt"))

