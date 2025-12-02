import subprocess
import sys

import setup_premake
import setup_opencv

from utils import get_platform, Platform


def check_platform():
    platform = get_platform()
    if platform not in (Platform.LINUX, Platform.WINDOWS, Platform.MACOS):
        sys.exit(f"Unsupported platform: {platform}")
    return platform


def run_git_submodules():
    print("\nUpdating submodules...")
    try:
        subprocess.run(["git", "submodule", "update",
                       "--init", "--recursive"], check=True)
    except subprocess.CalledProcessError as e:
        sys.exit(f"Failed to update submodules {e}")


def install_dependencies():
    setup_premake.install_premake()
    setup_opencv.install_opencv()


platform = check_platform()
install_dependencies()
run_git_submodules()

# Platform-specific logic
if platform == Platform.WINDOWS:
    # TODO: Add Windows-specific steps here
    pass
else:
    # TODO: Add Linux/Mac-specific steps here
    pass
