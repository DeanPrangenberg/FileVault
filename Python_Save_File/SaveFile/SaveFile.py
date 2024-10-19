import json
import os

SAVE_FILE_PATH = "save_file.json"

def add_struct_to_json(new_struct):
    try:
        with open(SAVE_FILE_PATH, 'r') as f:
            data = json.load(f)
    except FileNotFoundError:
        data = []
        print("Couldn't find save_file.json");

    data.append(new_struct)

    with open(SAVE_FILE_PATH, 'w') as f:
        json.dump(data, f, indent=4)

    print("New Struct got appended to save_file.json.")

def create_json_if_not_exists():
    if not os.path.exists(SAVE_FILE_PATH):
        with open(SAVE_FILE_PATH, 'w') as f:
            json.dump([], f)
        print(f"JSON-Datei '{SAVE_FILE_PATH}' created.")
    else:
        print(f"JSON-Datei '{SAVE_FILE_PATH}' already exists.")

def find_and_complete_struct(partial_struct):
    with open(SAVE_FILE_PATH, 'r') as f:
        data = json.load(f)

    for entry in data:
        if all(partial_struct[key] == entry[key] for key in partial_struct if key in entry):
            complete_struct = {**entry}
            return complete_struct

    print("Couldn't find the Struct in save_file.json.")
    return None

def delete_struct_from_json(partial_struct):
    with open(SAVE_FILE_PATH, 'r') as f:
        data = json.load(f)

    new_data = []
    struct_found = False

    for entry in data:
        if all(partial_struct[key] == entry[key] for key in partial_struct if key in entry):
            struct_found = True
        else:
            new_data.append(entry)

    if struct_found:
        with open(SAVE_FILE_PATH, 'w') as f:
            json.dump(new_data, f)
        print("Struct deleted got deleted from save_file.json.")
    else:
        print("Couldn't find the Struct in save_file.json.")
