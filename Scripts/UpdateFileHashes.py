import os
import hashlib
import glob
import re
from datetime import datetime

# === CONFIGURATION ===
BASE_DIR = r'C:/Dev/AlyCompiler'
SRC_DIR = os.path.join(BASE_DIR, 'src')
MANIFEST_FILE = os.path.join(BASE_DIR, 'manifest_hash.txt')

# === HASH FUNCTION ===
def compute_file_hash(file_path):
    hasher = hashlib.sha256()
    with open(file_path, 'rb') as f:
        while chunk := f.read(4096):
            hasher.update(chunk)
    return hasher.hexdigest()

# === WRITE TO MANIFEST ===
def save_hash_manifest(file_path, file_hash, section):
    relative_path = os.path.relpath(file_path, BASE_DIR).replace('\\', '/')
    timestamp = datetime.now().strftime("%Y-%m-%d, %H:%M:%S")

    # Read the existing manifest file
    manifest_data = {}
    if os.path.exists(MANIFEST_FILE):
        with open(MANIFEST_FILE, 'r') as mf:
            section_name = None
            for line in mf.readlines():
                if line.startswith("=== "):  # Track sections like "=== Tracked Source Files ==="
                    section_name = line.strip()
                    manifest_data[section_name] = []
                elif section_name and line.startswith("Filename:"):
                    filename = line.split(":")[1].strip()
                    current_hash = line.split("Current Hash:")[1].split("\n")[0].strip()
                    timestamp = line.split("Updated:")[1].split("\n")[0].strip()
                    manifest_data[section_name].append({
                        'filename': filename,
                        'current_hash': current_hash,
                        'timestamp': timestamp,
                    })

    # Check if the file already exists in the manifest (for updating the timestamp and hash)
    existing_entry = None
    for entry in manifest_data.get(section, []):
        if entry['filename'] == relative_path:
            existing_entry = entry
            break

    # If the file exists, update the current hash and timestamp
    if existing_entry:
        existing_entry['current_hash'] = file_hash
        existing_entry['timestamp'] = timestamp
    else:
        # If it's a new file, add it as an entry
        new_entry = {
            'filename': relative_path,
            'current_hash': file_hash,
            'timestamp': timestamp,
        }
        if section not in manifest_data:
            manifest_data[section] = []
        manifest_data[section].append(new_entry)

    # Write the updated manifest data back to the file
    with open(MANIFEST_FILE, 'w') as mf:
        for section, entries in manifest_data.items():
            mf.write(f"{section}\n")
            for entry in entries:
                mf.write(f"Filename: {entry['filename']}\n")
                mf.write(f"Current Hash: {entry['current_hash']}\n")
                mf.write(f"Updated: {entry['timestamp']}\n\n")
    print(f"Hash for {relative_path} saved to '{MANIFEST_FILE}' with timestamp.")

# === FOR C/H FILES ===
def insert_or_replace_hash_comment_for_source_files(file_path, file_hash):
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            content = file.read()

        relative_path = os.path.relpath(file_path, BASE_DIR).replace('\\', '/')
        timestamp = datetime.now().strftime("%Y-%m-%d, %H:%M:%S")

        copyright_block = (
            "/*\n"
            " * Copyright (c) 2025 NorthernL1ghts\n"
            " */\n\n"
        )

        hash_comment = (
            "/*\n"
            f" * SHA HASH: {file_hash}\n"
            f" * Filename: {relative_path}\n"
            f" * Updated: {timestamp}\n"
            "*/\n"
        )

        # Remove any existing full metadata block
        content = re.sub(
            r"(?s)^/\*\n \* Copyright \(c\) 2025 NorthernL1ghts\n \*/\n\n?"
            r"/\*\n \* SHA HASH: .*?\n \* Filename: .*?\n \* Updated: .*?\n\*/\n?",
            '', content, count=1
        )

        new_content = copyright_block + hash_comment + content.lstrip()

        with open(file_path, 'w', encoding='utf-8') as file:
            file.write(new_content)

        print(f"Updated hash block in {relative_path}")

    except Exception as e:
        print(f"Error processing {file_path}: {e}")

# === FOR .aly and .S FILES ===
def insert_or_replace_hash_comment_for_script_files(file_path, file_hash):
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            content = file.read()

        relative_path = os.path.relpath(file_path, BASE_DIR).replace('\\', '/')
        timestamp = datetime.now().strftime("%Y-%m-%d, %H:%M:%S")

        copyright_block = ";#; Copyright (c) 2025 NorthernL1ghts"
        hash_comment = (
            f";#; SHA HASH: {file_hash}\n"
            f";#; Filename: {relative_path}\n"
            f";#; Updated: {timestamp}"
        )

        full_block_pattern = re.compile(
            r"^;#; Copyright \(c\) 2025 NorthernL1ghts\n"
            r";#; SHA HASH: .*\n"
            r";#; Filename: .*\n"
            r";#; Updated: .*\n?",
            re.MULTILINE
        )

        content = re.sub(full_block_pattern, '', content, count=1)
        new_content = f"{copyright_block}\n{hash_comment}\n\n{content.lstrip()}"

        with open(file_path, 'w', encoding='utf-8') as file:
            file.write(new_content)

        print(f"Updated hash block in {relative_path}")

    except Exception as e:
        print(f"Error processing {file_path}: {e}")

# === FOR .bnf FILES ===
def insert_or_replace_hash_comment_for_bnf_files(file_path, file_hash):
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            content = file.read()

        relative_path = os.path.relpath(file_path, BASE_DIR).replace('\\', '/')
        timestamp = datetime.now().strftime("%Y-%m-%d, %H:%M:%S")

        copyright_block = ";#; Copyright (c) 2025 NorthernL1ghts"
        hash_comment = (
            f";#; SHA HASH: {file_hash}\n"
            f";#; Filename: {relative_path}\n"
            f";#; Updated: {timestamp}"
        )

        full_block_pattern = re.compile(
            r"^;#; Copyright \(c\) 2025 NorthernL1ghts\n"
            r";#; SHA HASH: .*\n"
            r";#; Filename: .*\n"
            r";#; Updated: .*\n?",
            re.MULTILINE
        )

        content = re.sub(full_block_pattern, '', content, count=1)
        new_content = f"{copyright_block}\n{hash_comment}\n\n{content.lstrip()}"

        with open(file_path, 'w', encoding='utf-8') as file:
            file.write(new_content)

        print(f"Updated hash block in {relative_path}")

    except Exception as e:
        print(f"Error processing {file_path}: {e}")

# === MAIN FUNCTION ===
def main():
    # Iterate over all files in the relevant directories
    tracked_files = glob.glob(f'{SRC_DIR}/**/*.c', recursive=True) + \
                     glob.glob(f'{SRC_DIR}/**/*.h', recursive=True) + \
                     glob.glob(f'{BASE_DIR}/examples/**/*.aly', recursive=True) + \
                     glob.glob(f'{BASE_DIR}/examples/**/*.S', recursive=True) + \
                     glob.glob(f'{BASE_DIR}/*.aly', recursive=False) + \
                     glob.glob(f'{BASE_DIR}/*.S', recursive=False) + \
                     glob.glob(f'{BASE_DIR}/*.bnf', recursive=False)

    # Debugging line to check files found
    print(f"Tracked files: {tracked_files}")

    # Initialize sections in manifest
    manifest_data = {
        'Tracked Source Files': [],
        'Tracked Scripts/Examples': [],
        'Tracked BNF': []
    }

    for file in tracked_files:
        try:
            # Compute the file hash
            file_hash = compute_file_hash(file)

            # Handle file hash insertion and manifest updating
            if file.endswith(('.c', '.h')):
                insert_or_replace_hash_comment_for_source_files(file, file_hash)
                manifest_data['Tracked Source Files'].append({
                    'filename': os.path.relpath(file, BASE_DIR).replace('\\', '/'),
                    'current_hash': file_hash,
                    'timestamp': datetime.now().strftime("%Y-%m-%d, %H:%M:%S")
                })

            elif file.endswith(('.aly', '.S')):
                insert_or_replace_hash_comment_for_script_files(file, file_hash)
                manifest_data['Tracked Scripts/Examples'].append({
                    'filename': os.path.relpath(file, BASE_DIR).replace('\\', '/'),
                    'current_hash': file_hash,
                    'timestamp': datetime.now().strftime("%Y-%m-%d, %H:%M:%S")
                })

            elif file.endswith('.bnf'):
                insert_or_replace_hash_comment_for_bnf_files(file, file_hash)
                manifest_data['Tracked BNF'].append({
                    'filename': os.path.relpath(file, BASE_DIR).replace('\\', '/'),
                    'current_hash': file_hash,
                    'timestamp': datetime.now().strftime("%Y-%m-%d, %H:%M:%S")
                })

        except Exception as e:
            print(f"Error processing {file}: {e}")

    # Write the updated manifest data back to the file
    with open(MANIFEST_FILE, 'w') as mf:
        for section, entries in manifest_data.items():
            mf.write(f"{section}\n")
            for entry in entries:
                mf.write(f"Filename: {entry['filename']}\n")
                mf.write(f"Current Hash: {entry['current_hash']}\n")
                mf.write(f"Updated: {entry['timestamp']}\n\n")

    print(f"Manifest has been updated and saved to {MANIFEST_FILE}")

if __name__ == "__main__":
    main()
