from distutils.dir_util import copy_tree
from pathlib import Path
import shutil

port_path = "SDK"

Path(port_path).mkdir(parents=True, exist_ok=True)
Path(port_path + "/Externals").mkdir(parents=True, exist_ok=True)
Path(port_path + "/Plugins").mkdir(parents=True, exist_ok=True)
Path(port_path + "/Resources").mkdir(parents=True, exist_ok=True)
Path(port_path + "/Sources").mkdir(parents=True, exist_ok=True)

copy_tree("Externals", port_path + "/Externals")
copy_tree("Plugins", port_path + "/Plugins")
copy_tree("Resources", port_path + "/Resources")
copy_tree("Sources", port_path + "/Sources")
shutil.copy("__SDKCMakeFiles/Sources/CMakeLists.txt", port_path + "/Sources")
shutil.rmtree(port_path + "/Sources/SGEntry")
shutil.copy(".gitattributes", port_path)
shutil.copy(".gitignore", port_path)
shutil.copy("CMakePresets.json", port_path)
shutil.copy("__SDKCMakeFiles/CMakeLists.txt", port_path)
shutil.copy("__SDKCMakeFiles/FindSungearEngineSDK.cmake", port_path)
shutil.copy("__SDKCMakeFiles/portfile.cmake", port_path)
shutil.copy("__SDKCMakeFiles/vcpkg.json", port_path)
