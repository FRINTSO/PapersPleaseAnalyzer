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
vendor_dir = utils.get_project_root() / "vendor"

opencv_version = "4.x"
opencv_dir = vendor_dir / "opencv"
opencv_src_dir = opencv_dir / "src"
opencv_build_dir = opencv_dir / "build"
opencv_install_dir = opencv_dir / "install"

opencv_archive_name = f"{opencv_version}{__platform_archive_table[platform]}"
opencv_archive_url = f"https://github.com/opencv/opencv/archive/{opencv_archive_name}"
opencv_archive_path = str(opencv_dir / opencv_archive_name)

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
    # "-DBUILD_opencv_highgui=ON",
]


def install_opencv() -> bool:
    if is_installed():
        return True

    opencv_dir.mkdir(0o755, parents=True, exist_ok=True)

    if not opencv_src_dir.exists():
        print("Download opencv")
        utils.download_file(opencv_archive_url, opencv_archive_path)
        print("Unzip opencv")
        utils.unzip_file(opencv_archive_path)
        # The archive extracts to opencv-4.x/, rename it to src/
        (opencv_dir / f"opencv-{opencv_version}").rename(opencv_src_dir)

    print("Making build and install directories")
    opencv_build_dir.mkdir(0o755, parents=True, exist_ok=True)
    opencv_install_dir.mkdir(0o755, parents=True, exist_ok=True)

    print("Building opencv")
    if subprocess.call(
        [
            "cmake",
            *opencv_build_options,
            "-DCMAKE_BUILD_TYPE=Release",
            f"-DCMAKE_INSTALL_PREFIX={opencv_install_dir}",
            str(opencv_src_dir),
        ],
        cwd=str(opencv_build_dir),
        stdout=sys.stdout,
        stderr=sys.stderr,
        ) != 0:
        return False

    print("Installing opencv")
    cores = os.cpu_count() or 4
    if subprocess.call(
        ["cmake", "--build", ".", "--config", "Release", "--parallel", str(cores)],
        cwd=str(opencv_build_dir),
        stdout=sys.stdout,
        stderr=sys.stderr,
        ) != 0:
        return False
    if subprocess.call(
        ["cmake", "--build", ".", "--config", "Debug", "--parallel", str(cores)],
        cwd=str(opencv_build_dir),
        stdout=sys.stdout,
        stderr=sys.stderr,
        ) != 0:
        return False
    if subprocess.call(
        [
            "cmake",
            "--build",
            ".",
            "--target",
            "install",
            "--config",
            "Release",
            "--parallel",
            str(cores),
        ],
        cwd=str(opencv_build_dir),
        stdout=sys.stdout,
        stderr=sys.stderr,
        ) != 0:
        return False
    if subprocess.call(
        [
            "cmake",
            "--build",
            ".",
            "--target",
            "install",
            "--config",
            "Debug",
            "--parallel",
            str(cores),
        ],
        cwd=str(opencv_build_dir),
        stdout=sys.stdout,
        stderr=sys.stderr,
        ) != 0:
        return False
    return True


def is_installed() -> bool:
    if platform == Platform.LINUX:
        return (opencv_install_dir / "lib" / "libopencv_core.so").exists()
    if platform == Platform.WINDOWS:
        # needs verification on windows
        return (opencv_install_dir / "x64" / "vc17" / "lib" / "opencv_core4130.lib").exists()
    if platform == Platform.MACOS:
        # needs verification on mac
        return (opencv_install_dir / "lib" / "libopencv_core.dylib").exists()
    return False
