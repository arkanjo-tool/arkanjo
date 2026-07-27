import json
import sys

with open(sys.argv[1], encoding="utf-8") as f:
    config = json.load(f)

for method in config.get("methods", []):
    print(method)
