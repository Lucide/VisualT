# -- readthedocs CI ----------------------------------------------------------

# import os

# read_the_docs_build = os.environ.get('READTHEDOCS', None) == "True"

# if read_the_docs_build:
import subprocess
import sys

subprocess.check_call(["cmake -D VisualT_ASSUME_LITTLE_ENDIAN:BOOL=YES -S . -B build"], cwd="../..", shell=True)
subprocess.check_call(["cmake --build build --target VisualT_doxygen_generate_xml"], cwd="../..", shell=True)
subprocess.call([sys.executable, "build/doc/conf.py"], cwd="../..")
