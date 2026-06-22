import argparse

from .run import main__run, adapters

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

    args = parser.parse_args()
    if hasattr(args, "func"):
        return args.func(args, parser)
    return main__run(args, parser)
