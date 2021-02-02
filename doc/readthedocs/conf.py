# -- readthedocs CI ----------------------------------------------------------

import subprocess
import os

current_dir = os.path.dirname(os.path.realpath(__file__))
subprocess.run(["cmake -P ci.cmake"], cwd=current_dir, shell=True, check=True)
with open("../../build/doc/conf.py") as infile:
    exec(infile.read())
