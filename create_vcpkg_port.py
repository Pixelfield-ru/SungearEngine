from distutils.dir_util import copy_tree
from pathlib import Path
import shutil

port_path = "ports/SungearEngine"

Path(port_path).mkdir(parents=True, exist_ok=True)

shutil.copy("portfile.cmake", port_path)
shutil.copy("vcpkg.json", port_path)
