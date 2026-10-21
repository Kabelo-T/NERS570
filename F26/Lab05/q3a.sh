#!/usr/bin/bash
#SBATCH --job-name=q3
#SBATCH --account=ners570f26_class
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=90G
#SBATCH --time=02:00:00

module load gcc/15.1.0 openmpi/5.0.10

EXE=/gpfs/accounts/ners570f26_class_root/ners570f26_class/kabelo/petsc/share/petsc/examples/src/ksp/ksp/tutorials/ex15

# GMRES with the default restart length, 1 processor
ksp_type=gmres
for pc_type in jacobi sor ilu icc gamg
do
    for dim in 8 32 128 256 512 1024
    do
        echo -e " ${dim} x ${dim} ${ksp_type} pc = ${pc_type}\n"
        time mpiexec -np 1 ${EXE} -pc_type $pc_type -m ${dim} -n ${dim} \
        -ksp_type $ksp_type \
        -ksp_converged_reason
        echo
    done
done > 3a.txt 2>&1
