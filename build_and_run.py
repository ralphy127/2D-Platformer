import subprocess
import os
import sys

source_dir = os.path.abspath(".")
build_dir = os.path.join(source_dir, "build")
executable = os.path.join(build_dir, "Game", "Game")

if not os.path.exists(build_dir):
    os.makedirs(build_dir)

print("Configuring CMake...")
cmake_result = subprocess.run(["cmake", "-S", source_dir, "-B", build_dir])

if cmake_result.returncode != 0:
    print("Error: CMake configuration failed")
    sys.exit(1)

print("Building project...")
build_result = subprocess.run(["cmake", "--build", build_dir])

if build_result.returncode != 0:
    print("Error: build failed")
    sys.exit(1)

if os.path.isfile(executable):
    print("Running executable...")
    subprocess.run([executable], cwd=os.path.dirname(executable)) 
else:
    print(f"Error: executable not found: {executable}")
