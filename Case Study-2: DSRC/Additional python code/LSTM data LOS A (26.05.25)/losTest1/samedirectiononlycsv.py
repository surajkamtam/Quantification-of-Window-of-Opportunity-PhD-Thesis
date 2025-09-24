import xml.etree.ElementTree as ET
import subprocess
import os

def get_depart_value(rou_xml_file):
    tree = ET.parse(rou_xml_file)
    root = tree.getroot()

    vehicle = root.find('.//vehicle[@id="v0"]')
    if vehicle is not None:
        return int(vehicle.get('depart'))
    else:
        raise ValueError("No vehicle definition found in the XML file.")

def change_depart_value(rou_xml_file, new_depart_value, new_vehs_per_hour):
    tree = ET.parse(rou_xml_file)
    root = tree.getroot()

    # Update flows
    for flow in root.findall('.//flow'):
        flow.set('vehsPerHour', str(new_vehs_per_hour))

    # Find the vehicle elements
    v0 = root.find('.//vehicle[@id="v0"]')
    h0 = root.find('.//vehicle[@id="h0"]')

    if v0 is not None:
        v0.set('depart', str(new_depart_value))

    # Rearranging order based on depart value
    if v0 is not None and h0 is not None:
        # Remove both from root
        root.remove(v0)
        root.remove(h0)

        # Insert in the correct order
        if new_depart_value <= 900:
            root.append(v0)
            root.append(h0)
        else:
            root.append(h0)
            root.append(v0)

    tree.write(rou_xml_file)

def change_seed_value(launchd_xml_file, new_seed_value):
    tree = ET.parse(launchd_xml_file)
    root = tree.getroot()

    for seed_element in root.iter('seed'):
        seed_element.set('value', str(new_seed_value))

    tree.write(launchd_xml_file)

def execute_simulation(command):
    subprocess.run(command, shell=True)

def generate_csv_from_results(results_folder, config_name, depart_value, iteration, output_csv_folder):
    expected_sca = f"{config_name}-#0.sca"
    expected_vec = f"{config_name}-#0.vec"

    sca_path = os.path.join(results_folder, expected_sca)
    vec_path = os.path.join(results_folder, expected_vec)

    if os.path.exists(sca_path):
        scalar_csv_name = f"{config_name}_depart_{depart_value}_iter_{iteration}_scalar.csv"
        scalar_csv_path = os.path.join(output_csv_folder, scalar_csv_name)
        scalar_cmd = f'scavetool x "{sca_path}" -o "{scalar_csv_path}"'
        subprocess.run(scalar_cmd, shell=True)
    else:
        print(f"[WARN] Missing scalar file: {sca_path}")

    if os.path.exists(vec_path):
        vector_csv_name = f"{config_name}_depart_{depart_value}_iter_{iteration}_vector.csv"
        vector_csv_path = os.path.join(output_csv_folder, vector_csv_name)
        vector_cmd = f'scavetool x "{vec_path}" -o "{vector_csv_path}"'
        subprocess.run(vector_cmd, shell=True)
    else:
        print(f"[WARN] Missing vector file: {vec_path}")

if __name__ == "__main__":
    base_folder = "/home/suraj/Downloads/Simulation/losTest1"
    rou_xml_file_path = os.path.join(base_folder, "lostest1.rou.xml")
    launchd_xml_file_path = os.path.join(base_folder, "lostest1.launchd.xml")
    results_folder_path = os.path.join(base_folder, "results")
    output_base_folder = "/home/suraj/Downloads/output_files_FDI_general1300"
    initial_depart_value = 895
    final_depart_value = 906
    simulation_runs = 30
    vehs_per_hour_values = [1300]
    #config_names = ["SporadicAttack"]
    config_names = ["General"]

    os.makedirs(output_base_folder, exist_ok=True)

    for config_name in config_names:
        for vehs_per_hour in vehs_per_hour_values:
            vehs_output_folder = os.path.join(output_base_folder, f"{config_name}_vehsPerHour_{vehs_per_hour}")
            os.makedirs(vehs_output_folder, exist_ok=True)

            for depart_value in range(initial_depart_value, final_depart_value):
                depart_output_folder = os.path.join(vehs_output_folder, f"depart_{depart_value}")
                os.makedirs(depart_output_folder, exist_ok=True)

                for i in range(1, simulation_runs + 1):
                    seed_value = i

                    change_depart_value(rou_xml_file_path, depart_value, vehs_per_hour)
                    change_seed_value(launchd_xml_file_path, seed_value)

                    base_command = (
                        f"opp_run -c {config_name} -r 0 -m -u Cmdenv "
                        f"-n .:../veins-5.2/examples/veins:../veins-5.2/src/veins "
                        f"--image-path=../veins-5.2/images "
                        f"-l ../veins-5.2/src/veins omnetpp.ini"
                    )
                    execute_simulation(base_command)

                    generate_csv_from_results(
                        results_folder=results_folder_path,
                        config_name=config_name,
                        depart_value=depart_value,
                        iteration=i,
                        output_csv_folder=depart_output_folder
                    )

    print("✅ All simulations done! Only scalar and vector CSVs saved.")

