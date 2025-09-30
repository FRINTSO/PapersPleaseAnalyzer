import platform
import subprocess

import setup_premake
import setup_opencv

from utils import get_platform, Platform

platform = platform.system()
if platform not in ("Linux", "Windows", "Darwin"):
    raise SystemExit(f"Unsupported platform {platform}")

setup_premake.install_premake()
setup_opencv.install_opencv()

print("\nUpdating submodules...")
subprocess.call(["git", "submodule", "update", "--init", "--recursive"])

platform = get_platform()
if platform == Platform.WINDOWS:
    pass
else:
    pass

