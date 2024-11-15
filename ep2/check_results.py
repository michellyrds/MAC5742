import hashlib

def hash_file(file_path):
    hash_func = hashlib.md5()
    with open(file_path, 'rb') as f:
        for chunk in iter(lambda: f.read(4096), b""):
            hash_func.update(chunk)
    return hash_func.hexdigest()

def compare_files(file1, file2):
    return hash_file(file1) == hash_file(file2)

file1 = 'room.txt'
file2 = 'cuda_room.txt'

if compare_files(file1, file2):
    print("Results are equal.")
else:
    print("Results are not equal.")
