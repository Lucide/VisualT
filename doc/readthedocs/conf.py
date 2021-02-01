# -- readthedocs CI ----------------------------------------------------------

import subprocess

subprocess.run(["cp -r -l -n doc/!(readthedocs) doc/readthedocs"], capture_output=True, cwd="../..", shell=True, check=True)
subprocess.run(["cmake -D VisualT_ASSUME_LITTLE_ENDIAN:BOOL=YES -S . -B build"], cwd="../..", shell=True, check=True)
subprocess.run(["cmake --build build --target VisualT_doxygen_generate_xml"], cwd="../..", shell=True, check=True)
with open("../../build/doc/conf.py") as infile:
    exec(infile.read())
