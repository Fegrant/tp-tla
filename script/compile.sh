
set -e
ROOT="$(dirname "$0")"

echo "Relocating script to '$ROOT'..."
cd "$ROOT/.."

cat "$1" | ./bin/Compiler

python3 output.py

echo "All done. Output images are in output directory."
