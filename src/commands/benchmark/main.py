import argparse

from .run import main__run, adapters
from .inspect import main__inspect

def main():
    parser = argparse.ArgumentParser(
        description="Run and inspect Arkanjo benchmark evaluations."
    )
    parser.add_argument(
        "--dataset",
        choices=adapters.keys(),
        required=True,
    )
    parser.add_argument(
        "--seed", type=int,
        default=42,
        help="Random seed used for reproducible sampling."
    )
    parser.add_argument(
        "-m", "--max-files", type=int,
        default=5000,
        help="Maximum number of files to load from the dataset."
    )
    parser.add_argument(
        "--methods", type=lambda s: s.split(","),
        help=(
            "Comma-separated list of Arkanjo matching methods to evaluate. "
            "Example: 1,2,3"
        )
    )
    parser.add_argument(
        "--h2-path", type=str,
        help="Path to the H2 database file."
    )

    parser.add_argument(
        "--h2-jar", type=str,
        help="Path to the H2 JDBC driver JAR."
    )
    parser.add_argument(
        "--fast", action="store_true",
        help="Run a reduced benchmark suite (10%%)."
    )

    subparsers = parser.add_subparsers(dest="command")

    inspect_parser = subparsers.add_parser(
        "inspect",
        help="Inspect benchmark results interactively.",
        description=(
            "Open an interactive interface to review benchmark results."
        ),
    )
    inspect_parser.set_defaults(func=main__inspect)
    inspect_parser.add_argument(
        "file",
        help="Path to the benchmark inspection file."
    )

    inspect_parser.add_argument(
        "--result",
        choices=["TP", "FP", "FN", "TN"],
        help="Filter inspection results by classification outcome."
    )

    args = parser.parse_args()
    if hasattr(args, "func"):
        return args.func(args, parser)
    return main__run(args, parser)
