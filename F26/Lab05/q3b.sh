#!/usr/bin/bash
#SBATCH --job-name=b
#SBATCH --account=ners570f26_class
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=90G
#SBATCH --time=04:00:00

module load gcc/15.1.0 openmpi/5.0.10

EXE=/gpfs/accounts/ners570f26_class_root/ners570f26_class/kabelo/petsc/share/petsc/examples/src/ksp/ksp/tutorials/ex15

# Condition number estimates from GMRES with no restart, 1 processor
# Cheap preconditioners first so a timeout only loses the expensive cases (jacobi, none)
ksp_type=gmres
rstart=10000
for pc_type in gamg ilu icc sor jacobi none
do
    for dim in 8 32 128 256 512 1024
    do
        echo -e " ${dim} x ${dim} ${ksp_type} restart = ${rstart} pc = ${pc_type}\n"
        time mpiexec -np 1 ${EXE} -pc_type $pc_type -m ${dim} -n ${dim} \
        -ksp_type $ksp_type \
        -ksp_max_it 10000 -ksp_gmres_restart $rstart \
        -ksp_converged_reason \
        -ksp_view_singularvalues
        echo
    done
done > 3b.txt 2>&1
