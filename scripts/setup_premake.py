from pathlib import Path

import utils
from utils import Platform

__premake_platform_suffix_table = {
    Platform.MACOS: "macosx.tar.gz",
    Platform.LINUX: "linux.tar.gz",
    Platform.WINDOWS: "windows.zip",
}

__premake_platform_executable_name_table = {
    Platform.MACOS: "premake5",
    Platform.LINUX: "premake5",
    Platform.WINDOWS: "premake5.exe",
}

platform = utils.get_platform()
project_root = (Path(__file__).parent.parent).resolve()

premake_version = "5.0.0-beta7"
premake_archive_name = f"premake-{premake_version}-{
    __premake_platform_suffix_table[platform]}"
premake_zip_url = f"https://github.com/premake/premake-core/releases/download/v{
    premake_version}/{premake_archive_name}"
premake_bin_dir = project_root / "vendor/premake/bin"
premake_archive_dir = str(premake_bin_dir / premake_archive_name)
premake_executable = premake_bin_dir / \
    __premake_platform_executable_name_table[platform]


def install_premake() -> bool:
    if is_installed():
        return True
    print("Installing premake")
    utils.download_file(premake_zip_url, premake_archive_dir)
    utils.unzip_file(premake_archive_dir)
    print("Premake installed")
    return True


def is_installed() -> bool:
    return premake_executable.exists()
