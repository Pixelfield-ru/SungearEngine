from distutils.dir_util import copy_tree
from pathlib import Path
import shutil

port_path = "SDK/ports/SungearEngineSDK"

Path(port_path).mkdir(parents=True, exist_ok=True)

shutil.copy("__SDKCMakeFiles/portfile.cmake", port_path)
shutil.copy("__SDKCMakeFiles/vcpkg.json", port_path)
