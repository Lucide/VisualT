# -- readthedocs CI ----------------------------------------------------------

import subprocess

subprocess.run(["cmake", "-P", "ci.cmake"], check=True)
with open("../../build/doc/conf.py") as infile:
    exec(infile.read())
