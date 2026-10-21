import re
from pathlib import Path

import pandas as pd

LAB_DIR = Path(__file__).resolve().parent
PRECONDITIONERS = ["none", "jacobi", "sor", "ilu", "icc", "gamg"]
COLUMNS = ["grid_size", "iters", "time [s]", "condition number"]

# e.g. " 128 x 128 gmres pc = ilu", " 32 x 32 gmres restart = 30", " 8 x 8 cg"
HEADER = re.compile(r"^\s*(\d+) x \d+ (\w+)(?: restart = (\d+))?(?: pc = (\w+))?")
ITERS = re.compile(r"Linear solve .* iterations (\d+)")
REAL = re.compile(r"^real\s+(\d+)m([\d.]+)s")
COND = re.compile(r"max/min ([\d.eE+-]+)")


def parse_runs(path):
    """Parse one output file into a DataFrame with one row per run."""
    runs = []
    for line in path.read_text().splitlines():
        if m := HEADER.match(line):
            runs.append({
                "grid_size": int(m[1]),
                "ksp": m[2],
                "restart": int(m[3]) if m[3] else None,
                "pc": m[4] or "none",
            })
        elif m := ITERS.search(line):
            runs[-1]["iters"] = int(m[1])
        elif m := REAL.match(line):
            runs[-1]["time [s]"] = 60 * int(m[1]) + float(m[2])
        elif m := COND.search(line):
            runs[-1]["condition number"] = float(m[1])
    return pd.DataFrame(runs)


q2 = parse_runs(LAB_DIR / "q2_timing.txt")
q3a = parse_runs(LAB_DIR / "3a.txt")
q3b = parse_runs(LAB_DIR / "3b.txt")

# iterations and time: 3a, plus GMRES(30) with no preconditioner from exercise 2
no_pc = q2[(q2["ksp"] == "gmres") & (q2["restart"] == 30)].assign(pc="none")
perf_cols = ["pc", "grid_size", "iters", "time [s]"]
perf = pd.concat([q3a[perf_cols], no_pc[perf_cols]], ignore_index=True)

# condition number: 3b
perf = perf.merge(q3b[["pc", "grid_size", "condition number"]], on=["pc", "grid_size"], how="left")

precond_perf = {
    pc: perf[perf["pc"] == pc][COLUMNS].sort_values("grid_size").reset_index(drop=True)
    for pc in PRECONDITIONERS
}

if __name__ == "__main__":
    for pc, df in precond_perf.items():
        print(f"\n{pc}\n{df.to_string(index=False)}")
