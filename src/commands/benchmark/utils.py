import subprocess

from pathlib import Path
from tqdm import tqdm

import jaydebeapi

def normalize(a: str, b: str):
    return tuple(sorted((str(a), str(b))))

class CursorManager:
    def __init__(self, h2_path, h2_jar, username = "sa", password = ""):
        self.conn = jaydebeapi.connect(
            "org.h2.Driver",
            f"jdbc:h2:{h2_path}",
            [username, password],
            h2_jar
        )
        self.cursor = None

    def __enter__(self):
        self.cursor = self.conn.cursor()
        return self.cursor

    def __exit__(self, exc_type, exc_val, exc_tb):
        if self.cursor:
            self.cursor.close()
            self.cursor = None
        if self.conn:
            self.conn.close()
            self.conn = None

def create_name(config, index = 0):
    return (
        f'{config.dataset.replace("_", "-")}_'
        f'm{config.arkanjo.method}_'
        f'i{str(index)}'
    )

def create_batch(
    ds, batch_index: int, workdir: str | Path, config, adapter
):
    total_rows = len(ds)

    start = batch_index * config.max_files
    end = min(start + config.max_files, total_rows)

    batch = ds.select(range(start, end))

    for row in tqdm(batch, leave=False, desc="Copying files"):
        adapter.create_batch_iter(workdir, row)

    return batch

def arkanjo_preprocessor(folder: str | Path, config):
    command = [
        "arkanjo", "preprocessor", "build",
        "--name", config.arkanjo.name,
        "--minimum-lines", "3",
        "--path", str(folder),
        "--method", config.arkanjo.method,
        "-S", "1.0",
        "--granularity", config.arkanjo.granularity,
        "--verbose"
    ]
    return subprocess.run(
        command,
        capture_output=True,
        text=True
    )

def arkanjo_explorer(config, threshold):
    command = [
        "arkanjo", "explorer",
        "--name", config.arkanjo.name,
        "-S", str(threshold),
        "--no-color",
        "--template", "{filename_a},{filename_b}",
    ]
    return subprocess.run(
        command,
        capture_output=True,
        text=True
    )

def cursor_foreach(cursor, batch_size=1000):
    while True:
        batch = cursor.fetchmany(batch_size)
        if not batch:
            break
        yield from batch
