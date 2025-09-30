import os
import subprocess
import sys

import utils
from utils import Platform

__platform_archive_table = {
    Platform.MACOS: ".tar.gz",
    Platform.LINUX: ".tar.gz",
    Platform.WINDOWS: ".zip",
}

platform = utils.get_platform()
vendor_dir = utils.get_project_root() / "paplease-core/vendor"

opencv_directory_name = "opencv-4.x"
opencv_archive_name = f"4.x{__platform_archive_table[platform]}"
opencv_archive_url = f"https://github.com/opencv/opencv/archive/{opencv_archive_name}"
opencv_archive_path = str(vendor_dir / opencv_archive_name)
opencv_directory = vendor_dir / opencv_directory_name

opencv_build_options = [
    "-DBUILD_PERF_TESTS:BOOL=OFF",
    "-DBUILD_TESTS:BOOL=OFF",
    "-DBUILD_DOCS:BOOL=OFF",
    "-DWITH_CUDA:BOOL=OFF",
    "-DBUILD_EXAMPLES:BOOL=OFF",
    "-DINSTALL_CREATE_DISTRIB=ON",
    "-DBUILD_SHARED_LIBS=ON",

    # Only include these modules
    "-DBUILD_LIST=core,imgproc,imgcodecs",
    "-DBUILD_opencv_world=OFF",
    
    # Optional GUI for development/debugging
    #"-DBUILD_opencv_highgui=ON",
]

def install_opencv() -> bool:
    if is_installed():
        return True

    if not opencv_directory.exists():
        print("Download opencv")
        utils.download_file(opencv_archive_url, opencv_archive_path)
        print("Unzip opencv")
        utils.unzip_file(opencv_archive_path)

    print("Making build and install directories")
    build_dir = (vendor_dir / "opencv-build")
    build_dir.mkdir(0o755, parents=True, exist_ok=True)

    install_dir = (vendor_dir / "opencv-install")
    install_dir.mkdir(0o755, parents=True, exist_ok=True)

    # build opencv
    print("Building opencv")
    subprocess.call([
        "cmake",
        *opencv_build_options,
        "-DCMAKE_BUILD_TYPE=Release",
        f"-DCMAKE_INSTALL_PREFIX={install_dir}",
        "../opencv-4.x"
    ], cwd=str(build_dir), stdout=sys.stdout, stderr=sys.stderr)

    # install opencv
    print("Installing opencv")
    cores = os.cpu_count() or 4
    subprocess.call(["cmake", "--build", ".", "--config", "Release", "--parallel", str(cores)], cwd=str(build_dir), stdout=sys.stdout, stderr=sys.stderr)
    subprocess.call(["cmake", "--build", ".", "--config", "Debug", "--parallel", str(cores)], cwd=str(build_dir), stdout=sys.stdout, stderr=sys.stderr)
    subprocess.call(["cmake", "--build", ".", "--target", "install", "--config", "Release", "--parallel", str(cores)], cwd=str(build_dir), stdout=sys.stdout, stderr=sys.stderr)
    subprocess.call(["cmake", "--build", ".", "--target", "install", "--config", "Debug", "--parallel", str(cores)], cwd=str(build_dir), stdout=sys.stdout, stderr=sys.stderr)
    return True

def is_installed() -> bool:
    return False
