import os
import platform
import tarfile
from enum import Enum
from pathlib import Path
from zipfile import ZipFile

import requests


def download_file(url: str, filepath: str) -> None:
    filepath = os.path.abspath(filepath)
    os.makedirs(os.path.dirname(filepath), exist_ok=True)
    response = requests.get(url)
    with open(filepath, "wb") as f:
        f.write(response.content)


def unzip_file(filepath: str, delete_zip_file: bool = True) -> None:
    archive_path = os.path.abspath(filepath)
    target_dir = os.path.dirname(archive_path)

    if filepath.endswith(".zip"):
        with ZipFile(archive_path, "r") as zip_file:
            zip_file.extractall(target_dir)
    elif filepath.endswith((".tar", ".tar.gz", ".tgz", ".tar.bz2", ".tar.xz")):
        with tarfile.open(archive_path, "r:*") as tar_file:
            tar_file.extractall(target_dir)
    else:
        raise ValueError(f"Unsupported archive format: {filepath}")

    if delete_zip_file:
        os.remove(archive_path)


class Platform(Enum):
    WINDOWS = "Windows"
    LINUX = "Linux"
    MACOS = "Darwin"


def get_platform() -> Platform:
    system = platform.system()
    if system not in Platform:
        raise SystemExit("Unsupported platform: {system}")
    return Platform(system)


def get_project_root() -> Path:
    return (Path(__file__).parent.parent).resolve()
